// PicBookView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "PicBookView.h"


// CPicBookView

IMPLEMENT_DYNCREATE(CPicBookView, CView)

CPicBookView::CPicBookView()
	: m_rsAutoCorrect(&theApp.m_db)
	, m_hwndBarTool(NULL)
	, m_hwndBarColor(NULL)
	, m_hwndView(NULL)
{

}

CPicBookView::~CPicBookView()
{
}

BEGIN_MESSAGE_MAP(CPicBookView, CView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPicBookView drawing

void CPicBookView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CPicBookView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	StartProcess(_T("C:\\Windows\\System32\\mspaint.exe"), _T(""), 1000);//Start ms paint
	::SetParent(m_hwndChildApp, m_hWnd);//set parent of ms paint to our dialog.
	SetWindowLong(m_hwndChildApp, GWL_STYLE, WS_VISIBLE);//eraze title of ms paint window.

	HWND hwndCtrlBarTool = ::FindWindowEx(m_hwndChildApp, NULL, _T("AfxControlBar42u"), _T("工具"));
	m_hwndBarTool = ::FindWindowEx(hwndCtrlBarTool, NULL, _T("AfxWnd42u"), _T("工具"));

	HWND hwndCtrlColor = ::FindWindowEx(m_hwndChildApp, NULL, _T("AfxControlBar42u"), _T("颜色"));
	m_hwndBarColor = ::FindWindowEx(hwndCtrlColor, NULL, _T("AfxWnd42u"), _T("颜色"));

	m_hwndView = ::FindWindowEx(m_hwndChildApp, NULL, _T("AfxFrameOrView42u"), _T(""));

	CString sql;
	sql.Format(_T("SELECT * FROM AUTOCORRECT WHERE LANGID=%d"), theApp.m_lblSettings.nLangID);
	m_rsAutoCorrect.Open(sql);
}

void CPicBookView::OnDestroy()
{
	::SendMessage(m_hwndChildApp, WM_CLOSE, 0, 0);

	CView::OnDestroy();
}

BOOL CPicBookView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPicBookView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	::MoveWindow(m_hwndChildApp, 0, 0, cx, cy, true);
}

void CPicBookView::SelectLine( bool bLine1 )
{
	LPARAM lParam = MAKELPARAM( bLine1 ? 106 : 202, 21);
	::SendMessage(m_hwndBarColor, WM_LBUTTONDOWN, 0, lParam);
	::SendMessage(m_hwndBarColor, WM_LBUTTONUP, 0, lParam);
	::SendMessage(m_hwndChildApp, WM_COMMAND, 624, 0);
	lParam = MAKELPARAM(31, 263);
	::SendMessage(m_hwndBarTool, WM_LBUTTONDOWN, 0, lParam);
	::SendMessage(m_hwndBarTool, WM_LBUTTONUP, 0, lParam);
}

void CPicBookView::BeginWrite()
{
	LPARAM lParam = MAKELPARAM(250, 21);
	::SendMessage(m_hwndBarColor, WM_LBUTTONDOWN, 0, lParam);
	::SendMessage(m_hwndBarColor, WM_LBUTTONUP, 0, lParam);
	::SendMessage(m_hwndChildApp, WM_COMMAND, 622, 0);
}

void CPicBookView::AutoCorrectWriting()
{
	HWND hwnd = ::GetDlgItem(m_hwndView, 0);
	HWND hwnd2 = ::FindWindowEx(hwnd, NULL, _T("AfxWnd42u"), NULL);
	if(hwnd2 == NULL) return;
	HWND hwndEdit = ::FindWindowEx(hwnd2, NULL, _T("Edit"), NULL);
	CString str;
	::SendMessage(hwndEdit, WM_GETTEXT, (WPARAM)1024, (LPARAM)(LPCTSTR)str.GetBufferSetLength(1024));
	str.ReleaseBuffer();
	str = theApp.AutoCorrect(str, m_rsAutoCorrect);
	::SendMessage(hwndEdit, WM_SETTEXT, NULL, (LPARAM)(LPCTSTR)str);
	LPARAM lParam = MAKELPARAM(10, 10);
	::SendMessage(hwnd, WM_LBUTTONDOWN, 0, lParam);
	::SendMessage(hwnd, WM_LBUTTONUP, 0, lParam);
}

void CPicBookView::OpenFile()
{
	::PostMessage(m_hwndChildApp, WM_COMMAND, ID_FILE_OPEN, 0);
}
