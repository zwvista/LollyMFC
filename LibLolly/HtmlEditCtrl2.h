#pragma once

// CHtmlEditCtrl2 html view

class CHtmlEditCtrl2 : public CHtmlEditView
{
	DECLARE_DYNCREATE(CHtmlEditCtrl2)

public:
	CHtmlEditCtrl2();           // protected constructor used by dynamic creation
	virtual ~CHtmlEditCtrl2();

	// craete control from scratch
	BOOL Create(const RECT& rc, CWnd* pParent, UINT nID,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE, CCreateContext* pContext = NULL)
	{
		return CHtmlEditView::Create(NULL, NULL, dwStyle, rc, pParent, nID, pContext);
	}
	// Normally, CHtmlEditView destroys itself in PostNcDestroy, but we don't want
	// to do that for a control since a control is usually implemented as a
	// member of another window object.
	//
	virtual void PostNcDestroy() {  }

protected:
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
