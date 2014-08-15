// WordsWebView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "WordsWebView.h"


// CWordsWebView

IMPLEMENT_DYNCREATE(CWordsWebView, CView)

CWordsWebView::CWordsWebView()
{

}

CWordsWebView::~CWordsWebView()
{
}

BEGIN_MESSAGE_MAP(CWordsWebView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CWordsWebView::OnDraw( CDC* pDC )
{

}

BOOL CWordsWebView::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style |= WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// CWordsWebView message handlers

void CWordsWebView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize(nType, cx, cy);
	for(const auto& pDictHtmlCtrl : m_vpDictHtmlCtrls)
		pDictHtmlCtrl->MoveWindow(0, 0, cx, cy);
}

int CWordsWebView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CWordsWebView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CWordsWebView::OnSetFocus( CWnd* pOldWnd )
{
}