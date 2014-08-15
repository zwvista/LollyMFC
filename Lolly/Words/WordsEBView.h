#pragma once

#include "StartProcess.h"

// CWordsEBView view

class CWordsEBView : public CView, CStartProcess
{
	DECLARE_DYNCREATE(CWordsEBView)

protected:
	CWordsEBView();           // protected constructor used by dynamic creation
	virtual ~CWordsEBView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	void Lookup(LPCTSTR pszWord);
	void SelectEntry(LPCTSTR pszRef);
	CString FindKana(LPCTSTR pszWord);
	void ChooseDict(LPCTSTR pszDict);
	void SearchAllDicts(bool bAll);

protected:
	HWND m_hwndComboWord;
	HWND m_hwndButtonFind;
	HWND m_hwndComboDicts;
	HWND m_hwndlstWords;
	HWND m_hwndtbSearch;

	void GetWordList(vector<CString>& vstrWords);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


