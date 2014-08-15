#pragma once

// CDictHtmlCtrl html view

#include "LollyHtmlView.h"

class CDictHtmlCtrl : public CLollyHtmlView
{
	DECLARE_DYNCREATE(CDictHtmlCtrl)

public:
	CDictHtmlCtrl();           // protected constructor used by dynamic creation
	virtual ~CDictHtmlCtrl();

	// create control from scratch
	BOOL Create(const RECT& rc, CWnd* pParent, UINT nID,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE, CCreateContext* pContext = NULL)
	{
		return CLollyHtmlView::Create(NULL, NULL, dwStyle, rc, pParent, nID, pContext);
	}
	// Normally, CLollyHtmlView destroys itself in PostNcDestroy, but we don't want
	// to do that for a control since a control is usually implemented as a
	// member of another window object.
	//
	virtual void PostNcDestroy() {  }

	CString m_strDictName;
	EDictImage m_nDictImage;
	CADORecordset2 m_rsDict;
	CADORecordset2 m_rsWord;
	bool m_bEmptyTrans;
	bool m_bAutomationDone;

	void UpdateLiveHtml(LPCTSTR pszWord, LPCTSTR pszDict, LPCTSTR pszTranslation);
	void UpdateHtml(const CString& strWord, CADORecordset2& rsAutoCorrect, const vector<CString>& vstrLingoesDicts, const vector<CString>& vstrOfflineDicts, const map<CString, vector<DictInfo>>& mapCustomDicts);
	void FindDict(LPCTSTR pszDictName);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
