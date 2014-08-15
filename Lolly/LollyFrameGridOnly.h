
// BooksFrm.h : interface of the CLollyFrameGridOnly class
//

#pragma once

#include "LollyFrameGrid.h"

class CLollyFrameGridOnly : public CLollyFrameGrid
{
	DECLARE_DYNCREATE(CLollyFrameGridOnly)
public:
	CLollyFrameGridOnly();

// Attributes
public:

// Operations
public:

// Overrides
public:

// Implementation
public:
	virtual ~CLollyFrameGridOnly();

// Generated message map functions
protected:
	bool m_bDataGridMultiLine;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};
