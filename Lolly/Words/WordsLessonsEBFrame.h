
// WordsLessonsEBFrame.h : interface of the CWordsLessonsEBFrame class
//

#pragma once

#include "WordsEBFrame.h"

class CWordsEBView;

class CWordsLessonsEBFrame : public CWordsEBFrame
{
	DECLARE_DYNCREATE(CWordsLessonsEBFrame)
public:
	CWordsLessonsEBFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:

// Implementation
public:
	virtual ~CWordsLessonsEBFrame();

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
