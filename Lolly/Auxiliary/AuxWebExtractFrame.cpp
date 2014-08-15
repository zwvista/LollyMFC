
// AuxWebExtractFrame.cpp : implementation of the CAuxWebExtractFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxWebExtractFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxWebExtractFrame

IMPLEMENT_DYNCREATE(CAuxWebExtractFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxWebExtractFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxWebExtractFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxWebExtractFrame construction/destruction

CAuxWebExtractFrame::CAuxWebExtractFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXWEBEXTRACT;
	m_bDataGridMultiLine = true;
}

CAuxWebExtractFrame::~CAuxWebExtractFrame()
{
}

CString CAuxWebExtractFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WEBEXTRACT"));
	return sql;
}

SDataGridColumnInfo* CAuxWebExtractFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("SITENAME"), _T("SITENAME"), _T("SITENAME"), 1, 5, TRUE },
		{ _T("TRANSFROM_WIN"), _T("TRANSFROM_WIN"), NULL, 2, 5, TRUE },
		{ _T("TRANSFROM_MAC"), _T("TRANSFROM_MAC"), NULL, 2, 5, TRUE },
		{ _T("WAIT"), _T("WAIT"), NULL, 50, 0, TRUE },
		{ _T("BODY"), _T("BODY"), NULL, 50, 0, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxWebExtractFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Web Extract"));
	return str;
}

void CAuxWebExtractFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditInProgress){
		CString strItem = m_rs.GetFieldValueAsString(_T("SITENAME"));
		CString strMsg;
		strMsg.Format(_T("The webextract item \"%s\" is about to be updated. Are you sure?"), strItem);
		if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2) == IDNO)
			*adStatus = adStatusCancel;
	}
}

void CAuxWebExtractFrame::OnDelete()
{
	CString strItem = m_rs.GetFieldValueAsString(_T("SITENAME"));
	CString strMsg;
	strMsg.Format(_T("The webextract item \"%s\" is about to be DELETED. Are you sure?"), strItem);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
