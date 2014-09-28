
// BooksFrm.cpp : implementation of the CLollyFrameGrid class
//

#include "stdafx.h"
#include "Lolly.h"

#include "LollyFrameGrid.h"
#include "FilterDlg.h"
#include "ReindexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLollyFrameGrid

IMPLEMENT_DYNCREATE(CLollyFrameGrid, CLollyFrame)

BEGIN_MESSAGE_MAP(CLollyFrameGrid, CLollyFrame)
	ON_MESSAGE(WM_LBLSETTINGS_CHANGED, OnLblSettingsChanged)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_WM_TIMER()
	ON_COMMAND(ID_TB_MOVEFIRST, OnMoveFirst)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVEFIRST, OnUpdateMoveFirst)
	ON_COMMAND(ID_TB_MOVEPREVIOUS, OnMovePrevious)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVEPREVIOUS, OnUpdateMovePrevious)
	ON_COMMAND(ID_TB_RECORDPOS, OnRecordPos)
	ON_COMMAND(ID_TB_MOVENEXT, OnMoveNext)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVENEXT, OnUpdateMoveNext)
	ON_COMMAND(ID_TB_MOVELAST, OnMoveLast)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVELAST, OnUpdateMoveLast)
	ON_COMMAND(ID_TB_ADDNEW, OnAddNew)
	ON_UPDATE_COMMAND_UI(ID_TB_ADDNEW, OnUpdateAddNew)
	ON_UPDATE_COMMAND_UI(ID_TB_MULTIADD, OnUpdateMultiAdd)
	ON_UPDATE_COMMAND_UI(ID_TB_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_TB_REFRESH, OnRefresh)
	ON_COMMAND(ID_TB_TIMER_NAVIGATE, OnTimerNavigate)
	ON_UPDATE_COMMAND_UI(ID_TB_TIMER_NAVIGATE, OnUpdateTimerNavigate)
	ON_COMMAND_RANGE(ID_TB_NAVIGATE_FORWARD, ID_TB_NAVIGATE_ATRANDOM, OnNavigate)
	ON_COMMAND(ID_TB_FILTER_SET, OnFilterSet)
	ON_UPDATE_COMMAND_UI(ID_TB_FILTER_SET, OnUpdateFilter)
	ON_COMMAND(ID_TB_FILTER_REMOVE, OnFilterRemove)
	ON_UPDATE_COMMAND_UI(ID_TB_FILTER_REMOVE, OnUpdateFilter)
	ON_COMMAND(ID_TB_REINDEX, OnReindex)
	ON_UPDATE_COMMAND_UI(ID_TB_REINDEX, OnUpdateReindex)
END_MESSAGE_MAP()

// CLollyFrameGrid construction/destruction

CLollyFrameGrid::CLollyFrameGrid()
	: m_rs(&theApp.m_db, this)
	, m_rsAutoCorrect(&theApp.m_db)
	, m_nFilterScope(0)
	, m_pedtRecordPos(NULL)
	, m_nRecordPos(0)
	, m_nRecordCount(0)
	, m_bHasFilter(false)
	, m_bHasMultiAdd(false)
	, m_bHasReindex(false)
	, m_bTimerNavigate(false)
	, m_bIsReindexing(false)
	, m_pbtnNavigate(NULL)
{
	// TODO: add member initialization code here
}

CLollyFrameGrid::~CLollyFrameGrid()
{
}


// CLollyFrameGrid message handlers

void CLollyFrameGrid::LoadTables()
{
	m_wndGrid.SetInitialSortCol(0);
	m_wndGrid.BindData(&m_rs, GetSQL(), GetDataGridColumnInfo());
	CString sql;
	sql.Format(_T("SELECT * FROM AUTOCORRECT WHERE LANGID=%d"), m_lbuSettings.nLangID);
	m_rsAutoCorrect.Open(sql);
}

LRESULT CLollyFrameGrid::OnLblSettingsChanged(WPARAM wParam, LPARAM lParam)
{
	m_lbuSettings = theApp.m_lbuSettings;
	SendMessage(WM_COMMAND, ID_TB_REFRESH);
	SetWindowText(GetFrameText());
	return 0;
}

