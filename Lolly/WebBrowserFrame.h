#pragma once

#include "LollyHtmlView.h"

// CWebBrowserFrame

class CWebBrowserFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CWebBrowserFrame)

public:
	CWebBrowserFrame();
	virtual ~CWebBrowserFrame();

	void Navigate2(LPCTSTR pszURL);

protected:
	CLollyHtmlView* m_pView;
	CMFCToolBarComboBoxButton* m_pcboAddress;

	CADORecordset2 m_rsWebText;

	virtual CString GetFrameText() const;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOpen();
	afx_msg void OnGo();
	afx_msg void OnSave();
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg void OnSpeak();
	afx_msg LRESULT OnHtmlViewDocComplete(WPARAM, LPARAM);
	afx_msg LRESULT OnHtmlViewNewWindow3(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


