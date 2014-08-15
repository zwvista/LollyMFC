// BlogPostView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "BlogPostView.h"


// CBlogPostView

IMPLEMENT_DYNCREATE(CBlogPostView, CView)

CBlogPostView::CBlogPostView()
{

}

CBlogPostView::~CBlogPostView()
{
}

BEGIN_MESSAGE_MAP(CBlogPostView, CView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
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

	StartProcess(_T("C:\\Program Files (x86)\\XML Notepad 2007\\XmlNotepad.exe"), _T(""));//Start ms paint
	::SetParent(m_hwndChildApp, m_hWnd);//set parent of ms paint to our dialog.
	SetWindowLong(m_hwndChildApp, GWL_STYLE, WS_VISIBLE);//erase title of ms paint window.

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
}

void CBlogPostView::OpenFile()
{
	::PostMessage(m_hwndChildApp, WM_COMMAND, ID_FILE_OPEN, 0);
}
