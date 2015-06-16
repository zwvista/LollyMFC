
// BooksFrm.cpp : implementation of the CWordsWebFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsWebFrame.h"
#include "DataGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsWebFrame

IMPLEMENT_DYNCREATE(CWordsWebFrame, CWordsBaseFrame)

BEGIN_MESSAGE_MAP(CWordsWebFrame, CWordsBaseFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_TB_DELETE, OnDelete)
	ON_COMMAND_RANGE(ID_TB_EXTRACT_OVERWRITE, ID_TB_EXTRACT_APPEND, OnExtract)
	ON_UPDATE_COMMAND_UI(ID_TB_EXTRACT_OVERWRITE, OnUpdateExtractOverwrite)
	ON_UPDATE_COMMAND_UI(ID_TB_EXTRACT_APPEND, OnUpdateExtractAppend)
	ON_COMMAND(ID_TB_EDIT_TRANS, OnEditTrans)
	ON_UPDATE_COMMAND_UI(ID_TB_EDIT_TRANS, OnUpdateEditTrans)
	ON_COMMAND(ID_TB_DELETE_TRANS, OnDeleteTrans)
	ON_UPDATE_COMMAND_UI(ID_TB_DELETE_TRANS, OnUpdateDeleteTrans)
	ON_MESSAGE(WM_HTMLVIEW_DOCCOMPLETE, OnHtmlViewDocComplete)
	ON_COMMAND(ID_TB_REFRESH, OnRefresh)
END_MESSAGE_MAP()

// CWordsWebFrame construction/destruction

CWordsWebFrame::CWordsWebFrame()
	: m_pView(NULL)
	, m_pDictHtmlCtrl(NULL)
{
}

CWordsWebFrame::~CWordsWebFrame()
{
}

BOOL CWordsWebFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	CCreateContext context;
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDataGridView), CSize(450, 0), &context);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CWordsWebView), CSize(0, 0), &context);
	m_pView = DYNAMIC_DOWNCAST(CWordsWebView, m_wndSplitter.GetPane(0, 1));
	return CWordsBaseFrame::OnCreateClient(lpcs, &context);
}

int CWordsWebFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWordsBaseFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_pView->SetBrowerOptions(DLCTL_SILENT);
	//m_pView->SetSilent(TRUE);

	SetTimerNavigate();

	return 0;
}

void CWordsWebFrame::OnDelete()
{
	CString strMsg;
	strMsg.Format(_T("The word \"%s\" is about to be DELETED. Are you sure?"), m_strWord);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}

void CWordsWebFrame::OnExtract(UINT nID)
{
}

void CWordsWebFrame::OnUpdateExtractOverwrite( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(!m_strWord.IsEmpty() && m_pDictHtmlCtrl != NULL && m_pDictHtmlCtrl->CanExtractAndOverriteTranslation());
}

void CWordsWebFrame::OnUpdateExtractAppend( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(!m_strWord.IsEmpty() && m_pDictHtmlCtrl != NULL && m_pDictHtmlCtrl->CanExtractAndAppendTranslation());
}

void CWordsWebFrame::OnEditTrans()
{
    if(m_pDictHtmlCtrl->DoEditTranslation(m_strWord))
        UpdateHtml();
}

void CWordsWebFrame::OnUpdateEditTrans( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(!m_strWord.IsEmpty() && m_pDictHtmlCtrl != NULL && m_pDictHtmlCtrl->CanEditTranslation());
}

void CWordsWebFrame::OnDeleteTrans()
{
}

void CWordsWebFrame::OnUpdateDeleteTrans( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(!m_strWord.IsEmpty() && m_pDictHtmlCtrl != NULL && m_pDictHtmlCtrl->CanDeleteTranslation());
}

void CWordsWebFrame::LoadDicts()
{
	vector<CString> vstrDictGroups;
	CMenu mnuDictGroups;
	mnuDictGroups.CreateMenu();
	UINT nID = ID_TB_DICTS_AVAILABLE;
	UINT nIDDefault = nID;


	m_pbtnDicts->CreateFromMenu(mnuDictGroups.Detach());

	m_wndToolBarDicts.RemoveAllButtons();
	AddDict(nIDDefault - ID_TB_DICTS_AVAILABLE + ID_TB_DICTS_OFFLINEALL, DICT_DEFAULT, DICTIMAGE_CUSTOM);
	m_setDictsInUse.insert(nIDDefault);
	SelectDict(0);
}

