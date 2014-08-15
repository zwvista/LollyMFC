
// AuxPicBooksFrame.h : interface of the CAuxPicBooksFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxPicBooksFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxPicBooksFrame)
public:
	CAuxPicBooksFrame();
	virtual ~CAuxPicBooksFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
