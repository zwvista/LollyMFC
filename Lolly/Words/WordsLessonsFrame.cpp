
// WordsLessonsFrame.cpp : implementation of the CWordsLessonsFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsLessonsFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsLessonsFrame

IMPLEMENT_DYNCREATE(CWordsLessonsFrame, CWordsWebFrame)

BEGIN_MESSAGE_MAP(CWordsLessonsFrame, CWordsWebFrame)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CWordsLessonsFrame construction/destruction

CWordsLessonsFrame::CWordsLessonsFrame()
{
	m_nToolBarID = IDR_TOOLBAR_WORDSLESSONS;
	m_strReindexItemFieldName = _T("WORD");
}

CWordsLessonsFrame::~CWordsLessonsFrame()
{
}

CString CWordsLessonsFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WORDSBOOK WHERE BOOKID=%d AND LESSON*10+PART>=%d AND LESSON*10+PART<=%d"), 
		m_lblSettings.nBookID, m_lblSettings.GetLessonPartFrom(), m_lblSettings.GetLessonPartTo());
	return sql;
}

SDataGridColumnInfo* CWordsLessonsFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("LESSON"), _T("LESSON"), _T("LESSON, PART, [INDEX]"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 1, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsLessonsFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsLessonsFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words (%s)"), m_lblSettings.GetBookLessonsDesc());
	return str;
}

void CWordsLessonsFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate){
		m_eEditMode = m_rs.GetEditMode();
		switch(m_eEditMode){
		case adEditAdd:
			m_rs.SetFieldValue(_T("BOOKID"), m_lblSettings.nBookID);
			if(m_rs.GetFieldValueAsInt(_T("LESSON")) == 0)
				m_rs.SetFieldValue(_T("LESSON"), m_lblSettings.nLessonTo);
			if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
				m_rs.SetFieldValue(_T("PART"), m_lblSettings.nPartTo);
			if(m_rs.GetFieldValueAsInt(_T("INDEX")) == 0)
				m_rs.SetFieldValue(_T("INDEX"), m_wndGrid.GetNumberRows());
			m_strWord = theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("WORD")), m_rsAutoCorrect);
			m_rs.SetFieldValue(_T("WORD"), m_strWord);
			break;
		case adEditInProgress:
			m_rs.SetFieldValue(_T("WORD"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("WORD")), m_rsAutoCorrect));
			break;
		}
	}
}

void CWordsLessonsFrame::RecordChangeComplete( EventReasonEnum adReason, LONG cRecords, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_eEditMode){
		case adEditAdd:
			InsertWordIfNeeded();
			break;
		case adEditDelete:
			DeleteWordIfNeeded();
			break;
		case adEditInProgress:
			// Check if we are returning from the CReIndexDlg
			if(!m_strWord.IsEmpty()){
				CString strWord = m_rs.GetFieldValueAsString(_T("WORD"));
				if(m_strWord != strWord){
					DeleteWordIfNeeded();
					m_strWord = strWord;
					InsertWordIfNeeded();
				}
			}
			break;
		}
}

void CWordsLessonsFrame::InsertWordIfNeeded()
{
	CADORecordset2 rs(&theApp.m_db);
	CString sql;
	sql.Format(_T("SELECT COUNT(*) FROM WORDSLANG WHERE LANGID=%d AND WORD=N'%s'"),
		m_lblSettings.nLangID, DoubleApostrophe(m_strWord));
	rs.Open(sql);
	if(rs.GetFieldValueAsInt(0) == 0){
		sql.Format(_T("INSERT INTO WORDSLANG (LANGID, WORD) VALUES(%d, '%s')"),
			m_lblSettings.nLangID, DoubleApostrophe(m_strWord));
		theApp.m_db.Execute(sql);
	}
}

void CWordsLessonsFrame::DeleteWordIfNeeded()
{
	CADORecordset2 rs(&theApp.m_db);
	CString sql;
	sql.Format(_T("SELECT COUNT(*) FROM (BOOKS INNER JOIN WORDSBOOK ON BOOKS.BOOKID ")
		_T(" = WORDSBOOK.BOOKID) WHERE (BOOKS.LANGID=%d) AND (WORDSBOOK.WORD=N'%s')"),
		m_lblSettings.nLangID, DoubleApostrophe(m_strWord));
	rs.Open(sql);
	if(rs.GetFieldValueAsInt(0) == 0){
		CString strMsg;
		strMsg.Format(_T("The word \"%s\" is about to be DELETED from the language \"%s\". Are you sure?"),
			m_strWord, m_lblSettings.GetLangDesc());
		if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES){
			sql.Format(_T("DELETE FROM WORDSLANG WHERE LANGID=%d AND WORD=N'%s'"),
				m_lblSettings.nLangID, DoubleApostrophe(m_strWord));
			theApp.m_db.Execute(sql);
		}
	}
}

LRESULT CWordsLessonsFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsWebFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID)
		CreateDictCtrls();
	return 0;
}
