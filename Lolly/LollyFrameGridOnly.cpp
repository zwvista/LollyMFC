
// BooksFrm.cpp : implementation of the CLollyFrameGridOnly class
//

#include "stdafx.h"
#include "Lolly.h"

#include "LollyFrameGridOnly.h"
#include "DataGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLollyFrameGridOnly

IMPLEMENT_DYNCREATE(CLollyFrameGridOnly, CLollyFrameGrid)

BEGIN_MESSAGE_MAP(CLollyFrameGridOnly, CLollyFrameGrid)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CLollyFrameGridOnly construction/destruction

CLollyFrameGridOnly::CLollyFrameGridOnly()
	: m_bDataGridMultiLine(false)
{
	// TODO: add member initialization code here
}

CLollyFrameGridOnly::~CLollyFrameGridOnly()
{
}

int CLollyFrameGridOnly::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CLollyFrameGrid::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CDataGridView));

	if(m_bDataGridMultiLine){
		m_wndGrid.SetDefRowHeight(84);
		CUGCell cell;
		cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
		m_wndGrid.SetGridDefault(&cell);
	}

	PostMessage(WM_LBUSETTINGS_CHANGED);

	return 0;
}

void CLollyFrameGridOnly::OnSetFocus(CWnd* pOldWnd) 
{
	m_wndGrid.SetFocus();
}
