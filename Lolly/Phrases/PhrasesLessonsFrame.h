
// PhrasesLessonsFrame.h : interface of the CPhrasesLessonsFrame class
//

#pragma once
#include "PhrasesBaseFrame.h"

class CPhrasesLessonsFrame : public CPhrasesBaseFrame
{
	DECLARE_DYNCREATE(CPhrasesLessonsFrame)
public:
	CPhrasesLessonsFrame();
	virtual ~CPhrasesLessonsFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo();
	virtual CString GetFrameText() const;

	virtual void WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum *adStatus, struct _Recordset *pRecordset);

	afx_msg void OnDelete();
	DECLARE_MESSAGE_MAP()
};
