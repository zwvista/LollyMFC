
// WordsAtWillEBFrame.cpp : implementation of the CWordsAtWillEBFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsAtWillEBFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsAtWillEBFrame

IMPLEMENT_DYNCREATE(CWordsAtWillEBFrame, CWordsEBFrame)

BEGIN_MESSAGE_MAP(CWordsAtWillEBFrame, CWordsEBFrame)
	ON_COMMAND(ID_TB_DELETE, OnDelete)
END_MESSAGE_MAP()

// CWordsAtWillEBFrame construction/destruction

CWordsAtWillEBFrame::CWordsAtWillEBFrame()
{
	m_strReindexItemFieldName = _T("WORD");
}

CWordsAtWillEBFrame::~CWordsAtWillEBFrame()
{
}

CString CWordsAtWillEBFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT [INDEX], WORD, NOTE FROM WORDSBOOK WHERE ID = -1"));
	return sql;
}

SDataGridColumnInfo* CWordsAtWillEBFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("INDEX"), _T("INDEX"), _T("[INDEX]"), 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 2, TRUE },
		{ _T("NOTE"), _T("NOTE"), _T("NOTE"), 1, 2, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsAtWillEBFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{1, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsAtWillEBFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words At Will(EBWin) (%s)"), m_lblSettings.GetLangDesc());
	return str;
}

void CWordsAtWillEBFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			if(m_rs.GetFieldValueAsInt(_T("INDEX")) == 0)
				m_rs.SetFieldValue(_T("INDEX"), m_wndGrid.GetNumberRows());
			break;
		}
}

void CWordsAtWillEBFrame::LoadTables()
{
	CWordsEBFrame::LoadTables();
	m_rs.Disconnect();
}

void CWordsAtWillEBFrame::OnDelete()
{
	m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}