#include "StdAfx.h"
#include "Lolly.h"
#include "ExtractWebDictGrid.h"

BEGIN_MESSAGE_MAP(CExtractWebDictGrid,CUGCtrl)
END_MESSAGE_MAP()

void CExtractWebDictGrid::OnSetup()
{
	CDataGrid::OnSetup();
	SetMultiSelectMode(TRUE);
	SetHighlightRow(TRUE);
	SetSH_Width(0);
}

void CExtractWebDictGrid::BindData( CADORecordset2* pRS, LPCTSTR pszSQL, SDataGridColumnInfo* ci )
{
	pRS->Open(pszSQL);
	pRS->Disconnect();
	if(pRS->GetRecordCount() > 0)
		for(pRS->MoveFirst(); !pRS->IsEof(); pRS->MoveNext()){
			pRS->Edit();
			pRS->SetFieldValue(0, "0");
			pRS->Update();
		}

	CDataGrid::BindData(pRS, pszSQL, ci);

	CUGCell cell;
	cell.SetCellType(AddCellType(&m_checkBoxType));
	SetColDefault(0, &cell);
}