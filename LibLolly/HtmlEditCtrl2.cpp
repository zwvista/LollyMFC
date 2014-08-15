// HtmlEditCtrl2.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlEditCtrl2.h"
#include "mshtmdid.h"

// CHtmlEditCtrl2

IMPLEMENT_DYNCREATE(CHtmlEditCtrl2, CHtmlEditView)

CHtmlEditCtrl2::CHtmlEditCtrl2()
{

}

CHtmlEditCtrl2::~CHtmlEditCtrl2()
{
}

BEGIN_MESSAGE_MAP(CHtmlEditCtrl2, CHtmlEditView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CHtmlEditCtrl2 message handlers

void CHtmlEditCtrl2::OnDestroy()
{
	CWnd::OnDestroy();
}

int CHtmlEditCtrl2::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
