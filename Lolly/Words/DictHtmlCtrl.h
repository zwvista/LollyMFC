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

    const CDictLangConfig* m_pConfig;
    vector<shared_ptr<CUIDictItem>> m_vpDictItems;
	CADORecordset2 m_rsWord;
	bool m_bEmptyTrans;
	bool m_bAutomationDone;

	void UpdateLiveHtml(LPCTSTR pszIfrId, LPCTSTR pszWord, LPCTSTR pszDict, LPCTSTR pszTranslation);
	void UpdateHtml(const CString& strWord, CADORecordset2& rsAutoCorrect);
	void FindDict(LPCTSTR pszDictName);
    bool CanDeleteTranslation();
    bool CanEditTranslation();
    bool CanExtractAndOverriteTranslation();
    bool CanExtractAndAppendTranslation();
    bool DoDeleteTranslation(const CString& strWord);
    bool DoEditTranslation(const CString& strWord);
    bool DoExtractTranslation(const CString& strWord, bool bOverriteDB);
    void DoWebAutomation(const CString& strWord);

protected:
    CString GetTemplatedHtml(const CString& strWord, const CString& str);
    CString GetTranslation(const CString& strWord);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
