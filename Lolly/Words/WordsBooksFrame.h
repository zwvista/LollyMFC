
// WordsBooksFrame.h : interface of the CWordsBooksFrame class
//

#pragma once
#include "WordsWebFrame.h"

class CWordsBooksFrame : public CWordsWebFrame
{
	DECLARE_DYNCREATE(CWordsBooksFrame)
public:
	CWordsBooksFrame();
	virtual ~CWordsBooksFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);
	virtual void RecordChangeComplete(EventReasonEnum adReason, LONG cRecords, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
