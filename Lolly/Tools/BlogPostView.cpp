// BlogPostView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "BlogPostView.h"


// CBlogPostView

IMPLEMENT_DYNCREATE(CBlogPostView, CView)

CBlogPostView::CBlogPostView()
: CStartProcess(_T("XML Notepad"))
{

}

CBlogPostView::~CBlogPostView()
{
}

BEGIN_MESSAGE_MAP(CBlogPostView, CView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_TB_BP_B, OnB)
	ON_COMMAND(ID_TB_BP_I, OnI)
END_MESSAGE_MAP()

// CBlogPostView drawing

void CBlogPostView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CBlogPostView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	StartProcess(theApp.m_strXmlNotepadExe, _T(""));//Start ms paint
	::SetParent(m_hwndChildApp, m_hWnd);//set parent of ms paint to our dialog.
	SetWindowLong(m_hwndChildApp, GWL_STYLE, WS_VISIBLE);//erase title of ms paint window.

	m_edtSrc.Create(WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN, CRect(), this, 1);
	m_edtSrc.SetFont(&theApp.m_defFont);
}

void CBlogPostView::OnDestroy()
{
	::SendMessage(m_hwndChildApp, WM_CLOSE, 0, 0);

	CView::OnDestroy();
}

BOOL CBlogPostView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CBlogPostView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	::MoveWindow(m_hwndChildApp, 0, 0, cx, cy, true);
	if(m_edtSrc.GetSafeHwnd())
		m_edtSrc.MoveWindow(0, 0, cx, cy);
}

void CBlogPostView::NewNote(const CString& strNote)
{
	CopyTextToClipBoard(strNote);
	::SetForegroundWindow(m_hwndChildApp);
	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
	keybd_event('V', MapVirtualKey('V', 0), 0, 0);
	keybd_event('V', MapVirtualKey('V', 0), KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
}

void CBlogPostView::ToggleView()
{
	if(m_edtSrc.IsWindowVisible()){
		m_edtSrc.ShowWindow(SW_HIDE);
		::ShowWindow(m_hwndChildApp, SW_SHOW);
		if(m_strFileName.IsEmpty()) return;
		CStdioFileEx f(m_strFileName, CFile::modeWrite | CFile::shareDenyNone | CFile::modeCreate);
		f.SetCodePage(65001);
		CString strText;
		m_edtSrc.GetWindowText(strText);
		f.WriteString(strText);
	}
	else{
		m_edtSrc.ShowWindow(SW_SHOW);
		::ShowWindow(m_hwndChildApp, SW_HIDE);
		CWnd::FromHandle(m_hwndChildApp)->GetWindowText(m_strFileName);
		m_strFileName.TrimLeft(_T("XML-Notepad - "));
		m_strFileName.TrimRight(_T("*"));
		if(m_strFileName.IsEmpty()) return;
		CStdioFileEx f(m_strFileName, CFile::modeRead);
		f.SetCodePage(65001);
		CString strLine, strText;
		while(f.ReadString(strLine))
			strText += strLine + _T("\r\n");
		m_edtSrc.SetWindowText(strText);
	}
}

void CBlogPostView::OnBI(LPCTSTR pszFormat)
{
	int nStart, nEnd;
	m_edtSrc.GetSel(nStart, nEnd);
	CString str;
	m_edtSrc.GetWindowText(str);
	CString strOld = str.Mid(nStart, nEnd - nStart);
	strOld = regex_replace(wstring(strOld), wregex(_T("<.+?>")), wstring()).c_str();
	CString strNew;
	strNew.Format(pszFormat, strOld);
	m_edtSrc.ReplaceSel(strNew);
}
