
// WordsUnitsFrame.h : interface of the CWordsUnitsFrame class
//

#pragma once
#include "WordsWebFrame.h"

class CWordsUnitsFrame : public CWordsWebFrame
{
	DECLARE_DYNCREATE(CWordsUnitsFrame)
public:
	CWordsUnitsFrame();
	virtual ~CWordsUnitsFrame();

// Generated message map functions
protected:
	enum EditModeEnum m_eEditMode;

	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);
	virtual void RecordChangeComplete(EventReasonEnum adReason, LONG cRecords, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	void InsertWordIfNeeded();
	void DeleteWordIfNeeded();

	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
