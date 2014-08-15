// MFCToolBarComboBoxExButton.cpp : implementation file
//

#include "stdafx.h"
#include "MFCToolBarComboBoxExButton.h"


// CMFCToolBarComboBoxExButton

IMPLEMENT_SERIAL(CMFCToolBarComboBoxExButton, CMFCToolBarButton, 1)

static const int nDefaultComboHeight = 150;
static const int nDefaultSize = 150;
static const int nHorzMargin = 1;

CMFCToolBarComboBoxExButton::CMFCToolBarComboBoxExButton()
{
	m_dwStyle = WS_CHILD | WS_VISIBLE | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL;
	m_iWidth = nDefaultSize;
	m_nDropDownHeight = nDefaultComboHeight;

	Initialize();
}

CMFCToolBarComboBoxExButton::CMFCToolBarComboBoxExButton( UINT uiID, int iImage, DWORD dwStyle /*= CBS_DROPDOWNLIST*/, int iWidth /*= 0*/, int iHeight/*= 0*/ )
	: CMFCToolBarButton(uiID, iImage)
{
	m_dwStyle = dwStyle | WS_CHILD | WS_VISIBLE | WS_VSCROLL;
	m_iWidth = (iWidth == 0) ? nDefaultSize : iWidth;
	m_nDropDownHeight = iHeight == 0 ? nDefaultComboHeight : iHeight;

	Initialize();
}

void CMFCToolBarComboBoxExButton::Initialize()
{
	m_iSelIndex = -1;
	m_pWndCombo = NULL;
	m_bHorz = TRUE;
	m_rectCombo.SetRectEmpty();
	m_rectButton.SetRectEmpty();
	m_pImageList = NULL;
}

CMFCToolBarComboBoxExButton::~CMFCToolBarComboBoxExButton()
{
}

BOOL CMFCToolBarComboBoxExButton::InsertItem( const COMBOBOXEXITEM* pCBItem )
{
	int iIndex = pCBItem->iItem;
	if (iIndex < 0 || iIndex > m_lstItems.GetCount())
	{
		return FALSE;
	}

	POSITION pos = m_lstItems.FindIndex(iIndex);
	if (pos == NULL)
		m_lstItems.AddTail(*pCBItem);
	else
		m_lstItems.InsertBefore(pos, *pCBItem);

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->InsertItem(pCBItem);
	}

	return TRUE;
}

