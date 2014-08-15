#pragma once

#include "StartProcess.h"

// CPicBookView view

class CPicBookView : public CView, CStartProcess
{
	DECLARE_DYNCREATE(CPicBookView)

protected:
	CPicBookView();           // protected constructor used by dynamic creation
	virtual ~CPicBookView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	void SelectLine(bool bLine1);
	void BeginWrite();
	void AutoCorrectWriting();
	void OpenFile();

protected:
	HWND m_hwndBarTool, m_hwndBarColor, m_hwndView;

	CADORecordset2 m_rsAutoCorrect;

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};