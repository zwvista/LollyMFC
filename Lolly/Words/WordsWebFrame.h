
// WordsWebFrame.h : interface of the CWordsWebFrame class
//

#pragma once

#include "WordsBaseFrame.h"
#include "WordsWebView.h"

class CWordsWebFrame : public CWordsBaseFrame
{
	DECLARE_DYNCREATE(CWordsWebFrame)
public:
	CWordsWebFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CWordsWebFrame();

// Generated message map functions
protected:
	CWordsWebView* m_pView;
	CDictHtmlCtrl* m_pDictHtmlCtrl;
    vector<shared_ptr<CUIDict>> m_vpUIDicts;

	void UpdateHtml(CDictHtmlCtrl* pDictHtmlCtrl);
	void UpdateHtml();

	virtual void OnMoveComplete();
	virtual void LoadDicts();
	virtual void OnDictSelected();
	virtual void AddDict(UINT nID, CUIDict* pUIDict);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDelete();
	afx_msg void OnExtract(UINT nID);
	afx_msg void OnUpdateExtractOverwrite(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExtractAppend(CCmdUI* pCmdUI);
	afx_msg void OnEditTrans();
	afx_msg void OnUpdateEditTrans(CCmdUI* pCmdUI);
	afx_msg void OnDeleteTrans();
	afx_msg void OnUpdateDeleteTrans(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnHtmlViewDocComplete(WPARAM wParam, LPARAM lParam);
    afx_msg void OnConfigDicts();
    DECLARE_MESSAGE_MAP()
};
