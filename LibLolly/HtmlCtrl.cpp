// HtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlCtrl.h"

// CHtmlCtrl

IMPLEMENT_DYNCREATE(CHtmlCtrl, CHtmlView)

CHtmlCtrl::CHtmlCtrl()
{

}

CHtmlCtrl::~CHtmlCtrl()
{
}

void CHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CHtmlCtrl message handlers

void CHtmlCtrl::OnDestroy()
{
	CWnd::OnDestroy();
}

int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