BOOL CMFCToolBarComboBoxExButton::SelectItem( int iIndex, BOOL bNotify /*= TRUE*/ )
{
	if (iIndex >= m_lstItems.GetCount())
	{
		return FALSE;
	}

	m_iSelIndex = max(-1, iIndex);

	if (m_pWndCombo->GetSafeHwnd() == NULL)
	{
		return TRUE;
	}

	if (m_pWndCombo->GetCurSel() == iIndex)
	{
		// Already selected
		return TRUE;
	}

	if (m_pWndCombo->SetCurSel(iIndex) != CB_ERR)
	{
		if (bNotify)
		{
			NotifyCommand(CBN_SELENDOK);
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CMFCToolBarComboBoxExButton::DeleteItem( int iIndex )
{
	if (iIndex < 0 || iIndex >= m_lstItems.GetCount())
	{
		return FALSE;
	}

	POSITION pos = m_lstItems.FindIndex(iIndex);
	if (pos == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_lstItems.RemoveAt(pos);

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->DeleteItem(iIndex);
	}

	if (iIndex == m_iSelIndex)
	{
		int iSelIndex = m_iSelIndex;
		if (iSelIndex >= m_lstItems.GetCount())
		{
			iSelIndex = (int) m_lstItems.GetCount() - 1;
		}

		SelectItem(iSelIndex, FALSE);
	}

	return TRUE;
}

BOOL CMFCToolBarComboBoxExButton::GetItem( COMBOBOXEXITEM* pCBItem )
{
	int iIndex = pCBItem->iItem;
	if (iIndex < 0 || iIndex >= m_lstItems.GetCount())
	{
		return FALSE;
	}

	POSITION pos = m_lstItems.FindIndex(iIndex);
	if (pos == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->GetItem(pCBItem);
	}
	else
	{
		*pCBItem = m_lstItems.GetAt(pos);
	}

	return TRUE;
}

BOOL CMFCToolBarComboBoxExButton::SetItem( const COMBOBOXEXITEM* pCBItem )
{
	int iIndex = pCBItem->iItem;
	if (iIndex < 0 || iIndex > m_lstItems.GetCount())
	{
		return FALSE;
	}

	POSITION pos = m_lstItems.FindIndex(iIndex);
	if (pos == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_lstItems.SetAt(pos, *pCBItem);

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->SetItem(pCBItem);
	}

	return TRUE;
}

void CMFCToolBarComboBoxExButton::RemoveAllItems()
{
	m_lstItems.RemoveAll();

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->ResetContent();
	}
}

void CMFCToolBarComboBoxExButton::SetDropDownHeight( int nHeight )
{
	if (m_nDropDownHeight == nHeight)
	{
		return;
	}

	m_nDropDownHeight = nHeight;
	OnMove();
}

CImageList* CMFCToolBarComboBoxExButton::SetImageList( CImageList* pImageList )
{
	CImageList* p = m_pImageList;
	m_pImageList = pImageList;

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		p = m_pWndCombo->SetImageList(pImageList);
	}

	return p;
}

void CMFCToolBarComboBoxExButton::OnDraw( CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz /*= TRUE*/, BOOL bCustomizeMode /*= FALSE*/, BOOL bHighlight /*= FALSE*/, BOOL bDrawBorder /*= TRUE*/, BOOL bGrayDisabledButtons /*= TRUE*/ )
{
	if (m_pWndCombo == NULL || m_pWndCombo->GetSafeHwnd() == NULL || !bHorz)
	{
		CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
		return;
	}
}

void CMFCToolBarComboBoxExButton::OnChangeParentWnd( CWnd* pWndParent )
{
	CMFCToolBarButton::OnChangeParentWnd(pWndParent);

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		CWnd* pWndParentCurr = m_pWndCombo->GetParent();
		ENSURE(pWndParentCurr != NULL);

		if (pWndParent != NULL && pWndParentCurr->GetSafeHwnd() == pWndParent->GetSafeHwnd())
		{
			return;
		}

		m_pWndCombo->DestroyWindow();
		delete m_pWndCombo;
		m_pWndCombo = NULL;
	}

	if (pWndParent == NULL || pWndParent->GetSafeHwnd() == NULL)
	{
		return;
	}

	BOOL bDisabled = CMFCToolBar::IsCustomizeMode() ||(m_nStyle & TBBS_DISABLED);

	CRect rect = m_rect;
	rect.InflateRect(-2, 0);
	rect.bottom = rect.top + m_nDropDownHeight;

	if ((m_pWndCombo = CreateCombo(pWndParent, rect)) == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (m_pWndCombo != NULL && m_pWndCombo->GetSafeHwnd() != NULL)
	{
		m_pWndCombo->EnableWindow(!bDisabled);
		m_pWndCombo->RedrawWindow();
	}

	AdjustRect();

	m_pWndCombo->SetFont(&afxGlobalData.fontRegular);

	if (m_pWndCombo->GetCount() > 0)
	{
		m_lstItems.RemoveAll();

		for (int i = 0; i < m_pWndCombo->GetCount(); i ++)
		{
			COMBOBOXEXITEM CBItem;
			CBItem.iItem = i;
			m_pWndCombo->GetItem(&CBItem);

			m_lstItems.AddTail(CBItem);
		}

		m_iSelIndex = m_pWndCombo->GetCurSel();
	}
	else
	{
		m_pWndCombo->ResetContent();

		for (POSITION pos = m_lstItems.GetHeadPosition(); pos != NULL;)
		{
			const COMBOBOXEXITEM& CBItem = m_lstItems.GetNext(pos);
			m_pWndCombo->InsertItem(&CBItem);
		}

		if (m_iSelIndex != CB_ERR)
		{
			m_pWndCombo->SetCurSel(m_iSelIndex);
		}
	}

}

CComboBoxEx* CMFCToolBarComboBoxExButton::CreateCombo( CWnd* pWndParent, const CRect& rect )
{
	CComboBoxEx* pWndCombo = new CComboBoxEx;
	if (!pWndCombo->Create(m_dwStyle, rect, pWndParent, m_nID))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}

void CMFCToolBarComboBoxExButton::AdjustRect()
{
	if (m_pWndCombo->GetSafeHwnd() == NULL || m_rect.IsRectEmpty() || !m_bHorz)
	{
		m_rectCombo.SetRectEmpty();
		m_rectButton.SetRectEmpty();
		return;
	}

	CRect rectParent;
	m_pWndCombo->SetWindowPos(NULL, m_rect.left + nHorzMargin, m_rect.top, m_rect.Width() - 2 * nHorzMargin, m_nDropDownHeight, SWP_NOZORDER | SWP_NOACTIVATE);

	{
		CRect rect;
		m_pWndCombo->GetWindowRect(&m_rectCombo);
		m_pWndCombo->ScreenToClient(&m_rectCombo);
		m_pWndCombo->MapWindowPoints(m_pWndCombo->GetParent(), &m_rectCombo);
	}

	m_rectButton.SetRectEmpty();
}

BOOL CMFCToolBarComboBoxExButton::OnClick( CWnd* pWnd, BOOL bDelay /*= TRUE*/ )
{
	if (m_pWndCombo == NULL || m_pWndCombo->GetSafeHwnd() == NULL || !m_bHorz)
	{
		return FALSE;
	}

	return TRUE;
}

void CMFCToolBarComboBoxExButton::OnMove()
{
	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		AdjustRect();
	}
}

