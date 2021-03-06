// DataGridView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "DataGridView.h"
#include "LollyFrameGrid.h"


// CDataGridView

IMPLEMENT_DYNCREATE(CDataGridView, CFormView)

CDataGridView::CDataGridView()
	: CFormView(CDataGridView::IDD)
	, m_pedtGridItem(nullptr)
	, m_pDataGrid(nullptr)
{

}

CDataGridView::~CDataGridView()
{
}

BEGIN_MESSAGE_MAP(CDataGridView, CFormView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
    //ON_WM_SIZE()
END_MESSAGE_MAP()


void CDataGridView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEW_WORD, *m_pedtGridItem);
}

BOOL CDataGridView::PreCreateWindow(CREATESTRUCT& cs)
{
    if(!CFormView::PreCreateWindow(cs))
        return FALSE;

    cs.style |= WS_CLIPCHILDREN;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

	return TRUE;
}

void CDataGridView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    LockWindowUpdate();

    //GetParentFrame()->RecalcLayout();
    //ResizeParentToFit();

	m_pDataGrid->AttachGrid(this, IDC_GRID_WORD);

    m_resizer.Hook(this);
    m_resizer.SetDock(IDC_EDIT_NEW_WORD, DOCK_TOP);
    m_resizer.SetDock(IDC_GRID_WORD, DOCK_FILL);

	AFXGetParentFrame(this)->SendMessage(WM_LBUSETTINGS_CHANGED);

    CRect r;
    GetClientRect(r);
    SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(r.Width(), r.Height()));

    UnlockWindowUpdate();
}

// CDataGridView message handlers

int CDataGridView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CLollyFrameGrid* pFrame = DYNAMIC_DOWNCAST(CLollyFrameGrid, AFXGetParentFrame(this));
	m_pedtGridItem = &pFrame->m_edtGridItem;
	m_pDataGrid = &pFrame->m_wndGrid;
	return 0;
}

void CDataGridView::OnSetFocus( CWnd* pOldWnd )
{
	m_pDataGrid->SetFocus();
}

void CDataGridView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if(!m_pDataGrid->GetSafeHwnd()) return;

    CRect r;
    m_pedtGridItem->GetWindowRect(r);
    m_pedtGridItem->MoveWindow(0, 0, cx, r.Height());
    m_pDataGrid->MoveWindow(0, r.Height(), cx, cy - r.Height() - 1);
}
