#pragma once


// CLollyFrame

class CLollyFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CLollyFrame)

public:
	CLollyFrame();
	virtual ~CLollyFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnDrawMenuImage (CDC* pDC, const CMFCToolBarMenuButton* pMenuButton, const CRect& rectImage){return FALSE;}

protected:
	CMFCToolBar m_wndToolBar;
	UINT m_nToolBarID;

	void CreateView(CRuntimeClass* pNewViewClass);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


