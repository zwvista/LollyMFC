
// WordsLangFrame.h : interface of the CWordsLangFrame class
//

#pragma once
#include "WordsWebFrame.h"

class CWordsLangFrame : public CWordsWebFrame
{
	DECLARE_DYNCREATE(CWordsLangFrame)
public:
	CWordsLangFrame();
	virtual ~CWordsLangFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
