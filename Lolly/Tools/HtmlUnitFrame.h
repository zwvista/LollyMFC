#pragma once

// CHtmlUnitFrame

class CHtmlUnitFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CHtmlUnitFrame)

public:
	CHtmlUnitFrame();
	virtual ~CHtmlUnitFrame();

protected:
	CString m_strFileName;
	CHtmlEditView* m_pView;

	virtual CString GetFrameText() const;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnSetBack1();
	afx_msg void OnSetBack2();
	afx_msg void OnRemoveFormat();
	afx_msg void OnSpeak();
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};