void CMFCToolBarComboBoxExButton::OnSize( int iSize )
{
	m_iWidth = iSize;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		AdjustRect();
	}
}

BOOL CMFCToolBarComboBoxExButton::NotifyCommand( int iNotifyCode )
{
	if (m_pWndCombo->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	switch (iNotifyCode)
	{
	case CBN_SELENDOK:
		{
			m_iSelIndex = m_pWndCombo->GetCurSel();
			if (m_iSelIndex < 0)
			{
				return FALSE;
			}
		}

		return TRUE;

	case CBN_KILLFOCUS:
	case CBN_EDITUPDATE:
		return TRUE;

	case CBN_SETFOCUS:
		return TRUE;

	case CBN_SELCHANGE: // yurig: process selchange
		return TRUE;

	case CBN_EDITCHANGE:
		return TRUE;
	}

	return FALSE;

}

void CMFCToolBarComboBoxExButton::OnShow( BOOL bShow )
{
	if (m_pWndCombo->GetSafeHwnd() != NULL)
	{
		if (bShow && m_bHorz)
		{
			OnMove();
			m_pWndCombo->ShowWindow(SW_SHOWNOACTIVATE);
		}
		else
		{
			m_pWndCombo->ShowWindow(SW_HIDE);
		}
	}
}

BOOL CMFCToolBarComboBoxExButton::HasFocus() const
{
	if (m_pWndCombo == NULL)
	{
		return FALSE;
	}

	CWnd* pWndFocus = CWnd::GetFocus();

	if (m_pWndCombo->GetDroppedState() || pWndFocus == m_pWndCombo || m_pWndCombo->IsChild(pWndFocus))
	{
		return TRUE;
	}

	return FALSE;
}

void CMFCToolBarComboBoxExButton::CopyFrom( const CMFCToolBarButton& s )
{
	CMFCToolBarButton::CopyFrom(s);
	POSITION pos;

	m_lstItems.RemoveAll();

	const CMFCToolBarComboBoxExButton& src = (const CMFCToolBarComboBoxExButton&) s;
	for (pos = src.m_lstItems.GetHeadPosition(); pos != NULL;)
	{
		m_lstItems.AddTail(src.m_lstItems.GetNext(pos));
	}

	m_dwStyle = src.m_dwStyle;
	m_iWidth = src.m_iWidth;
	m_iSelIndex = src.m_iSelIndex;
	m_nDropDownHeight = src.m_nDropDownHeight;
	m_pImageList = src.m_pImageList;
}

SIZE CMFCToolBarComboBoxExButton::OnCalculateSize( CDC* pDC, const CSize& sizeDefault, BOOL bHorz )
{
	m_bHorz = bHorz;
	m_sizeText = CSize(0, 0);

	if (!IsVisible())
	{

		if (m_pWndCombo->GetSafeHwnd() != NULL &&
			(m_pWndCombo->GetStyle() & WS_VISIBLE))
		{
			m_pWndCombo->ShowWindow(SW_HIDE);
		}

		return CSize(0,0);
	}

	if (bHorz)
	{
		int cy = sizeDefault.cy;

		if (m_pWndCombo != NULL && m_pWndCombo->GetSafeHwnd() != NULL)
		{
			CRect rectCombo;
			m_pWndCombo->GetWindowRect(&rectCombo);

			cy = rectCombo.Height();
		}

		return CSize(m_iWidth, cy + m_sizeText.cy);

	}
	else
	{
		if (m_pWndCombo->GetSafeHwnd() != NULL && (m_pWndCombo->GetStyle() & WS_VISIBLE))
		{
			m_pWndCombo->ShowWindow(SW_HIDE);
		}

		return CMFCToolBarButton::OnCalculateSize(pDC, sizeDefault, bHorz);
	}

}

CMFCToolBarComboBoxExButton* __stdcall CMFCToolBarComboBoxExButton::GetByCmd( UINT uiCmd, BOOL bIsFocus /*= FALSE*/ )
{
	CMFCToolBarComboBoxExButton* pSrcCombo = NULL;

	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(uiCmd, listButtons) > 0)
	{
		for (POSITION posCombo= listButtons.GetHeadPosition(); posCombo != NULL;)
		{
			CMFCToolBarComboBoxExButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxExButton, listButtons.GetNext(posCombo));
			ENSURE(pCombo != NULL);

			if (pCombo != NULL &&(!bIsFocus || pCombo->HasFocus()))
			{
				pSrcCombo = pCombo;
				break;
			}
		}
	}

	return pSrcCombo;
}