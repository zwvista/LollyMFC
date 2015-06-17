
// PhrasesUnitsFrame.cpp : implementation of the CPhrasesUnitsFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "PhrasesUnitsFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhrasesUnitsFrame

IMPLEMENT_DYNCREATE(CPhrasesUnitsFrame, CPhrasesBaseFrame)

BEGIN_MESSAGE_MAP(CPhrasesUnitsFrame, CPhrasesBaseFrame)
	ON_COMMAND(ID_TB_DELETE, &CPhrasesUnitsFrame::OnDelete)
END_MESSAGE_MAP()

// CPhrasesUnitsFrame construction/destruction

CPhrasesUnitsFrame::CPhrasesUnitsFrame()
{
	m_nToolBarID = IDR_TOOLBAR_PHRASESUNITS;
	m_strReorderItemFieldName = _T("PHRASE");
}

CPhrasesUnitsFrame::~CPhrasesUnitsFrame()
{
}

CString CPhrasesUnitsFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM PHRASES WHERE BOOKID=%d AND UNIT*10+PART>=%d AND UNIT*10+PART <=%d"), 
		m_lbuSettings.nBookID, m_lbuSettings.GetUnitPartFrom(), m_lbuSettings.GetUnitPartTo());
	return sql;
}

SDataGridColumnInfo* CPhrasesUnitsFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("UNIT"), _T("UNIT"), _T("UNIT, PART, ORD"), 75, 0, TRUE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, TRUE },
		{ _T("ORD"), _T("ORD"), NULL, 75, 0, TRUE },
		{ _T("PHRASE"), _T("PHRASE"), _T("PHRASE"), 1, 2, TRUE },
		{ _T("TRANSLATION"), _T("TRANSLATION"), _T("[TRANSLATION]"), 1, 2, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

SDataGridNewRecordInfo* CPhrasesUnitsFrame::GetDataGridNewRecordInfo()
{
	static SDataGridNewRecordInfo nri[] = {
		{3, NULL}, {4, NULL}, {-1, NULL}
	};
	return nri;
}

CString CPhrasesUnitsFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Phrases (%s)"), m_lbuSettings.GetBookUnitsDesc());
	return str;
}

void CPhrasesUnitsFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			m_rs.SetFieldValue(_T("BOOKID"), m_lbuSettings.nBookID);
			if(m_rs.GetFieldValueAsInt(_T("UNIT")) == 0)
				m_rs.SetFieldValue(_T("UNIT"), m_lbuSettings.nUnitTo);
			if(m_rs.GetFieldValueAsInt(_T("PART")) == 0)
				m_rs.SetFieldValue(_T("PART"), m_lbuSettings.nPartTo);
			if(m_rs.GetFieldValueAsInt(_T("ORD")) == 0)
				m_rs.SetFieldValue(_T("ORD"), m_wndGrid.GetNumberRows());
			m_rs.SetFieldValue(_T("PHRASE"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("PHRASE")), m_rsAutoCorrect));
			break;
		case adEditInProgress:
			m_rs.SetFieldValue(_T("PHRASE"), theApp.AutoCorrect(
				m_rs.GetFieldValueAsString(_T("PHRASE")), m_rsAutoCorrect));
			break;
		}
}

void CPhrasesUnitsFrame::OnDelete()
{
	CString strPhrase = m_rs.GetFieldValueAsString(_T("PHRASE"));
	CString strMsg;
	strMsg.Format(_T("The phrase \"%s\" is about to be DELETED. Are you sure?"), strPhrase);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
