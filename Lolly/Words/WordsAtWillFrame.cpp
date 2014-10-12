
// WordsAtWillFrame.cpp : implementation of the CWordsAtWillFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsAtWillFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsAtWillFrame

IMPLEMENT_DYNCREATE(CWordsAtWillFrame, CWordsWebFrame)

BEGIN_MESSAGE_MAP(CWordsAtWillFrame, CWordsWebFrame)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND(ID_TB_DELETE, OnDelete)
END_MESSAGE_MAP()

// CWordsAtWillFrame construction/destruction

CWordsAtWillFrame::CWordsAtWillFrame()
{
	m_nToolBarID = IDR_TOOLBAR_WORDSATWILL;
	m_strReindexItemFieldName = _T("WORD");
}

CWordsAtWillFrame::~CWordsAtWillFrame()
{
}

CString CWordsAtWillFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT ORD, WORD FROM WORDSBOOK WHERE ID = -1"));
	return sql;
}

SDataGridColumnInfo* CWordsAtWillFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("ORD"), _T("ORD"), _T("ORD"), 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 1, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsAtWillFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{1, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsAtWillFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words At Will (%s)"), m_lbuSettings.GetLangDesc());
	return str;
}

void CWordsAtWillFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			if(m_rs.GetFieldValueAsInt(_T("ORD")) == 0)
				m_rs.SetFieldValue(_T("ORD"), m_wndGrid.GetNumberRows());
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

LRESULT CWordsAtWillFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CWordsWebFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID)
		CreateDictCtrls();
	return 0;
}

void CWordsAtWillFrame::LoadTables()
{
	CWordsWebFrame::LoadTables();
	m_rs.Disconnect();
}

void CWordsAtWillFrame::OnDelete()
{
	m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}