LRESULT CLollyFrameGrid::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	if(wParam == m_nToolBarID){
		m_wndToolBar.ReplaceButton(ID_TB_RECORDPOS, CMFCToolBarEditBoxButton2(ID_TB_RECORDPOS, 0, ES_AUTOHSCROLL, 50));
		m_pedtRecordPos = (CMFCToolBarEditBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_RECORDPOS));
		m_wndToolBar.ReplaceButton(ID_TB_RECORDCOUNT, CMFCToolBarLabelButton(ID_TB_RECORDCOUNT));
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_RECORDCOUNT), _T(""), TRUE, FALSE);

		m_bHasMultiAdd = m_wndToolBar.CommandToIndex(ID_TB_MULTIADD) != -1;
		m_bHasReindex = m_wndToolBar.CommandToIndex(ID_TB_REINDEX) != -1;

		int nIndex = m_wndToolBar.CommandToIndex(ID_TB_NAVIGATE_FORWARD);
		if(nIndex != -1){
			CMenu menu;
			menu.LoadMenu(IDR_POPUP_NAVIGATE);
			m_wndToolBar.ReplaceButton(ID_TB_NAVIGATE_FORWARD, CMFCToolBarMenuButton(ID_TB_NAVIGATE_FORWARD, menu.Detach(), -1));
			m_pbtnNavigate = m_wndToolBar.GetButton((m_wndToolBar.CommandToIndex(ID_TB_NAVIGATE_FORWARD)));
			m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_TB_NAVIGATE_BACKWARD));
			m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_TB_NAVIGATE_ATRANDOM));
		}
	}
	return 0;
}

void CLollyFrameGrid::KeepGridRowWithRS()
{
	//m_wndGrid.SetFocus();
	m_bIsReindexing = true;
	m_wndGrid.GotoRow(m_rs.GetAbsolutePosition() - 1);
	m_bIsReindexing = false;
}

void CLollyFrameGrid::OnMoveFirst()
{
	m_rs.MoveFirst();
	KeepGridRowWithRS();
}

void CLollyFrameGrid::OnUpdateMoveFirst( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && m_rs.GetAbsolutePosition() != 1);
}

void CLollyFrameGrid::OnMovePrevious()
{
	m_rs.MovePrevious();
	KeepGridRowWithRS();
}

void CLollyFrameGrid::OnUpdateMovePrevious( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && m_rs.GetAbsolutePosition() != 1);
}

void CLollyFrameGrid::OnMoveNext()
{
	m_rs.MoveNext();
	KeepGridRowWithRS();
}

void CLollyFrameGrid::OnUpdateMoveNext( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && !m_rs.IsEof() && m_rs.GetAbsolutePosition() != m_rs.GetRecordCount());
}

void CLollyFrameGrid::OnMoveLast()
{
	m_rs.MoveLast();
	KeepGridRowWithRS();
}

void CLollyFrameGrid::OnUpdateMoveLast( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && m_rs.GetAbsolutePosition() != m_rs.GetRecordCount());
}

void CLollyFrameGrid::OnAddNew()
{
	m_wndGrid.SetFocus();
	m_wndGrid.GotoRow(m_wndGrid.GetNumberRows() - 1);
}

void CLollyFrameGrid::OnUpdateAddNew( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && !m_rs.IsEof());
}

void CLollyFrameGrid::OnUpdateDelete( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_rs.IsOpen() && !m_rs.IsEof());
}

void CLollyFrameGrid::OnRefresh()
{
	DoUpdateFilter();
	LoadTables();
}

void CLollyFrameGrid::MoveComplete( EventReasonEnum adReason, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(!m_wndGrid.IsInitializing() && !m_bIsReindexing && adReason != adRsnRequery)
		OnMoveComplete();
}

void CLollyFrameGrid::OnMoveComplete()
{
	m_nRecordCount = m_rs.GetRecordCount();
	if(m_nRecordPos == -3)
		m_nRecordPos = ++m_nRecordCount;

	CString str;
	str.Format(_T(" / %d"), m_nRecordCount);
	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_RECORDCOUNT), str);

	str.Format(_T("%d"), m_nRecordPos);
	m_pedtRecordPos->SetContents(str);

	m_wndToolBar.AdjustSizeImmediate();
}

