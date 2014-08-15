
// AuxBooksFrame.h : interface of the CAuxBooksFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxBooksFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxBooksFrame)
public:
	CAuxBooksFrame();
	virtual ~CAuxBooksFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
