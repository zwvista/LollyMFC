// LollyFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "LollyFrame.h"


// CLollyFrame

IMPLEMENT_DYNCREATE(CLollyFrame, CMDIChildWndEx)

CLollyFrame::CLollyFrame()
	: m_nToolBarID(0)
{

}

CLollyFrame::~CLollyFrame()
{
}


BEGIN_MESSAGE_MAP(CLollyFrame, CMDIChildWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


BOOL CLollyFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CLollyFrame message handlers


int CLollyFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(m_nToolBarID))
		return -1;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	m_dockManager.DockPane(&m_wndToolBar);

	return 0;
}

void CLollyFrame::CreateView( CRuntimeClass* pNewViewClass )
{
	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	CMDIChildWndEx::CreateView(&context, AFX_IDW_PANE_FIRST);
	RecalcLayout();
}
