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
	bool m_bSourceView, m_bLineOnly;;
	CMFCToolBarComboBoxButton2* m_pcboPatternNames;
	vector<CString> m_vstrPatterns;

	void NewNote(const CString& strTag, bool bNoNoteTag);
	void InitPatterns();

	virtual CString GetFrameText() const;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSourceView();
	afx_msg void OnUpdateSourceView(CCmdUI* pCmdUI){pCmdUI->SetCheck(m_bSourceView);}
	afx_msg void OnLineOnly() {m_bLineOnly = !m_bLineOnly;}
	afx_msg void OnUpdateLineOnly(CCmdUI* pCmdUI){
		pCmdUI->SetCheck(m_bLineOnly), pCmdUI->Enable(!m_bSourceView);
	}
	afx_msg void OnNewWord();
	afx_msg void OnNewPattern();
	afx_msg void OnPatternNames(){}
	afx_msg void OnOriginal();
	afx_msg void OnDefinition();
	afx_msg void OnTranslation();
	afx_msg void OnUpdateTreeViewButtons(CCmdUI* pCmdUI){pCmdUI->Enable(!m_bSourceView);}
	afx_msg void OnUpdateSourceViewButtons(CCmdUI* pCmdUI){pCmdUI->Enable(m_bSourceView);}
};
