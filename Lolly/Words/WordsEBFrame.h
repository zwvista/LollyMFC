
// WordsEBFrame.h : interface of the CWordsEBFrame class
//

#pragma once

#include "WordsBaseFrame.h"

class CWordsEBView;

class CWordsEBFrame : public CWordsBaseFrame
{
	DECLARE_DYNCREATE(CWordsEBFrame)
public:
	CWordsEBFrame();

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
	virtual ~CWordsEBFrame();

// Generated message map functions
protected:
	CWordsEBView* m_pView;
	CString m_strKana;
	bool m_bLookupByKana;
	bool m_bIsFindingKana;
	vector<CString> m_vstrDicts;

	void UpdateEBView();

	virtual void OnMoveComplete();
	virtual void LoadDicts();
	virtual void OnDictSelected();
	virtual void OnAddComplete();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeLookupBy(UINT nID);
	afx_msg void OnUpdateChangeLookupBy(CCmdUI* pCmdUI);
	afx_msg void OnFindKana();
	afx_msg void OnCopyKanjiKana();
	afx_msg void OnUpdateCopyKanjiKana(CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAvailableDicts(UINT nID);
	afx_msg void OnUpdateAvailableDicts(CCmdUI* pCmdUI);
	afx_msg void OnRefresh();
	DECLARE_MESSAGE_MAP()
};