void CLollyFrameGrid::OnRecordPos()
{
	CString str;
	m_pedtRecordPos->GetEditBox()->GetWindowText(str);
	int n;
	if(_stscanf(str, _T("%d"), &n) == 1 && n >= 1 && n <= m_rs.GetRecordCount()){
		if(m_wndGrid.GetCurrentRow() != n - 1)
			m_wndGrid.GotoRow(n - 1);
		m_wndGrid.SetFocus();
	}
	else{
		str.Format(_T("%d"), m_rs.GetAbsolutePosition());
		m_pedtRecordPos->SetContents(str);
	}
}

void CLollyFrameGrid::CreateFilterCtrls()
{
	m_wndToolBar.ReplaceButton(ID_TB_FILTER, CMFCToolBarLabelButton(ID_TB_FILTER));
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_FILTER), _T(""), TRUE, FALSE);
	m_bHasFilter = true;
}

void CLollyFrameGrid::DoAddNew( SDataGridNewRecordInfo* nri )
{
	OnAddNew();
	for(SDataGridNewRecordInfo* p = nri; p->lpszText != NULL; ++p)
		m_wndGrid.QuickSetText(p->nCol, m_wndGrid.GetNumberRows() - 1, p->lpszText);
	m_wndGrid.OnRowChange(0, 0);
	OnAddNew();
}

void CLollyFrameGrid::OnFilterSet()
{
	CFilterDlg dlg(m_rsAutoCorrect, this);
	if(dlg.DoModal() == IDOK){
		m_strFilter = dlg.m_strFilter;
		m_nFilterScope = dlg.m_nScope;
		SendMessage(WM_COMMAND, ID_TB_REFRESH);
	}
}

void CLollyFrameGrid::OnFilterRemove()
{
	m_strFilter.Empty();
	SendMessage(WM_COMMAND, ID_TB_REFRESH);
}

void CLollyFrameGrid::DoUpdateFilter()
{
	if(!m_bHasFilter) return;
	CString str;
	if(m_strFilter.IsEmpty())
		str = _T("No Filter");
	else
		str.Format(_T("Filter: \"%s\""), m_strFilter);
	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_FILTER), str);
}

void CLollyFrameGrid::OnUpdateFilter( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_bHasFilter);
}

void CLollyFrameGrid::OnUpdateMultiAdd( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_bHasMultiAdd);
}

void CLollyFrameGrid::SetTimerNavigate()
{
	if(m_bTimerNavigate)
		SetTimer(100, 3000, NULL);
	else
		KillTimer(100);
}

void CLollyFrameGrid::OnTimerNavigate()
{
	m_bTimerNavigate = !m_bTimerNavigate;
	SetTimerNavigate();
}

void CLollyFrameGrid::OnUpdateTimerNavigate( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(m_bTimerNavigate);
}

void CLollyFrameGrid::OnTimer( UINT_PTR nIDEvent )
{
	DoNavigate();
}

void CLollyFrameGrid::OnNavigate( UINT nID )
{
	m_pbtnNavigate->m_nID = nID;
	m_pbtnNavigate->ResetImageToDefault();
	DoNavigate();
}

void CLollyFrameGrid::DoNavigate()
{
	int nIndex = 
		m_pbtnNavigate->m_nID == ID_TB_NAVIGATE_FORWARD ? 
			m_nRecordPos % m_nRecordCount :
		m_pbtnNavigate->m_nID == ID_TB_NAVIGATE_BACKWARD ? 
			(m_nRecordPos - 2 + m_nRecordCount) % m_nRecordCount :
		rand() % m_nRecordCount;
	m_rs.Move(nIndex, CADORecordset2::bmFirst);
	KeepGridRowWithRS();
}

void CLollyFrameGrid::OnReindex()
{
	m_bIsReindexing = true;
	CReindexDlg dlg(m_rs, m_strReindexItemFieldName, this);
	if(dlg.DoModal() == IDOK){
		m_bIsReindexing = false;
		m_wndGrid.Rebind();
	}
	else{
		m_rs.MoveFirst();
		m_rs.Move(m_wndGrid.GetCurrentRow());
		m_bIsReindexing = false;
	}
}

void CLollyFrameGrid::OnUpdateReindex( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_bHasReindex);
}


BOOL CLollyFrameGrid::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == m_edtGridItem.m_hWnd && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		CString strGridItem;
		m_edtGridItem.GetWindowText(strGridItem);
		strGridItem.Trim();
		if(!strGridItem.IsEmpty())
			OnGridItemEntered(strGridItem);
		m_edtGridItem.SetWindowText(_T(""));
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}
