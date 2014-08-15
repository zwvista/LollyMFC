
// PhrasesLangFrame.h : interface of the CPhrasesLangFrame class
//

#pragma once
#include "PhrasesBaseFrame.h"

class CPhrasesLangFrame : public CPhrasesBaseFrame
{
	DECLARE_DYNCREATE(CPhrasesLangFrame)
public:
	CPhrasesLangFrame();
	virtual ~CPhrasesLangFrame();

// Generated message map functions
protected:
	virtual CString GetSQL();
	virtual SDataGridColumnInfo* GetDataGridColumnInfo();
	virtual CString GetFrameText() const;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
