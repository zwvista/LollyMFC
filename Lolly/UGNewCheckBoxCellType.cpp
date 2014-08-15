#include "StdAfx.h"
#include "UGCtrl.h"
#include "UGNewCheckBoxCellType.h"

void CUGNewCheckBoxCellType::DrawText( CDC *dc,RECT *rect,int offset,int col,long row,CUGCell *cell, int selected,int current )
{
	COLORREF	backcolor;
	//check the selected and current states
	if(selected || (current && m_ctrl->m_GI->m_currentCellMode&2))
		backcolor = cell->GetHBackColor();
	else
		backcolor = cell->GetBackColor();

	DrawBackground( dc, rect, backcolor, row, col, cell, (current != 0), (selected != 0) );

	HANDLE hTheme = ::OpenThemeData(NULL, L"BUTTON");
	int iPartId = BP_CHECKBOX;
	CString str = cell->GetText();
	BOOL checked = str == _T("1");
	int iStateId = row != m_ctrl->m_GI->m_hoverRow || col != m_ctrl->m_GI->m_hoverCol || 
		m_ctrl->m_GI->m_hoverState == RELEASED_OUTSIDE ? checked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL :
		m_ctrl->m_GI->m_hoverState == RELEASED_INSIDE ? checked ? CBS_CHECKEDHOT : CBS_UNCHECKEDHOT :
		m_ctrl->m_GI->m_hoverState == CELL_PRESSED_OUTSIDE ? checked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL :
		m_ctrl->m_GI->m_hoverState == CELL_PRESSED_INSIDE ? checked ? CBS_CHECKEDHOT : CBS_UNCHECKEDHOT :
		m_ctrl->m_GI->m_hoverState == BOX_PRESSED_OUTSIDE ? checked ? CBS_CHECKEDHOT : CBS_UNCHECKEDHOT :
		checked ? CBS_CHECKEDPRESSED : CBS_UNCHECKEDPRESSED;
	::DrawThemeBackground(hTheme, *dc, iPartId, iStateId, rect, NULL);
	::CloseThemeData(hTheme);
}

BOOL CUGNewCheckBoxCellType::OnMouseMove( int col,long row,POINT *point,UINT nFlags )
{
	if(!(nFlags & MK_LBUTTON) && m_ctrl->m_GI->m_hoverCol == -1 && m_ctrl->m_GI->m_hoverRow == -1){
		CUGCtrl::TrackMouseEvent(m_ctrl->m_CUGGrid->m_hWnd);
		return TRUE;
	}
	return FALSE;
}

BOOL CUGNewCheckBoxCellType::OnMouseHover( int col,long row,POINT *point,UINT nFlags )
{
	TRACE(_T("HOVER %d,%d\n"), col, row);
	RECT rect;
	m_ctrl->GetCellRect(col, row, &rect);
	BOOL bInside = InsideCheckBox(&rect, point);
	int nState =
		m_ctrl->m_GI->m_hoverState == RELEASED_OUTSIDE ||
		m_ctrl->m_GI->m_hoverState == RELEASED_INSIDE ? 
		bInside ?  RELEASED_INSIDE : RELEASED_OUTSIDE :
		m_ctrl->m_GI->m_hoverState == CELL_PRESSED_OUTSIDE ||
		m_ctrl->m_GI->m_hoverState == CELL_PRESSED_INSIDE ? 
		bInside ?  CELL_PRESSED_INSIDE : CELL_PRESSED_OUTSIDE :
		bInside ?  BOX_PRESSED_INSIDE : BOX_PRESSED_OUTSIDE;

	if(m_ctrl->m_GI->m_hoverState != nState){
		m_ctrl->m_GI->m_hoverState = nState;
		m_ctrl->RedrawCell(col, row);
	}
	return TRUE;
}

BOOL CUGNewCheckBoxCellType::OnMouseLeave( int col,long row )
{
	TRACE(_T("LEAVE %d,%d\n"), col, row);
	m_ctrl->m_GI->m_hoverState = RELEASED_OUTSIDE;
	m_ctrl->RedrawCell(col, row);
	return TRUE;
}

BOOL CUGNewCheckBoxCellType::OnLClicked( int col,long row,int updn,RECT *rect,POINT *point )
{
	TRACE(_T("%s %d,%d\n"), updn ? _T("LDOWN") : _T("LUP"), col, row);
	if(updn){ // DOWN
		if(InsideCheckBox(rect, point)){
			// must be RELEASED_INSIDE
			m_ctrl->m_GI->m_hoverState = BOX_PRESSED_INSIDE;
			m_ctrl->RedrawCell(col, row);
		}
		else{
			// must be RELEASED_OUTSIDE
			m_ctrl->m_GI->m_hoverState = CELL_PRESSED_OUTSIDE;
		}
	}
	else if(col == m_ctrl->m_GI->m_hoverCol && row == m_ctrl->m_GI->m_hoverRow){  //UP
		if(InsideCheckBox(rect, point)){
			if(m_ctrl->m_GI->m_hoverState == BOX_PRESSED_INSIDE){
				CString str = m_ctrl->QuickGetText(col, row);
				m_ctrl->QuickSetText(col, row, str == _T("1") ? _T("0") : _T("1"));
			}
			m_ctrl->m_GI->m_hoverState = RELEASED_INSIDE;
			m_ctrl->RedrawCell(col, row);
		}
		else{
			m_ctrl->m_GI->m_hoverState = RELEASED_OUTSIDE;
			m_ctrl->RedrawCell(col, row);
		}
	}
	return TRUE;
}

BOOL CUGNewCheckBoxCellType::InsideCheckBox( RECT *rect,POINT *point )
{
	HANDLE hTheme = ::OpenThemeData(NULL, L"BUTTON");
	int iPartId = BP_CHECKBOX;
	int iStateId = CBS_UNCHECKEDNORMAL;
	SIZE sz;
	::GetThemePartSize(hTheme, NULL, iPartId, iStateId, rect, TS_TRUE, &sz);
	::CloseThemeData(hTheme);
	CRect rBox = *rect;
	rBox.DeflateRect((rBox.Width() - sz.cx - 1) / 2, (rBox.Height() - sz.cy - 1) / 2);
	return ::PtInRect(&rBox, *point);
}

BOOL CUGNewCheckBoxCellType::OnDClicked( int col,long row,RECT *rect,POINT *point )
{
	OnLClicked(col, row, TRUE, rect, point);
	OnLClicked(col, row, FALSE, rect, point);
	return TRUE;
}