
// AuxDictionariesFrame.h : interface of the CAuxDictionariesFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CAuxDictionariesFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CAuxDictionariesFrame)
public:
	CAuxDictionariesFrame();
	virtual ~CAuxDictionariesFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
