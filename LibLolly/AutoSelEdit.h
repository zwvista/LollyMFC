#pragma once


// CAutoSelEdit

class CAutoSelEdit : public CEdit
{
	DECLARE_DYNAMIC(CAutoSelEdit)

public:
	CAutoSelEdit();
	virtual ~CAutoSelEdit();

protected:
	DECLARE_MESSAGE_MAP()

	bool m_bSelectAll;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


