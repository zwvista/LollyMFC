
// AuxBooksFrame.cpp : implementation of the CAuxBooksFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxBooksFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxBooksFrame

IMPLEMENT_DYNCREATE(CAuxBooksFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxBooksFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxBooksFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxBooksFrame construction/destruction

CAuxBooksFrame::CAuxBooksFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXBOOKS;
}

CAuxBooksFrame::~CAuxBooksFrame()
{
}

CString CAuxBooksFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM BOOKS WHERE LANGID=%d"), m_lbuSettings.nLangID);
	return sql;
}

SDataGridColumnInfo* CAuxBooksFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("BOOKID"), _T("BOOKID"), _T("BOOKID"), 100, 0, TRUE },
		{ _T("BOOKNAME"), _T("BOOKNAME"), _T("BOOKNAME"), 300, 0, TRUE },
		{ _T("UNITSINBOOK"), _T("UNITSINBOOK"), _T("UNITSINBOOK"), 100, 0, TRUE },
		{ _T("PARTS"), _T("PARTS"), _T("PARTS"), 300, 0, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxBooksFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Books (%s)"), m_lbuSettings.GetLangDesc());
	return str;
}

void CAuxBooksFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditAdd)
		m_rs.SetFieldValue(_T("LANGID"), m_lbuSettings.nLangID);
}

void CAuxBooksFrame::OnDelete()
{
	CString strBookName = m_rs.GetFieldValueAsString(_T("BOOKNAME"));
	CString strMsg;
	strMsg.Format(_T("The book \"%s\" is about to be DELETED. Are you sure?"), strBookName);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
