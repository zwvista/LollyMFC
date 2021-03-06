
// AuxAutoCorrectFrame.cpp : implementation of the CAuxAutoCorrectFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxAutoCorrectFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxAutoCorrectFrame

IMPLEMENT_DYNCREATE(CAuxAutoCorrectFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxAutoCorrectFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxAutoCorrectFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxAutoCorrectFrame construction/destruction

CAuxAutoCorrectFrame::CAuxAutoCorrectFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXAUTOCORRECT;
	m_strReorderItemFieldName = _T("EXTENDED");
}

CAuxAutoCorrectFrame::~CAuxAutoCorrectFrame()
{
}

CString CAuxAutoCorrectFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM AUTOCORRECT WHERE LANGID=%d"), m_lbuSettings.nLangID);
	return sql;
}

SDataGridColumnInfo* CAuxAutoCorrectFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("ORD"), _T("ORD"), _T("ORD"), 100, 0, TRUE },
		{ _T("INPUT"), _T("INPUT"), _T("INPUT"), 100, 0, TRUE },
		{ _T("EXTENDED"), _T("EXTENDED"), _T("EXTENDED"), 100, 0, TRUE },
		{ _T("BASIC"), _T("BASIC"), _T("BASIC"), 100, 0, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxAutoCorrectFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("AutoCorrect (%s)"), m_lbuSettings.GetLangDesc());
	return str;
}

void CAuxAutoCorrectFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditAdd){
		m_rs.SetFieldValue(_T("LANGID"), m_lbuSettings.nLangID);
		if(m_rs.GetFieldValueAsInt(_T("ORD")) == 0)
			m_rs.SetFieldValue(_T("ORD"), m_wndGrid.GetNumberRows());
	}
}

void CAuxAutoCorrectFrame::OnDelete()
{
	CString strItem = m_rs.GetFieldValueAsString(_T("INPUT"));
	CString strMsg;
	strMsg.Format(_T("The autocorrect item \"%s\" is about to be DELETED. Are you sure?"), strItem);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
