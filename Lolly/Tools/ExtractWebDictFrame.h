#pragma once

#include "LollyHtmlView.h"
#include "DictHtmlCtrl.h"

// CExtractWebDictFrame

class CExtractWebDictFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CExtractWebDictFrame)

public:
	CExtractWebDictFrame();
	virtual ~CExtractWebDictFrame();

	void Init(const vector<CString>& vstrWords, LPCTSTR pszDict, bool bOverwrite, CDictHtmlCtrl* pDictHtmlCtrl = nullptr, LPCTSTR pszIfrId = nullptr);

protected:
	CLollyHtmlView* m_pView;

	SLangBookUnitSettings m_lbuSettings;
	vector<CString> m_vstrWords;
	bool m_bOverwrite;
	int m_nWordIndex;
	CMFCToolBarEditBoxButton2 *m_pedtWord, *m_pedtLang, *m_pedtDict;
	CString m_strWord;
	CADORecordset2 m_rsWord;
	CADORecordset2 m_rsDict;
	CADORecordset2 m_rsAutoCorrect;
	enum {DIC_NAVIGATING, DIC_AUTOMATING, DIC_READY} m_eDicStatus;
	CDictHtmlCtrl* m_pDictHtmlCtrl;
    CString m_strIfrId;

	void GetNextWord();
	void GetNextDict();
	bool NeedSearchDict();
	bool GetNextSearch();
	void SearchDictForWord();
	void ExtractWebDict();

	virtual CString GetFrameText() const;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnHtmlViewDocComplete(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
