
// WordsAtWillEBFrame.h : interface of the CWordsAtWillEBFrame class
//

#pragma once
#include "WordsEBFrame.h"

class CWordsAtWillEBFrame : public CWordsEBFrame
{
	DECLARE_DYNCREATE(CWordsAtWillEBFrame)
public:
	CWordsAtWillEBFrame();
	virtual ~CWordsAtWillEBFrame();

// Generated message map functions
protected:

	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();
	virtual CString GetFrameText() const;
	virtual void LoadTables();

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
