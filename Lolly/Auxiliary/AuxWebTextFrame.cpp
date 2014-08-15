
// AuxWebTextFrame.cpp : implementation of the CAuxWebTextFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxWebTextFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxWebTextFrame

IMPLEMENT_DYNCREATE(CAuxWebTextFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxWebTextFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxWebTextFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxWebTextFrame construction/destruction

CAuxWebTextFrame::CAuxWebTextFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXWEBTEXT;
	m_bDataGridMultiLine = true;
}

CAuxWebTextFrame::~CAuxWebTextFrame()
{
}

CString CAuxWebTextFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WEBTEXT"));
	return sql;
}

SDataGridColumnInfo* CAuxWebTextFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("SITENAME"), _T("SITENAME"), _T("SITENAME"), 1, 7, TRUE },
		{ _T("URL"), _T("URL"), _T("URL"), 2, 7, TRUE },
		{ _T("TEMPLATE"), _T("TEMPLATE"), NULL, 2, 7, TRUE },
		{ _T("FOLDER"), _T("FOLDER"), _T("FOLDER"), 2, 7, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxWebTextFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Web Text"));
	return str;
}

void CAuxWebTextFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditInProgress){
		CString strItem = m_rs.GetFieldValueAsString(_T("SITENAME"));
		CString strMsg;
		strMsg.Format(_T("The webtext item \"%s\" is about to be updated. Are you sure?"), strItem);
		if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2) == IDNO)
			*adStatus = adStatusCancel;
	}
}

void CAuxWebTextFrame::OnDelete()
{
	CString strItem = m_rs.GetFieldValueAsString(_T("SITENAME"));
	CString strMsg;
	strMsg.Format(_T("The webtext item \"%s\" is about to be DELETED. Are you sure?"), strItem);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
