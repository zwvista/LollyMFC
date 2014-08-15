#include "StdAfx.h"
#include "resource.h"
#include "UGCtrl.h"
#include "UGRecordMarkerCellType.h"

CImageList CUGRecordMarkerCellType::s_imglist;

CUGRecordMarkerCellType::CUGRecordMarkerCellType()
{
	if(!s_imglist.GetSafeHandle())
		s_imglist.Create(IDB_RECORD_MARKERS, 15, 1, RGB(255, 255, 255));
}

void CUGRecordMarkerCellType::DrawText( CDC *dc,RECT *rect,int offset,int col,long row,CUGCell *cell, int selected,int current )
{
	COLORREF	backcolor = cell->GetBackColor();
	DrawBackground( dc, rect, backcolor, row, col, cell, (current != 0), (selected != 0) );

	int n;
	cell->GetNumber(&n);
	if(n > 0)
		s_imglist.Draw(dc, n - 1, CPoint(rect->left + 2, rect->top + 2), ILD_NORMAL);
}
