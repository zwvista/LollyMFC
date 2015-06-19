
// BooksFrm.cpp : implementation of the CWordsEBFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsEBFrame.h"
#include "DataGridView.h"
#include "WordsEBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordsEBFrame

IMPLEMENT_DYNCREATE(CWordsEBFrame, CWordsBaseFrame)

BEGIN_MESSAGE_MAP(CWordsEBFrame, CWordsBaseFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_TB_LOOKUPBY_KANJI, ID_TB_LOOKUPBY_KANA, OnChangeLookupBy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_LOOKUPBY_KANJI, ID_TB_LOOKUPBY_KANA, OnUpdateChangeLookupBy)
	ON_COMMAND(ID_TB_FIND_KANA, OnFindKana)
	ON_COMMAND(ID_TB_COPY_KANJI_KANA, OnCopyKanjiKana)
	ON_UPDATE_COMMAND_UI(ID_TB_COPY_KANJI_KANA, OnUpdateCopyKanjiKana)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_TB_DICTS_AVAILABLE, ID_TB_DICTS_AVAILABLE + 50, OnAvailableDicts)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_DICTS_AVAILABLE, ID_TB_DICTS_AVAILABLE + 50, OnUpdateAvailableDicts)
	ON_COMMAND(ID_TB_REFRESH, OnRefresh)
END_MESSAGE_MAP()

// CWordsEBFrame construction/destruction

CWordsEBFrame::CWordsEBFrame()
	: m_bLookupByKana(false)
	, m_bIsFindingKana(false)
{
	m_nToolBarID = IDR_TOOLBAR_WORDSEB;
}

CWordsEBFrame::~CWordsEBFrame()
{
}

BOOL CWordsEBFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this, 1, 2)) return FALSE;

	CRect rect;
	GetClientRect(rect);
	CSize sz(550, rect.Height());

	CCreateContext context;
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDataGridView), sz, &context);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CWordsEBView), CSize(0, 0), &context);
	m_pView = DYNAMIC_DOWNCAST(CWordsEBView, m_wndSplitter.GetPane(0, 1));
	return __super::OnCreateClient(lpcs, &context);
}

int CWordsEBFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWordsBaseFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimerNavigate();

	return 0;
}

void CWordsEBFrame::OnSize( UINT nType, int cx, int cy )
{
	CWordsBaseFrame::OnSize(nType, cx, cy);
	CRect rc;
	m_wndToolBar.GetWindowRect(rc);
	ScreenToClient(rc);
	int ht = rc.Height();
	m_wndSplitter.MoveWindow(0, ht, cx, cy - ht);
	m_wndToolBar.CWnd::MoveWindow(0, 0, cx, ht);
}

void CWordsEBFrame::OnMoveComplete()
{
	CWordsBaseFrame::OnMoveComplete();
	m_strKana = m_rs.IsEof() ? CString() : m_rs.GetFieldValueAsString(_T("NOTE"));
	UpdateEBView();
}

void CWordsEBFrame::UpdateEBView()
{
	if(!m_bIsFindingKana && !m_strWord.IsEmpty())
		if(m_bLookupByKana)
			m_pView->Lookup(m_strKana), m_pView->SelectEntry(m_strWord);
		else
			m_pView->Lookup(m_strWord), m_pView->SelectEntry(m_strKana);
}

void CWordsEBFrame::OnChangeLookupBy( UINT nID )
{
	m_pView->SearchAllDicts(false);
	m_bLookupByKana = nID == ID_TB_LOOKUPBY_KANA;
}

void CWordsEBFrame::OnUpdateChangeLookupBy( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(pCmdUI->m_nID == ID_TB_LOOKUPBY_KANA == m_bLookupByKana);
}

