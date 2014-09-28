// HtmlUnitFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "WebBrowserFrame.h"


// CWebBrowserFrame

IMPLEMENT_DYNCREATE(CWebBrowserFrame, CLollyFrame)

CWebBrowserFrame::CWebBrowserFrame()
	: m_rsWebText(&theApp.m_db)
	, m_pView(NULL)
	, m_pcboAddress(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_WEBBROWSER;
}

CWebBrowserFrame::~CWebBrowserFrame()
{
}


BEGIN_MESSAGE_MAP(CWebBrowserFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TB_OPEN, OnOpen)
	ON_COMMAND(ID_TB_WB_GO, OnGo)
	ON_COMMAND(ID_TB_SAVE, OnSave)
	ON_COMMAND(ID_TB_WB_ADDRESS, OnGo)
	ON_UPDATE_COMMAND_UI(ID_TB_SAVE, OnUpdateSave)
	ON_MESSAGE(WM_HTMLVIEW_DOCCOMPLETE, OnHtmlViewDocComplete)
	ON_MESSAGE(WM_HTMLVIEW_NEWWINDOW3, OnHtmlViewNewWindow3)
	ON_COMMAND(ID_TB_SPEAK, OnSpeak)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()


// CWebBrowserFrame message handlers


int CWebBrowserFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CLollyHtmlView));
	m_pView = DYNAMIC_DOWNCAST(CLollyHtmlView, GetDlgItem(AFX_IDW_PANE_FIRST));
	return 0;
}

CString CWebBrowserFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Web Browser"));
	return str;
}

void CWebBrowserFrame::OnGo()
{
	CString strAddress = m_pcboAddress->GetText();
	m_pView->Navigate2(strAddress);
}

void CWebBrowserFrame::OnOpen()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() != IDOK) return;
	m_pcboAddress->SetText(dlg.GetPathName());
	OnGo();
}

void CWebBrowserFrame::OnSave()
{
	CString strText = theApp.ExtractFromWeb(m_pView, m_rsWebText); 

	CString strTitle = GetDocumentTitle(m_pView);

	CString strHtml = m_rsWebText.GetFieldValueAsString(_T("TEMPLATE"));
	strHtml.Replace(_T("{0}"), strTitle);
	strHtml.Replace(_T("{2}"), strText);

	CFileDialog dlg(FALSE, _T("html"), strTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HTML Files(*.htm)|*.htm||"));
	CString strFolder = m_rsWebText.GetFieldValueAsString(_T("FOLDER"));
	dlg.m_ofn.lpstrInitialDir = strFolder;
	if(dlg.DoModal() == IDOK){
		CStdioFileEx f(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CStdioFileEx::modeWriteUnicode);
		f.WriteString(strHtml);
	}
}

void CWebBrowserFrame::OnUpdateSave( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rsWebText.IsOpen() && m_rsWebText.GetRecordCount() > 0);
}

LRESULT CWebBrowserFrame::OnHtmlViewDocComplete( WPARAM, LPARAM lparam)
{
	CString strURL = m_pView->GetLocationURL();
	m_pcboAddress->SetText(strURL);

	CString sql;
	sql.Format(_T("SELECT * FROM WEBTEXTALL WHERE InStr('%s', URL) <> 0"), strURL);
	m_rsWebText.Open(sql);
	return 0;
}

LRESULT CWebBrowserFrame::OnHtmlViewNewWindow3( WPARAM, LPARAM lParam )
{
	SNewWindow3Params* p = (SNewWindow3Params*)lParam;
	CWebBrowserFrame* pNewFrame = (CWebBrowserFrame*)theApp.OnNewChild(RUNTIME_CLASS(CWebBrowserFrame));
	pNewFrame->m_pView->SetRegisterAsBrowser(TRUE);
	*p->ppDisp = pNewFrame->m_pView->GetApplication();
	pNewFrame->Navigate2(CString(p->bstrUrl));
	return 0;
}

void CWebBrowserFrame::Navigate2( LPCTSTR pszURL )
{
	m_pcboAddress->SetText(pszURL);
	m_pView->Navigate2(pszURL);
}

LRESULT CWebBrowserFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	m_wndToolBar.ReplaceButton(ID_TB_WB_ADDRESS, CMFCToolBarComboBoxButton2(ID_TB_WB_ADDRESS, 0, CBS_DROPDOWN, 600));
	m_pcboAddress = (CMFCToolBarComboBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_WB_ADDRESS));
	return 0;
}

void CWebBrowserFrame::OnSpeak()
{
	m_pView->SendMessage(WM_COMMAND, ID_EDIT_COPY);
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_TTS_SPEAK);
}