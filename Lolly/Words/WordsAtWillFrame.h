
// WordsAtWillFrame.h : interface of the CWordsAtWillFrame class
//

#pragma once
#include "WordsWebFrame.h"

class CWordsAtWillFrame : public CWordsWebFrame
{
	DECLARE_DYNCREATE(CWordsAtWillFrame)
public:
	CWordsAtWillFrame();
	virtual ~CWordsAtWillFrame();

// Generated message map functions
protected:

	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();
	virtual CString GetFrameText() const;
	virtual void LoadTables();

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
