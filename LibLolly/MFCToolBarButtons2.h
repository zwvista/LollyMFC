#pragma once

class CMFCToolBarLabelButton : public CMFCToolBarButton  
{
	DECLARE_SERIAL(CMFCToolBarLabelButton)
public:
	CMFCToolBarLabelButton (UINT nID = 0, LPCTSTR lpszText = NULL);

	virtual void OnDraw (CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, 
		BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,						
		BOOL bHighlight = FALSE,						
		BOOL bDrawBorder = TRUE, 
		BOOL bGrayDisabledButtons = TRUE);
};

class CMFCToolBarEditBoxButton2 : public CMFCToolBarEditBoxButton
{
	DECLARE_SERIAL(CMFCToolBarEditBoxButton2)
public:
	CMFCToolBarEditBoxButton2() {}
	CMFCToolBarEditBoxButton2(UINT uiID, int iImage, DWORD dwStyle = ES_AUTOHSCROLL, int iWidth = 0)
		: CMFCToolBarEditBoxButton(uiID, iImage, dwStyle, iWidth) {}

	virtual BOOL NotifyCommand(int iNotifyCode);
};

class CMFCToolBarSpinEditBoxButton2 : public CMFCToolBarSpinEditBoxButton
{
	DECLARE_SERIAL(CMFCToolBarSpinEditBoxButton2)
public:
	CMFCToolBarSpinEditBoxButton2() {}
	CMFCToolBarSpinEditBoxButton2(UINT uiID, int iImage, DWORD dwStyle = ES_AUTOHSCROLL, int iWidth = 0)
		: CMFCToolBarSpinEditBoxButton(uiID, iImage, dwStyle, iWidth) {}

	virtual BOOL NotifyCommand(int iNotifyCode);
};

class CMFCToolBarComboBoxButton2 : public CMFCToolBarComboBoxButton
{
	DECLARE_SERIAL(CMFCToolBarComboBoxButton2)
public:
	CMFCToolBarComboBoxButton2() {}
	CMFCToolBarComboBoxButton2(UINT uiID, int iImage, DWORD dwStyle = ES_AUTOHSCROLL, int iWidth = 0, int iHeight = 0)
		: CMFCToolBarComboBoxButton(uiID, iImage, dwStyle, iWidth) { if(iHeight) m_nDropDownHeight = iHeight; }

	virtual BOOL NotifyCommand(int iNotifyCode);
};
