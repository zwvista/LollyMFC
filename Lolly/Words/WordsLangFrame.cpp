
// WordsLangFrame.cpp : implementation of the CWordsLangFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsLangFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsLangFrame

IMPLEMENT_DYNCREATE(CWordsLangFrame, CWordsWebFrame)

BEGIN_MESSAGE_MAP(CWordsLangFrame, CWordsWebFrame)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CWordsLangFrame construction/destruction

CWordsLangFrame::CWordsLangFrame()
{
	m_nToolBarID = IDR_TOOLBAR_WORDSLANG;
}

CWordsLangFrame::~CWordsLangFrame()
{
}

CString CWordsLangFrame::GetSQL()
{
	const CString strUnion(_T(" Union "));
	CString sql;
	switch(m_nFilterScope){
	case 0:
		sql.Format(_T("SELECT LANGID, WORD FROM WORDSLANG ")
			_T("WHERE (LANGID = %d) AND (WORD LIKE '%%%s%%')"),
			m_lbuSettings.nLangID, m_strFilter);
		break;
	case 1:
		for(const CString& strDictTable : m_vstrOfflineDictTables){
			CString str;
			str.Format(
				_T("SELECT LANGID, WORDSLANG.WORD ")
				_T("FROM WORDSLANG INNER JOIN [%s] ON WORDSLANG.WORD = [%s].WORD ")
				_T("WHERE LANGID = %d AND [TRANSLATION] LIKE '%%%s%%'%s"),
				strDictTable, strDictTable, m_lbuSettings.nLangID, m_strFilter, strUnion);
			sql += str;
		}
		sql = sql.Left(sql.GetLength() - strUnion.GetLength());
		break;
	}
	return sql;
}

SDataGridColumnInfo* CWordsLangFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 1, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CWordsLangFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words (%s)"), m_lbuSettings.GetLangDesc());
	return str;
}

void CWordsLangFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			m_rs.SetFieldValue(_T("LANGID"), m_lbuSettings.nLangID);
			m_rs.SetFieldValue(_T("WORD"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("WORD")), m_rsAutoCorrect));
			break;
		case adEditInProgress:
			m_rs.SetFieldValue(_T("WORD"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("WORD")), m_rsAutoCorrect));
			break;
	}
}

LRESULT CWordsLangFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsWebFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID){
		CreateDictCtrls();
		CreateFilterCtrls();
	}
	return 0;
}

SDataGridNewRecordInfo* CWordsLangFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{0, NULL}, {-1, NULL}
	};
	return nri;
}
