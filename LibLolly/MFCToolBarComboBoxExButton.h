#pragma once


// CMFCToolBarComboBoxExButton

class CMFCToolBarComboBoxExButton : public CMFCToolBarButton
{
	DECLARE_SERIAL(CMFCToolBarComboBoxExButton)

public:
	CMFCToolBarComboBoxExButton();
	CMFCToolBarComboBoxExButton(UINT uiID, int iImage, DWORD dwStyle = CBS_DROPDOWNLIST, int iWidth = 0, int iHeight = 0);
	virtual ~CMFCToolBarComboBoxExButton();

// Operations:
	BOOL InsertItem(const COMBOBOXEXITEM* pCBItem);
	BOOL SelectItem(int iIndex, BOOL bNotify = TRUE);
	BOOL DeleteItem(int iIndex);
	BOOL GetItem(COMBOBOXEXITEM* pCBItem);
	BOOL SetItem(const COMBOBOXEXITEM* pCBItem);
	INT_PTR GetCount() const { return m_lstItems.GetCount(); }
	void RemoveAllItems();
	int GetCurSel() const { return m_iSelIndex; }
	void SetDropDownHeight(int nHeight);
	CImageList* SetImageList(CImageList* pImageList);
	CImageList* GetImageList( ) const { return m_pImageList; }

// Overrides:
	virtual CComboBoxEx* CreateCombo(CWnd* pWndParent, const CRect& rect);
	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
		BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE);
	virtual BOOL OnClick(CWnd* pWnd, BOOL bDelay = TRUE);
	virtual void OnChangeParentWnd(CWnd* pWndParent);
	virtual void CopyFrom(const CMFCToolBarButton& src);
	virtual SIZE OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnMove();
	virtual void OnSize(int iSize);
	virtual HWND GetHwnd() { return m_pWndCombo->GetSafeHwnd(); }

	virtual void EnableWindow(BOOL bEnable = TRUE)
	{
		if (m_pWndCombo->GetSafeHwnd() != NULL)
		{
			m_pWndCombo->EnableWindow(bEnable);
		}
	}

	virtual BOOL IsWindowVisible()
	{
		return m_pWndCombo->GetSafeHwnd() != NULL && m_pWndCombo->GetStyle() & WS_VISIBLE;
	}

	virtual BOOL IsOwnerOf(HWND hwnd)
	{
		if (m_pWndCombo->GetSafeHwnd() != NULL && (m_pWndCombo->GetSafeHwnd() == hwnd || ::IsChild(m_pWndCombo->GetSafeHwnd(), hwnd)))
		{
			return TRUE;
		}
		return FALSE;
	}

	virtual BOOL NotifyCommand(int iNotifyCode);

	virtual BOOL CanBeStretched() const { return TRUE; }
	virtual void OnShow(BOOL bShow);

protected:
	void Initialize();
	void AdjustRect();

// Attributes:
public:
	CComboBoxEx* GetComboBox() const { return m_pWndCombo; }
	virtual BOOL HasFocus() const;
	static CMFCToolBarComboBoxExButton* __stdcall GetByCmd(UINT uiCmd, BOOL bIsFocus = FALSE);

protected:
	int   m_iWidth;
	int   m_iSelIndex;
	int   m_nDropDownHeight;
	BOOL  m_bHorz;
	DWORD m_dwStyle;
	CRect m_rectCombo;
	CRect m_rectButton;

	CComboBoxEx* m_pWndCombo;
	CImageList* m_pImageList;
	CList<COMBOBOXEXITEM> m_lstItems;
};


