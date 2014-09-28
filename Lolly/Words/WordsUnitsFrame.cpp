
// WordsUnitsFrame.cpp : implementation of the CWordsUnitsFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsUnitsFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsUnitsFrame

IMPLEMENT_DYNCREATE(CWordsUnitsFrame, CWordsWebFrame)

BEGIN_MESSAGE_MAP(CWordsUnitsFrame, CWordsWebFrame)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CWordsUnitsFrame construction/destruction

CWordsUnitsFrame::CWordsUnitsFrame()
{
	m_nToolBarID = IDR_TOOLBAR_WORDSUNITS;
	m_strReindexItemFieldName = _T("WORD");
}

CWordsUnitsFrame::~CWordsUnitsFrame()
{
}

CString CWordsUnitsFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WORDSBOOK WHERE BOOKID=%d AND UNIT*10+PART>=%d AND UNIT*10+PART<=%d"), 
		m_lbuSettings.nBookID, m_lbuSettings.GetUnitPartFrom(), m_lbuSettings.GetUnitPartTo());
	return sql;
}

SDataGridColumnInfo* CWordsUnitsFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("UNIT"), _T("UNIT"), _T("UNIT, PART, [INDEX]"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 1, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsUnitsFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsUnitsFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words (%s)"), m_lbuSettings.GetBookUnitsDesc());
	return str;
}

void CWordsUnitsFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate){
		m_eEditMode = m_rs.GetEditMode();
		switch(m_eEditMode){
		case adEditAdd:
			m_rs.SetFieldValue(_T("BOOKID"), m_lbuSettings.nBookID);
			if(m_rs.GetFieldValueAsInt(_T("UNIT")) == 0)
				m_rs.SetFieldValue(_T("UNIT"), m_lbuSettings.nUnitTo);
			if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
				m_rs.SetFieldValue(_T("PART"), m_lbuSettings.nPartTo);
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

void CWordsUnitsFrame::RecordChangeComplete( EventReasonEnum adReason, LONG cRecords, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
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

void CWordsUnitsFrame::InsertWordIfNeeded()
{
	CADORecordset2 rs(&theApp.m_db);
	CString sql;
	sql.Format(_T("SELECT COUNT(*) FROM WORDSLANG WHERE LANGID=%d AND WORD=N'%s'"),
		m_lbuSettings.nLangID, DoubleApostrophe(m_strWord));
	rs.Open(sql);
	if(rs.GetFieldValueAsInt(0) == 0){
		sql.Format(_T("INSERT INTO WORDSLANG (LANGID, WORD) VALUES(%d, '%s')"),
			m_lbuSettings.nLangID, DoubleApostrophe(m_strWord));
		theApp.m_db.Execute(sql);
	}
}

void CWordsUnitsFrame::DeleteWordIfNeeded()
{
	CADORecordset2 rs(&theApp.m_db);
	CString sql;
	sql.Format(_T("SELECT COUNT(*) FROM (BOOKS INNER JOIN WORDSBOOK ON BOOKS.BOOKID ")
		_T(" = WORDSBOOK.BOOKID) WHERE (BOOKS.LANGID=%d) AND (WORDSBOOK.WORD=N'%s')"),
		m_lbuSettings.nLangID, DoubleApostrophe(m_strWord));
	rs.Open(sql);
	if(rs.GetFieldValueAsInt(0) == 0){
		CString strMsg;
		strMsg.Format(_T("The word \"%s\" is about to be DELETED from the language \"%s\". Are you sure?"),
			m_strWord, m_lbuSettings.GetLangDesc());
		if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES){
			sql.Format(_T("DELETE FROM WORDSLANG WHERE LANGID=%d AND WORD=N'%s'"),
				m_lbuSettings.nLangID, DoubleApostrophe(m_strWord));
			theApp.m_db.Execute(sql);
		}
	}
}

LRESULT CWordsUnitsFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsWebFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID)
		CreateDictCtrls();
	return 0;
}
