
// BooksFrm.cpp : implementation of the CWordsLessonsEBFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsLessonsEBFrame.h"
#include "DataGridView.h"
#include "WordsEBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordsLessonsEBFrame

IMPLEMENT_DYNCREATE(CWordsLessonsEBFrame, CWordsEBFrame)

BEGIN_MESSAGE_MAP(CWordsLessonsEBFrame, CWordsEBFrame)
	ON_COMMAND(ID_TB_DELETE, &CWordsLessonsEBFrame::OnDelete)
END_MESSAGE_MAP()

// CWordsLessonsEBFrame construction/destruction

CWordsLessonsEBFrame::CWordsLessonsEBFrame()
{
	m_strReindexItemFieldName = _T("WORD");
}

CWordsLessonsEBFrame::~CWordsLessonsEBFrame()
{
}

void CWordsLessonsEBFrame::OnDelete()
{
	CString strMsg;
	strMsg.Format(_T("The word \"%s\" is about to be DELETED. Are you sure?"), m_strWord);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}

CString CWordsLessonsEBFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WORDSBOOK WHERE BOOKID=%d AND LESSON*10+PART>=%d AND LESSON*10+PART<=%d"), 
		m_lblSettings.nBookID, m_lblSettings.GetLessonPartFrom(), m_lblSettings.GetLessonPartTo());
	return sql;
}

SDataGridColumnInfo* CWordsLessonsEBFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("LESSON"), _T("LESSON"), _T("LESSON, PART, [INDEX]"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 2, TRUE },
		{ _T("NOTE"), _T("NOTE"), _T("NOTE"), 1, 2, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsLessonsEBFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsLessonsEBFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words(EBWin) (%s)"), m_lblSettings.GetBookLessonsDesc());
	return str;
}

void CWordsLessonsEBFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditAdd){
		m_rs.SetFieldValue(_T("BOOKID"), m_lblSettings.nBookID);
		if(m_rs.GetFieldValueAsInt(_T("LESSON")) == 0)
			m_rs.SetFieldValue(_T("LESSON"), m_lblSettings.nLessonTo);
		if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
			m_rs.SetFieldValue(_T("PART"), m_lblSettings.nPartTo);
		if(m_rs.GetFieldValueAsInt(_T("INDEX")) == 0)
			m_rs.SetFieldValue(_T("INDEX"), m_wndGrid.GetNumberRows());
	}
}
