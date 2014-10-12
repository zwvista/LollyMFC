
// WordsBooksFrame.cpp : implementation of the CWordsBooksFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsBooksFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsBooksFrame

IMPLEMENT_DYNCREATE(CWordsBooksFrame, CWordsWebFrame)

BEGIN_MESSAGE_MAP(CWordsBooksFrame, CWordsWebFrame)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CWordsBooksFrame construction/destruction

CWordsBooksFrame::CWordsBooksFrame()
{
	m_nToolBarID = IDR_TOOLBAR_WORDSBOOKS;
}

CWordsBooksFrame::~CWordsBooksFrame()
{
}

CString CWordsBooksFrame::GetSQL()
{
	const CString strUnion(_T(" Union "));
	CString sql;
	switch(m_nFilterScope){
	case 0:
		sql.Format(_T("SELECT WORDSBOOK.ID, WORDSBOOK.BOOKID, WORDSBOOK.UNIT, WORDSBOOK.PART, ")
			_T("WORDSBOOK.ORD, WORDSBOOK.WORD, BOOKS.BOOKNAME, WORDSBOOK.[NOTE]  ")
			_T("FROM (WORDSBOOK INNER JOIN BOOKS ON WORDSBOOK.BOOKID = BOOKS.BOOKID) ")
			_T("WHERE (BOOKS.LANGID = %d) AND WORDSBOOK.WORD LIKE '%%%s%%'"),
			m_lbuSettings.nLangID, m_strFilter);
		break;
	case 1:
		for(const CString& strDictTable : m_vstrOfflineDictTables){
			CString str;
			str.Format(
				_T("SELECT ID, WORDSBOOK.BOOKID, BOOKNAME, UNIT, PART, ORD, WORDSBOOK.WORD, NOTE ")
				_T("FROM BOOKS INNER JOIN (WORDSBOOK INNER JOIN [%s] ")
				_T("ON WORDSBOOK.WORD = [%s].WORD) ON BOOKS.BOOKID = WORDSBOOK.BOOKID ")
				_T("WHERE LANGID = %d AND [TRANSLATION] LIKE '%%%s%%'%s"),
				strDictTable, strDictTable, m_lbuSettings.nLangID, m_strFilter, strUnion);
			sql += str;
		}
		sql = sql.Left(sql.GetLength() - strUnion.GetLength());
		break;
	}
	return sql;
}

SDataGridColumnInfo* CWordsBooksFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("BOOKNAME"), _T("BOOKNAME"), _T("BOOKNAME, UNIT, PART, ORD"), 150, 0, FALSE },
		{ _T("UNIT"), _T("UNIT"), NULL, 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("ORD"), _T("ORD"), NULL, 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 1, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CWordsBooksFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words in Books (%s)"), m_lbuSettings.GetLangDesc());
	return str;
}

void CWordsBooksFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditInProgress)
		m_rs.SetFieldValue(_T("WORD"), theApp.AutoCorrect(
			m_rs.GetFieldValueAsString(_T("WORD")), m_rsAutoCorrect));
}

void CWordsBooksFrame::RecordChangeComplete( EventReasonEnum adReason, LONG cRecords, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{

}

int CWordsBooksFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_wndGrid.SetAllowAddNew(FALSE);
	m_wndGrid.SetAllowDelete(FALSE);

	if (CWordsWebFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

LRESULT CWordsBooksFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsWebFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID){
		CreateDictCtrls();
		CreateFilterCtrls();
	}
	return 0;
}

SDataGridNewRecordInfo* CWordsBooksFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{4, NULL}, {-1, NULL}
	};
	return nri;
}
