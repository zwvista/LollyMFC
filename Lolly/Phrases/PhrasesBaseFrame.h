
// PhrasesBaseFrame.h : interface of the CPhrasesBaseFrame class
//

#pragma once
#include "LollyFrameGridOnly.h"

class CPhrasesBaseFrame : public CLollyFrameGridOnly
{
	DECLARE_DYNCREATE(CPhrasesBaseFrame)
public:
	CPhrasesBaseFrame();
	virtual ~CPhrasesBaseFrame();

// Generated message map functions
protected:
	bool m_bKeepSpeak, m_bKeepSpeakTrans;

	void SpeakPhraseTrans(bool bPhrase, bool bTrans);

	virtual void OnMoveComplete();

	afx_msg LRESULT OnLblSettingsChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSpeak();
	afx_msg void OnKeepSpeak();
	afx_msg void OnUpdateKeepSpeak(CCmdUI* pCmdUI);
	afx_msg void OnSpeakTrans();
	afx_msg void OnKeepSpeakTrans();
	afx_msg void OnUpdateKeepSpeakTrans(CCmdUI* pCmdUI);
	afx_msg void OnMultiAdd();
	DECLARE_MESSAGE_MAP()
};
