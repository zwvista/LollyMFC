
// AuxAutoCorrectFrame.h : interface of the CAuxAutoCorrectFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxAutoCorrectFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxAutoCorrectFrame)
public:
	CAuxAutoCorrectFrame();
	virtual ~CAuxAutoCorrectFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
