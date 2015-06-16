// ExtractWebDictFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "ExtractWebDictFrame.h"


// CExtractWebDictFrame

IMPLEMENT_DYNCREATE(CExtractWebDictFrame, CLollyFrame)

CExtractWebDictFrame::CExtractWebDictFrame()
	: m_lbuSettings(theApp.m_lbuSettings)
	, m_nWordIndex(0)
	, m_bOverwrite(false)
	, m_rsWord(&theApp.m_db)
	, m_rsDict(&theApp.m_db)
	, m_rsAutoCorrect(&theApp.m_db)
	, m_pedtWord(NULL)
	, m_pedtLang(NULL)
	, m_pedtDict(NULL)
	, m_eDicStatus(DIC_NAVIGATING)
	, m_pDictHtmlCtrl(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_EXTRACTWEBDICT;
}

CExtractWebDictFrame::~CExtractWebDictFrame()
{
}

BEGIN_MESSAGE_MAP(CExtractWebDictFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_HTMLVIEW_DOCCOMPLETE, OnHtmlViewDocComplete)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CExtractWebDictFrame message handlers

int CExtractWebDictFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CLollyHtmlView));
	m_pView = DYNAMIC_DOWNCAST(CLollyHtmlView, GetDlgItem(AFX_IDW_PANE_FIRST));
	m_pView->SetBrowserOptions(
		DLCTL_SILENT |
		DLCTL_NO_SCRIPTS |
		DLCTL_NO_JAVA |
		DLCTL_NO_DLACTIVEXCTLS |
		DLCTL_NO_RUNACTIVEXCTLS |
		DLCTL_NO_CLIENTPULL);

	CString sql;
	sql.Format(_T("SELECT * FROM AUTOCORRECT WHERE LANGID=%d"), m_lbuSettings.nLangID);
	m_rsAutoCorrect.Open(sql);

	m_pedtLang->SetContents(m_lbuSettings.strLangName);
	return 0;
}

LRESULT CExtractWebDictFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	m_wndToolBar.ReplaceButton(ID_TB_EWD_WORDLBL, CMFCToolBarLabelButton(ID_TB_EWD_WORDLBL));
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_EWD_WORDLBL), _T("Word"), TRUE, FALSE);
	m_wndToolBar.ReplaceButton(ID_TB_EWD_WORD, CMFCToolBarEditBoxButton2(ID_TB_EWD_WORD, -1, ES_AUTOHSCROLL, 100));
	m_pedtWord = (CMFCToolBarEditBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_EWD_WORD));
	m_wndToolBar.ReplaceButton(ID_TB_EWD_LANGLBL, CMFCToolBarLabelButton(ID_TB_EWD_LANGLBL));
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_EWD_LANGLBL), _T("Language"), TRUE, FALSE);
	m_wndToolBar.ReplaceButton(ID_TB_EWD_LANG, CMFCToolBarEditBoxButton2(ID_TB_EWD_LANG, -1, ES_AUTOHSCROLL, 100));
	m_pedtLang = (CMFCToolBarEditBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_EWD_LANG));
	m_wndToolBar.ReplaceButton(ID_TB_EWD_DICTLBL, CMFCToolBarLabelButton(ID_TB_EWD_DICTLBL));
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_EWD_DICTLBL), _T("Dictionary"), TRUE, FALSE);
	m_wndToolBar.ReplaceButton(ID_TB_EWD_DICT, CMFCToolBarEditBoxButton2(ID_TB_EWD_DICT, -1, ES_AUTOHSCROLL, 150));
	m_pedtDict = (CMFCToolBarEditBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_EWD_DICT));
	return 0;
}

CString CExtractWebDictFrame::GetFrameText() const
{
	return _T("Extract Web Dictionary");
}

void CExtractWebDictFrame::OnSize(UINT nType, int cx, int cy)
{
	CLollyFrame::OnSize(nType, cx, cy);

	CRect rc;
	m_wndToolBar.GetWindowRect(rc);
	ScreenToClient(rc);
	int ht = rc.Height();
	m_pView->MoveWindow(0, ht, cx, cy - ht);
	m_wndToolBar.CWnd::MoveWindow(0, 0, cx, ht);
}

