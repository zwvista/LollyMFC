
// AuxPicBooksFrame.cpp : implementation of the CAuxPicBooksFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxPicBooksFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxPicBooksFrame

IMPLEMENT_DYNCREATE(CAuxPicBooksFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxPicBooksFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxPicBooksFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxPicBooksFrame construction/destruction

CAuxPicBooksFrame::CAuxPicBooksFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXPICBOOKS;
}

CAuxPicBooksFrame::~CAuxPicBooksFrame()
{
}

CString CAuxPicBooksFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM PICBOOKS WHERE LANGID=%d"), m_lblSettings.nLangID);
	return sql;
}

SDataGridColumnInfo* CAuxPicBooksFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("BOOKNAME"), _T("BOOKNAME"), _T("BOOKNAME"), 1, 3, TRUE },
		{ _T("FILENAME"), _T("FILENAME"), _T("FILENAME"), 2, 3, TRUE },
		{ _T("NUMPAGES"), _T("NUMPAGES"), _T("NUMPAGES"), 100, 0, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxPicBooksFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Picture Books (%s)"), m_lblSettings.GetLangDesc());
	return str;
}

void CAuxPicBooksFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditAdd)
		m_rs.SetFieldValue(_T("LANGID"), m_lblSettings.nLangID);
}

void CAuxPicBooksFrame::OnDelete()
{
	CString strItem = m_rs.GetFieldValueAsString(_T("BOOKNAME"));
	CString strMsg;
	strMsg.Format(_T("The picbooks item \"%s\" is about to be DELETED. Are you sure?"), strItem);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
