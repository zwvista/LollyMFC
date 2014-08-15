// AutoSelEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AutoSelEdit.h"


// CAutoSelEdit

IMPLEMENT_DYNAMIC(CAutoSelEdit, CEdit)

CAutoSelEdit::CAutoSelEdit()
: m_bSelectAll(false)
{

}

CAutoSelEdit::~CAutoSelEdit()
{
}


BEGIN_MESSAGE_MAP(CAutoSelEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CAutoSelEdit message handlers



void CAutoSelEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	m_bSelectAll = true;
}

void CAutoSelEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown(nFlags, point);
}

void CAutoSelEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonUp(nFlags, point);
	if(m_bSelectAll){
		m_bSelectAll = false;
		SetSel(0, -1);
	}
}
