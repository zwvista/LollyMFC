
// AuxWebTextFrame.h : interface of the CAuxWebTextFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxWebTextFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxWebTextFrame)
public:
	CAuxWebTextFrame();
	virtual ~CAuxWebTextFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