void CExtractWebDictFrame::Init(const vector<CString>& vstrWords, LPCTSTR pszDict, bool bOverwrite, CDictHtmlCtrl* pDictHtmlCtrl /* = nullptr */, LPCTSTR pszIfrId /* = nullptr */)
{
	m_vstrWords = vstrWords;
	m_bOverwrite = bOverwrite;
	m_pDictHtmlCtrl = pDictHtmlCtrl;
    m_strIfrId = pszIfrId;

	m_pedtDict->SetContents(pszDict);
	m_wndToolBar.AdjustSizeImmediate();

	CString sql;
	sql.Format(_T("SELECT * FROM DICTALL WHERE LANGID=%d AND DICTNAME='%s'"),
		m_lbuSettings.nLangID, pszDict);
	m_rsDict.Open(sql);

	GetNextWord();
	while(!NeedSearchDict())
		if(!GetNextSearch())
			return;
	SearchDictForWord();
}

void CExtractWebDictFrame::GetNextWord()
{
	m_strWord = m_vstrWords[m_nWordIndex];
	theApp.OpenDictTable(m_strWord, m_rsWord, m_rsDict);
}

bool CExtractWebDictFrame::NeedSearchDict()
{
	return m_bOverwrite || m_rsWord.GetFieldValueAsString(_T("TRANSLATION")).IsEmpty();
}

bool CExtractWebDictFrame::GetNextSearch()
{
	if(++m_nWordIndex == m_vstrWords.size()){
		PostMessage(WM_CLOSE);
		return false;
	}
	GetNextWord();
	return true;
}

void CExtractWebDictFrame::SearchDictForWord()
{
	CString str;
	str.Format(_T("Word: %d / %d"), m_nWordIndex + 1, m_vstrWords.size());
	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_EWD_WORDLBL), str);
	m_pedtWord->SetContents(m_strWord);
	m_wndToolBar.AdjustSizeImmediate();

	m_eDicStatus = DIC_NAVIGATING;
	if(m_rsDict.GetFieldValueAsString(_T("DICTNAME") == _T("Frhelper")))
		m_pView->SetHTML(theApp.m_objFrhelper.Search(m_strWord));
	else{
		CString strURL = theApp.GetDictURLForWord(m_strWord, m_rsDict, m_rsAutoCorrect);
		m_pView->Navigate2(strURL);
	}
}

void CExtractWebDictFrame::ExtractWebDict()
{
	if(m_rsDict.GetFieldValueAsBool(_T("WAIT")))
		::Sleep(1000);
	if(m_pDictHtmlCtrl == NULL)
		theApp.UpdateDictTable(m_pView, m_rsWord, m_rsDict, false);
	else
		m_pDictHtmlCtrl->UpdateLiveHtml(m_strIfrId, m_strWord, m_rsDict.GetFieldValueAsString(_T("DICTNAME")),
			theApp.ExtractFromWeb(m_pView, m_rsDict, theApp.m_strNoTrans));
}

LRESULT CExtractWebDictFrame::OnHtmlViewDocComplete( WPARAM, LPARAM )
{
	if(m_eDicStatus != DIC_READY){
		CString strAutomation = m_rsDict.GetFieldValueAsString(_T("AUTOMATION"));
		m_eDicStatus = m_eDicStatus == DIC_NAVIGATING &&
			!strAutomation.IsEmpty() &&
			theApp.DoWebAutomation(m_pView, strAutomation, m_strWord) &&
			!m_rsDict.GetFieldValueAsBool(_T("AUTOJUMP")) ?
			DIC_AUTOMATING : DIC_READY;
		if(m_eDicStatus == DIC_READY)
			SetTimer(100, m_rsDict.GetFieldValueAsInt(_T("WAIT")), NULL);
	}
	return 0;
}

void CExtractWebDictFrame::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(100);
	ExtractWebDict();
	do 
		if(!GetNextSearch())
			return;
	while (!NeedSearchDict());
	SearchDictForWord();
}
