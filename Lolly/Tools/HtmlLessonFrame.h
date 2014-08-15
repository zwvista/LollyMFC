#pragma once

// CHtmlLessonFrame

class CHtmlLessonFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CHtmlLessonFrame)

public:
	CHtmlLessonFrame();
	virtual ~CHtmlLessonFrame();

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


