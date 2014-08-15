
// AuxWebExtractFrame.h : interface of the CAuxWebExtractFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxWebExtractFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxWebExtractFrame)
public:
	CAuxWebExtractFrame();
	virtual ~CAuxWebExtractFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
