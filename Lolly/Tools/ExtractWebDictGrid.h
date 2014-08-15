#pragma once

#include "DataGrid.h"
#include "UGNewCheckBoxCellType.h"

class CExtractWebDictGrid : public CDataGrid
{
public:
	virtual void BindData(CADORecordset2* pRS, LPCTSTR pszSQL, SDataGridColumnInfo* ci);
	virtual void OnSetup();
	virtual void OnSetCell(int col,long row,CUGCell *cell) {}
	virtual void OnRowChange(long oldrow,long newrow) {}

protected:
	CUGNewCheckBoxCellType m_checkBoxType;

	DECLARE_MESSAGE_MAP()
};
