
// AuxDictionariesFrame.cpp : implementation of the CAuxDictionariesFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "AuxDictionariesFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAuxDictionariesFrame

IMPLEMENT_DYNCREATE(CAuxDictionariesFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CAuxDictionariesFrame, CLollyFrameGridOnly)
	ON_COMMAND(ID_TB_DELETE, &CAuxDictionariesFrame::OnDelete)
END_MESSAGE_MAP()

// CAuxDictionariesFrame construction/destruction

CAuxDictionariesFrame::CAuxDictionariesFrame()
{
	m_nToolBarID = IDR_TOOLBAR_AUXDICTIONARIES;
	m_bDataGridMultiLine = true;
}

CAuxDictionariesFrame::~CAuxDictionariesFrame()
{
}

CString CAuxDictionariesFrame::GetSQL()
{
	CString sql;
	sql.Format(_T("SELECT * FROM DICTIONARIES WHERE LANGID=%d"), m_lblSettings.nLangID);
	return sql;
}

SDataGridColumnInfo* CAuxDictionariesFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("INDEX"), _T("INDEX"), _T("[INDEX]"), 50, 0, TRUE },
		{ _T("DICTTYPEID"), _T("DICTTYPE"), NULL, 100, 0, TRUE },
		{ _T("DICTNAME"), _T("DICTNAME"), _T("DICTNAME"), 100, 0, TRUE },
		{ _T("LANGIDTO"), _T("LANGTO"), NULL, 100, 0, TRUE },
		{ _T("URL"), _T("URL"), _T("URL"), 2, 6, TRUE },
		{ _T("CHCONV"), _T("CHCONV"), _T("CHCONV"), 75, 0, TRUE },
		{ _T("AUTOMATION"), _T("AUTOMATION"), NULL, 2, 6, TRUE },
		{ _T("AUTOJUMP"), _T("AUTOJUMP"), NULL, 50, 0, TRUE },
		{ _T("DICTTABLE"), _T("DICTTABLE"), _T("DICTTABLE"), 75, 0, TRUE },
		{ _T("TEMPLATE"), _T("TEMPLATE"), NULL, 2, 6, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CAuxDictionariesFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Dictionaries (%s)"), m_lblSettings.GetLangDesc());
	return str;
}

void CAuxDictionariesFrame::WillChangeRecord( EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset )
{
	if(adReason == adRsnUpdate)
		switch(m_rs.GetEditMode()){
		case adEditAdd:
			m_rs.SetFieldValue(_T("LANGID"), m_lblSettings.nLangID);
			break;
		case adEditInProgress:
			{
				CString strItem = m_rs.GetFieldValueAsString(_T("DICTNAME"));
				CString strMsg;
				strMsg.Format(_T("The webextract item \"%s\" is about to be updated. Are you sure?"), strItem);
				if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2) == IDNO)
					*adStatus = adStatusCancel;
			}
			break;
		}
}

void CAuxDictionariesFrame::OnDelete()
{
	CString strItem = m_rs.GetFieldValueAsString(_T("DICTNAME"));
	CString strMsg;
	strMsg.Format(_T("The autocorrect item \"%s\" is about to be DELETED. Are you sure?"), strItem);
	if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) == IDYES)
		m_wndGrid.DeleteRow(m_wndGrid.GetCurrentRow());
}