void CWordsEBFrame::OnFindKana()
{
	if(m_rs.GetRecordCount() == 0) return;
	m_bIsFindingKana = true;
	m_pView->SearchAllDicts(false);
	//m_pView->ChoooseDict(_T("新明解国語辞典　第五版"));
	m_pView->ChooseDict(_T("三省堂　スーパー大辞林"));
	for(m_rs.MoveFirst(); !m_rs.IsEof(); m_rs.MoveNext()){
        CString strNote = m_rs.GetFieldValueAsString(_T("NOTE"));
		if(!strNote.IsEmpty()) continue;
		m_rs.Edit();
		m_rs.SetFieldValue(_T("NOTE"), m_pView->FindKana(m_rs.GetFieldValueAsString(_T("WORD"))));
		m_rs.Update();
	}
	m_wndGrid.Rebind();
	OnAddNew();
	m_bIsFindingKana = false;
}

void CWordsEBFrame::OnAvailableDicts( UINT nID )
{
	SelectDict(m_mapDictID2Info[nID].second - DICTIMAGE_OFFLINE);
}

void CWordsEBFrame::OnUpdateAvailableDicts( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(m_mapDictID2Info[pCmdUI->m_nID].second - DICTIMAGE_OFFLINE == m_nDictIndex);
}

void CWordsEBFrame::LoadDicts()
{
	m_wndToolBarDicts.RemoveAllButtons();

	//CMenu mnuDicts;
	//mnuDicts.CreateMenu();
	//int nID = ID_TB_DICTS_AVAILABLE;
	//int nImageIndex = DICTIMAGE_OFFLINE;
	//for(const CString strDictName : m_pConfig->m_vstrDictsEBWin){
	//	AddDict(nID, strDictName, (EDictImage)nImageIndex);
	//	m_mapDictID2Info[nID] = make_pair(strDictName, (EDictImage)nImageIndex++);
	//	mnuDicts.AppendMenu(MF_STRING, (UINT_PTR)nID++, strDictName);
	//}
	//m_pbtnDicts->CreateFromMenu(mnuDicts.Detach());

	SelectDict(0);
}

LRESULT CWordsEBFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsBaseFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID)
		CreateDictCtrls();
	return 0;
}

void CWordsEBFrame::OnDictSelected()
{
	const CString& strDict = m_mapDictID2Info.at(ID_TB_DICTS_AVAILABLE + m_nDictIndex).first;
	m_pView->ChooseDict(strDict);
	UpdateEBView();
}

BOOL CWordsEBFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN){
		WPARAM key = pMsg->wParam;
		bool bAlt = GetAsyncKeyState(VK_MENU) < 0;
		bool bCtl = GetAsyncKeyState(VK_CONTROL) < 0;
		bool bShift = GetAsyncKeyState(VK_SHIFT) < 0;

		if(bAlt){
			switch(key){
			case VK_UP:
				PostMessage(WM_COMMAND, ID_TB_NAVIGATE_BACKWARD);
				return TRUE;
			case VK_DOWN:
				PostMessage(WM_COMMAND, ID_TB_NAVIGATE_FORWARD);
				return TRUE;
			case VK_LEFT:
				//SelectNextDict(false, m_pbtnDicts->GetCount());
				return TRUE;
			case VK_RIGHT:
				//SelectNextDict(true, m_pbtnDicts->GetCount());
				return TRUE;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				//if(key < '1' + m_pbtnDicts->GetCount())
				//	SelectDict(key - '1');
				return TRUE;
			}
		}
	}

	return CWordsBaseFrame::PreTranslateMessage(pMsg);
}

void CWordsEBFrame::OnCopyKanjiKana()
{
	CString strText;
	CString strNote = m_rs.GetFieldValueAsString(_T("NOTE"));
	strText.Format(m_strWord != strNote ? _T("%s（%s）：") : _T("%s："), m_strWord, strNote);
	CopyTextToClipBoard(strText);
}

void CWordsEBFrame::OnUpdateCopyKanjiKana(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_strWord.IsEmpty());
}

void CWordsEBFrame::OnAddComplete()
{
	CWordsBaseFrame::OnAddComplete();
	OnFindKana();
	m_edtGridItem.SetFocus();
}

void CWordsEBFrame::OnRefresh()
{
	//m_vstrDicts.clear();
	CWordsBaseFrame::OnRefresh();
}
