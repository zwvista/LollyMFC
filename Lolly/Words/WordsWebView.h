#pragma once


// CWordsWebView view
#include "DictHtmlCtrl.h"

class CWordsWebView : public CView
{
	DECLARE_DYNCREATE(CWordsWebView)

protected:
	CWordsWebView();           // protected constructor used by dynamic creation
	virtual ~CWordsWebView();

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	vector<shared_ptr<CDictHtmlCtrl>> m_vpDictHtmlCtrls;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};