void CWordsWebFrame::AddDict( UINT nID, LPCTSTR pszDict, EDictImage nImageIndex )
{
	CWordsBaseFrame::AddDict(nID, pszDict, nImageIndex);

	auto pDictHtmlCtrl = make_shared<CDictHtmlCtrl>();
	pDictHtmlCtrl->Create(CRect(), m_pView, 1234 + m_pView->m_vpDictHtmlCtrls.size(), WS_CHILD);
	pDictHtmlCtrl->SetSilent(TRUE);
	CRect rc;
	m_pView->GetClientRect(rc);
	m_pView->m_vpDictHtmlCtrls.push_back(pDictHtmlCtrl);
	m_pView->SendMessage(WM_SIZE, 0, MAKELPARAM(rc.Width(), rc.Height()));

	pDictHtmlCtrl->m_strDictName = pszDict;
	if(nImageIndex >= DICTIMAGE_OFFLINE && nImageIndex < DICTIMAGE_OFFLINE + 9)
		nImageIndex = DICTIMAGE_OFFLINE;
	else if(nImageIndex >= DICTIMAGE_ONLINE && nImageIndex < DICTIMAGE_ONLINE + 9)
		nImageIndex = DICTIMAGE_ONLINE;
	else if(nImageIndex >= DICTIMAGE_LIVE && nImageIndex < DICTIMAGE_LIVE + 9)
		nImageIndex = DICTIMAGE_LIVE;
	pDictHtmlCtrl->m_nDictImage = nImageIndex;

	CString sql;
	sql.Format(_T("SELECT * FROM DICTALL WHERE LANGID=%d"), m_lbuSettings.nLangID);
	pDictHtmlCtrl->m_rsDict.Open(sql);
	pDictHtmlCtrl->FindDict(pszDict);

	UpdateHtml(pDictHtmlCtrl);
}

int CWordsWebFrame::RemoveDict( UINT nID )
{
	int i = CWordsBaseFrame::RemoveDict(nID);
	auto it = m_pView->m_vpDictHtmlCtrls.begin() + i;
	m_pView->m_vpDictHtmlCtrls.erase(it);
	return i;
}

void CWordsWebFrame::OnDictSelected()
{
	if(m_pDictHtmlCtrl != NULL)
		m_pDictHtmlCtrl->ShowWindow(SW_HIDE);
	m_pDictHtmlCtrl = m_pView->m_vpDictHtmlCtrls[m_nDictIndex].get();
	m_pDictHtmlCtrl->ShowWindow(SW_SHOW);
	SetFocus();
}

void CWordsWebFrame::OnMoveComplete()
{
	CWordsBaseFrame::OnMoveComplete();
	if(m_pDictHtmlCtrl != NULL)
		UpdateHtml();
}

void CWordsWebFrame::UpdateHtml(shared_ptr<CDictHtmlCtrl>& pDictHtmlCtrl)
{
	pDictHtmlCtrl->UpdateHtml(m_strWord, m_rsAutoCorrect);
}

void CWordsWebFrame::UpdateHtml()
{
	for(auto& v : m_pView->m_vpDictHtmlCtrls)
		UpdateHtml(v);
}

void CWordsWebFrame::OnTimer( UINT_PTR nIDEvent )
{
	if(m_pDictHtmlCtrl->m_nDictImage == DICTIMAGE_WEB || m_pDictHtmlCtrl->m_nDictImage == DICTIMAGE_ONLINE) return;
	CWordsBaseFrame::OnTimer(nIDEvent);
}

BOOL CWordsWebFrame::PreTranslateMessage(MSG* pMsg)
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
				//SelectNextDict(false, bCtl ? m_pmnuDicts->GetCount() : m_nDictOnlineOffset);
				return TRUE;
			case VK_RIGHT:
				//SelectNextDict(true, bCtl ? m_pmnuDicts->GetCount() : m_nDictOnlineOffset);
				return TRUE;
			case '0':
				//SelectDict(bCtl ? m_nDictConjugatorOffset : 0);
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
				//if(key < '1' + (m_nDictConjugatorOffset - m_nDictOfflineOffset))
				//	SelectDict(
				//		(bCtl ? m_nDictOnlineOffset : m_nDictOfflineOffset) +
				//		(key - '1')
				//	);
				return TRUE;
			}
		}
	}

	return CWordsBaseFrame::PreTranslateMessage(pMsg);
}

LRESULT CWordsWebFrame::OnHtmlViewDocComplete( WPARAM wParam, LPARAM lParam )
{
	CDictHtmlCtrl* pDictHtmlCtrl = (CDictHtmlCtrl*)wParam;
    pDictHtmlCtrl->DoWebAutomation(m_strWord);
	return 0;
}

void CWordsWebFrame::OnRefresh()
{
	m_pDictHtmlCtrl = NULL;
	m_pView->m_vpDictHtmlCtrls.clear();

	CWordsBaseFrame::OnRefresh();
}
