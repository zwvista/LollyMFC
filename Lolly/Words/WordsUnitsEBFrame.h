
// WordsUnitsEBFrame.h : interface of the CWordsUnitsEBFrame class
//

#pragma once

#include "WordsEBFrame.h"

class CWordsEBView;

class CWordsUnitsEBFrame : public CWordsEBFrame
{
	DECLARE_DYNCREATE(CWordsUnitsEBFrame)
public:
	CWordsUnitsEBFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:

// Implementation
public:
	virtual ~CWordsUnitsEBFrame();

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
