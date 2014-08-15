#pragma once
#include "ugceltyp.h"

class CUGRecordMarkerCellType : public CUGCellType
{
public:
	CUGRecordMarkerCellType();
	virtual void DrawText(CDC *dc,RECT *rect,int offset,int col,long row,CUGCell *cell, int selected,int current);
private:
	static CImageList s_imglist;
};
