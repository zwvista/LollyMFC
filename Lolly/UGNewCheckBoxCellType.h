#pragma once
#include "ugceltyp.h"

class CUGNewCheckBoxCellType : public CUGCellType
{
public:
	virtual void DrawText(CDC *dc,RECT *rect,int offset,int col,long row,CUGCell *cell, int selected,int current);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT nFlags);
	virtual BOOL OnMouseHover(int col,long row,POINT *point,UINT nFlags);
	virtual BOOL OnMouseLeave(int col,long row);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
private:
	enum{
		RELEASED_OUTSIDE,
		RELEASED_INSIDE,
		CELL_PRESSED_OUTSIDE,
		CELL_PRESSED_INSIDE,
		BOX_PRESSED_OUTSIDE,
		BOX_PRESSED_INSIDE,
	};
	BOOL InsideCheckBox(RECT *rect,POINT *point);
};
