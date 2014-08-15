#pragma once

// CHtmlCtrl html view


class CHtmlCtrl : public CHtmlView
{
	DECLARE_DYNCREATE(CHtmlCtrl)

public:
	CHtmlCtrl();           // protected constructor used by dynamic creation
	virtual ~CHtmlCtrl();

	// craete control from scratch
	BOOL Create(const RECT& rc, CWnd* pParent, UINT nID,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE, CCreateContext* pContext = NULL)
	{
		return CHtmlView::Create(NULL, NULL, dwStyle, rc, pParent, nID, pContext);
	}
	// Normally, CHtmlView destroys itself in PostNcDestroy, but we don't want
	// to do that for a control since a control is usually implemented as a
	// member of another window object.
	//
	virtual void PostNcDestroy() {  }

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
