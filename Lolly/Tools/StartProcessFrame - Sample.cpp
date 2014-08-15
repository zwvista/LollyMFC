// BlogPostFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "BlogPostFrame.h"


// CBlogPostFrame

IMPLEMENT_DYNCREATE(CBlogPostFrame, CLollyFrame)

CBlogPostFrame::CBlogPostFrame()
	: m_pView(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_BlogPost;
}

CBlogPostFrame::~CBlogPostFrame()
{
}


BEGIN_MESSAGE_MAP(CBlogPostFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CBlogPostFrame message handlers

int CBlogPostFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CBlogPostView));
	m_pView = DYNAMIC_DOWNCAST(CBlogPostView, GetDlgItem(AFX_IDW_PANE_FIRST));
	return 0;
}

LRESULT CBlogPostFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	if(wParam == m_nToolBarID){
	}
	return 0;
}

void CBlogPostFrame::OnDestroy()
{
	CLollyFrame::OnDestroy();
}

CString CBlogPostFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Blog Post"));
	return str;
}
