
// PhrasesLessonsFrame.cpp : implementation of the CPhrasesLessonsFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "PhrasesLessonsFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhrasesLessonsFrame

IMPLEMENT_DYNCREATE(CPhrasesLessonsFrame, CPhrasesBaseFrame)

BEGIN_MESSAGE_MAP(CPhrasesLessonsFrame, CPhrasesBaseFrame)
	ON_COMMAND(ID_TB_DELETE, &CPhrasesLessonsFrame::OnDelete)
END_MESSAGE_MAP()

// CPhrasesLessonsFrame construction/destruction

CPhrasesLessonsFrame::CPhrasesLessonsFrame()
{
	m_nToolBarID = IDR_TOOLBAR_PHRASESLESSONS;
	m_strReindexItemFieldName = _T("PHRASE");
}

CPhrasesLessonsFrame::~CPhrasesLessonsFrame()
{
}

CString CPhrasesLessonsFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM PHRASES WHERE BOOKID=%d AND LESSON*10+PART>=%d AND LESSON*10+PART <=%d"), 
		m_lblSettings.nBookID, m_lblSettings.GetLessonPartFrom(), m_lblSettings.GetLessonPartTo());
	return sql;
}

SDataGridColumnInfo* CPhrasesLessonsFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("LESSON"), _T("LESSON"), _T("LESSON, PART, [INDEX]"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, TRUE },
		{ _T("PHRASE"), _T("PHRASE"), _T("PHRASE"), 1, 2, TRUE },
		{ _T("TRANSLATION"), _T("TRANSLATION"), _T("[TRANSLATION]"), 1, 2, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CPhrasesLessonsFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {4, NULL}, {-1, NULL}
	};
	return nri;
}

CString CPhrasesLessonsFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Phrases (%s)"), m_lblSettings.GetBookLessonsDesc());
	return str;
}

void CPhrasesLessonsFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			m_rs.SetFieldValue(_T("BOOKID"), m_lblSettings.nBookID);
			if(m_rs.GetFieldValueAsInt(_T("LESSON")) == 0)
				m_rs.SetFieldValue(_T("LESSON"), m_lblSettings.nLessonTo);
			if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
				m_rs.SetFieldValue(_T("PART"), m_lblSettings.nPartTo);
			if(m_rs.GetFieldValueAsInt(_T("INDEX")) == 0)
				m_rs.SetFieldValue(_T("INDEX"), m_wndGrid.GetNumberRows());
			m_rs.SetFieldValue(_T("PHRASE"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("PHRASE")), m_rsAutoCorrect));
			break;
		case adEditInProgress:
			m_rs.SetFieldValue(_T("PHRASE"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("PHRASE")), m_rsAutoCorrect));
			break;
		}
}

void CPhrasesLessonsFrame::OnDelete()
{
	CString strPhrase = m_rs.GetFieldValueAsString(_T("PHRASE"));
	CString strMsg;
	strMsg.Format(_T("The phrase \"%s\" is about to be DELETED. Are you sure?"), strPhrase);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
