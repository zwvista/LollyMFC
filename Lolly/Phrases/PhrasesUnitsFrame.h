
// PhrasesUnitsFrame.h : interface of the CPhrasesUnitsFrame class
//

#pragma once
#include "PhrasesBaseFrame.h"

class CPhrasesUnitsFrame : public CPhrasesBaseFrame
{
	DECLARE_DYNCREATE(CPhrasesUnitsFrame)
public:
	CPhrasesUnitsFrame();
	virtual ~CPhrasesUnitsFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
