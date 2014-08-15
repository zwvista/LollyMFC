#pragma once

#include "BlogPostView.h"

// CBlogPostFrame

class CBlogPostFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CBlogPostFrame)

public:
	CBlogPostFrame();
	virtual ~CBlogPostFrame();

protected:
	CBlogPostView* m_pView;

	virtual CString GetFrameText() const;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
};
