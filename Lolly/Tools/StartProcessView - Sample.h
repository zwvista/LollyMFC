#pragma once

#include "StartProcess.h"

// CBlogPostView view

class CBlogPostView : public CView, CStartProcess
{
	DECLARE_DYNCREATE(CBlogPostView)

protected:
	CBlogPostView();           // protected constructor used by dynamic creation
	virtual ~CBlogPostView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

protected:

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};