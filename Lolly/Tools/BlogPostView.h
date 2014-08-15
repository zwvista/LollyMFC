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

	void NewNote(const CString& strNote);
	void ToggleView();

protected:
	CEdit m_edtSrc;
	CString m_strFileName;
	void OnBI(LPCTSTR pszFormat);

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnB() {OnBI(_T("<b>%s</b>"));}
	afx_msg void OnI() {OnBI(_T("<i>%s</i>"));}
	DECLARE_MESSAGE_MAP()
};