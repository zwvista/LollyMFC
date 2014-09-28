
// BooksFrm.cpp : implementation of the CWordsUnitsEBFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "WordsUnitsEBFrame.h"
#include "DataGridView.h"
#include "WordsEBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordsUnitsEBFrame

IMPLEMENT_DYNCREATE(CWordsUnitsEBFrame, CWordsEBFrame)

BEGIN_MESSAGE_MAP(CWordsUnitsEBFrame, CWordsEBFrame)
	ON_COMMAND(ID_TB_DELETE, &CWordsUnitsEBFrame::OnDelete)
END_MESSAGE_MAP()

// CWordsUnitsEBFrame construction/destruction

CWordsUnitsEBFrame::CWordsUnitsEBFrame()
{
	m_strReindexItemFieldName = _T("WORD");
}

CWordsUnitsEBFrame::~CWordsUnitsEBFrame()
{
}

void CWordsUnitsEBFrame::OnDelete()
{
	CString strMsg;
	strMsg.Format(_T("The word \"%s\" is about to be DELETED. Are you sure?"), m_strWord);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}

CString CWordsUnitsEBFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM WORDSBOOK WHERE BOOKID=%d AND UNIT*10+PART>=%d AND UNIT*10+PART<=%d"), 
		m_lbuSettings.nBookID, m_lbuSettings.GetUnitPartFrom(), m_lbuSettings.GetUnitPartTo());
	return sql;
}

SDataGridColumnInfo* CWordsUnitsEBFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("UNIT"), _T("UNIT"), _T("UNIT, PART, [INDEX]"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, TRUE },
		{ _T("WORD"), _T("WORD"), _T("WORD"), 1, 2, TRUE },
		{ _T("NOTE"), _T("NOTE"), _T("NOTE"), 1, 2, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CWordsUnitsEBFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {-1, NULL}
	};
	return nri;
}

CString CWordsUnitsEBFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Words(EBWin) (%s)"), m_lbuSettings.GetBookUnitsDesc());
	return str;
}

void CWordsUnitsEBFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate && m_rs.GetEditMode() == adEditAdd){
		m_rs.SetFieldValue(_T("BOOKID"), m_lbuSettings.nBookID);
		if(m_rs.GetFieldValueAsInt(_T("UNIT")) == 0)
			m_rs.SetFieldValue(_T("UNIT"), m_lbuSettings.nUnitTo);
		if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
			m_rs.SetFieldValue(_T("PART"), m_lbuSettings.nPartTo);
		if(m_rs.GetFieldValueAsInt(_T("INDEX")) == 0)
			m_rs.SetFieldValue(_T("INDEX"), m_wndGrid.GetNumberRows());
	}
}
