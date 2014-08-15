// PicBookView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "PicBookView.h"


// CPicBookView

IMPLEMENT_DYNCREATE(CPicBookView, CView)

CPicBookView::CPicBookView()
	: m_rsAutoCorrect(&theApp.m_db)
	, m_hwndNetUI(NULL)
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

	StartProcess(_T("C:\\Windows\\System32\\mspaint.exe"), _T(""));//Start ms paint
	::SetParent(m_hwndChildApp, m_hWnd);//set parent of ms paint to our dialog.
	SetWindowLong(m_hwndChildApp, GWL_STYLE, WS_VISIBLE);//erase title of ms paint window.

	HWND hwnd = ::FindWindowEx(m_hwndChildApp, NULL, _T("UIRibbonCommandBarDock"), _T("UIRibbonDockTop"));
	hwnd = ::GetDlgItem(hwnd, 0);
	hwnd = ::GetDlgItem(hwnd, 0);
	hwnd = ::GetDlgItem(hwnd, 0);
	m_hwndNetUI = ::GetDlgItem(hwnd, 0);

	m_hwndView = ::FindWindowEx(m_hwndChildApp, NULL, _T("MSPaintView"), NULL);

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

void CPicBookView::ClickUI( HWND hwnd, int x, int y )
{
	LPARAM lParam = MAKELPARAM(x, y);
	::PostMessage(hwnd, WM_MOUSEMOVE, 0, lParam);
	::PostMessage(hwnd, WM_LBUTTONDOWN, 1, lParam);
	::PostMessage(hwnd, WM_LBUTTONUP, 0, lParam);
}

void CPicBookView::ToggleRibbonPanel()
{
	ClickUI(m_hwndNetUI, 83, 43);
	ClickUI(m_hwndNetUI, 83, 43);
}

void CPicBookView::SelectLine( bool bLine1 )
{
	ToggleRibbonPanel();
	ClickUI(m_hwndNetUI, bLine1 ? 837 : 932, 72);
	ClickUI(m_hwndNetUI, 405, 72);
	ClickUI(m_hwndNetUI, 645, 72);
	CPoint p(0, 0);
	::ClientToScreen(m_hwndNetUI, &p);
	HWND hwnd = ::WindowFromPoint(CPoint(p.x + 645, p.y + 150));
	ClickUI(hwnd, 10, 100);
	ToggleRibbonPanel();
}

void CPicBookView::BeginWrite()
{
	ToggleRibbonPanel();
	ClickUI(m_hwndNetUI, 969, 72);
	ClickUI(m_hwndNetUI, 316, 83);
	ToggleRibbonPanel();
}

void CPicBookView::AutoCorrectWriting()
{
	HWND hwnd = ::GetDlgItem(m_hwndView, 0);
	HWND hwnd2 = ::GetDlgItem(hwnd, 0x73);
	if(hwnd2 == NULL) return;
	HWND hwndEdit = ::GetDlgItem(hwnd2, 0x72);
	CString str;
	::SendMessage(hwndEdit, WM_GETTEXT, (WPARAM)1024, (LPARAM)(LPCTSTR)str.GetBufferSetLength(1024));
	str.ReleaseBuffer();
	str = theApp.AutoCorrect(str, m_rsAutoCorrect);
	::SendMessage(hwndEdit, WM_SETTEXT, NULL, (LPARAM)(LPCTSTR)str);
	ClickUI(hwnd, 10, 10);
}

void CPicBookView::OpenFile()
{
	::PostMessage(m_hwndChildApp, WM_COMMAND, ID_FILE_OPEN, 0);
}
