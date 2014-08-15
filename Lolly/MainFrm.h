// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnShowPopupMenu (CMFCPopupMenu* pMenuPopup);
	virtual BOOL OnDrawMenuImage (CDC* pDC, const CMFCToolBarMenuButton* pMenuButton, const CRect& rectImage);
	virtual BOOL OnMenuButtonToolHitTest (CMFCToolBarButton* pButton, TOOLINFO* pTI);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CMenu			m_menuFavotites;
	BOOL IsFavoritesMenu (const CMFCToolBarMenuButton* pMenuButton);

// Generated message map functions
protected:
	void FindOrCreateChild(CRuntimeClass* pRuntimeClass, UINT nID);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnWindowClose();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnFavorite(UINT nID);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnWindowNextWordsLessons();
	afx_msg void OnWindowNextWordsLang();
	afx_msg void OnWindowNextWordsAtWill();
	afx_msg void OnWindowNextWordsBooks();
	afx_msg void OnWindowNextPhrasesLessons();
	afx_msg void OnWindowNextPhrasesLang();
	afx_msg void OnWindowNextWordsLessonsEB();
	afx_msg void OnWindowNextWordsAtWillEB();
	afx_msg void OnNewCurrentWindow();
	afx_msg void OnUpdateNewCurrentWindow(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


