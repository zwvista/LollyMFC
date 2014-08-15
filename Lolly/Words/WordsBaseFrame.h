
// BooksFrm.h : interface of the CWordsBaseFrame class
//

#pragma once

#include "LollyFrameGrid.h"
#include "LollyHtmlView.h"

class CWordsBaseFrame : public CLollyFrameGrid
{
	DECLARE_DYNCREATE(CWordsBaseFrame)
public:
	CWordsBaseFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:

// Implementation
public:
	virtual ~CWordsBaseFrame();
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL OnDrawMenuImage (CDC* pDC, const CMFCToolBarMenuButton* pMenuButton, const CRect& rectImage);

// Generated message map functions
protected:
	CSplitterWndEx m_wndSplitter;
	CString m_strWord;
	bool m_bKeepSpeak;
	CMFCToolBar m_wndToolBarDicts;
	CMFCToolBarMenuButton* m_pbtnDicts;
	map<int, pair<CString, EDictImage>> m_mapDictID2Info;
	int m_nDictIndex;
	map<CString, int> m_mapWord2Level;
	CADORecordset2 m_rsWordLevel;

	virtual void OnMoveComplete();
	virtual void LoadTables();
	virtual void LoadDicts(){}
	virtual void OnDictSelected(){}
	virtual void OnGridItemEntered(const CString& strGridItem);
	virtual void AddDict(UINT nID, LPCTSTR pszDict, EDictImage nImageIndex);
	virtual int RemoveDict(UINT nID);
	int GetDictIndexByID(UINT nID);
	virtual void OnAddComplete(){}

	void CreateDictCtrls();
	void SelectDict(int nIndex);
	void SelectNextDict(bool bForward, int nDicts);
	void DataGridCellFormatting(int col,long row,CUGCell *cell);
	int GetWordLevel(LPCTSTR pszWord);

	afx_msg void OnSpeak();
	afx_msg void OnKeepSpeak();
	afx_msg void OnUpdateSpeak(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKeepSpeak(CCmdUI* pCmdUI);
	afx_msg void OnMultiAdd();
	afx_msg void OnUpdateDicts(CCmdUI* pCmdUI);
	afx_msg void OnDicts(UINT nID);
	afx_msg void OnAvailableDicts(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWordLevelChange(UINT nID);
	afx_msg void OnUpdateWordLevelChange(CCmdUI* pCmdUI){}
	afx_msg void OnRefresh();
	DECLARE_MESSAGE_MAP()
};
