#include "StdAfx.h"
#include "MFCToolBarButtons2.h"

IMPLEMENT_SERIAL(CMFCToolBarLabelButton, CMFCToolBarButton, 1)
IMPLEMENT_SERIAL(CMFCToolBarEditBoxButton2, CMFCToolBarEditBoxButton, 1)
IMPLEMENT_SERIAL(CMFCToolBarSpinEditBoxButton2, CMFCToolBarSpinEditBoxButton, 1)
IMPLEMENT_SERIAL(CMFCToolBarComboBoxButton2, CMFCToolBarComboBoxButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMFCToolBarLabelButton::CMFCToolBarLabelButton (UINT nID, LPCTSTR lpszText)
{
	if (lpszText != NULL)	
	{		
		m_strText = lpszText;	
	}	

	m_bText = TRUE;
	m_nID = nID;
	m_iImage = -1;
}

void CMFCToolBarLabelButton::OnDraw (CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, 
							  BOOL bHorz, BOOL /*bCustomizeMode*/, BOOL /*bHighlight*/, 
							  BOOL /*bDrawBorder*/,	BOOL /*bGrayDisabledButtons*/)
{
	UINT nStyle = m_nStyle;	
	m_nStyle &= ~TBBS_DISABLED;

	CMFCToolBarButton::OnDraw (pDC, rect, pImages, bHorz, FALSE,
								FALSE, FALSE, FALSE);

	m_nStyle = nStyle;
}

BOOL CMFCToolBarEditBoxButton2::NotifyCommand( int iNotifyCode )
{
	if (m_pWndEdit->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	switch (iNotifyCode)
	{
	case EN_UPDATE:
		return !m_bChangingText;
	}

	return FALSE;
}

BOOL CMFCToolBarSpinEditBoxButton2::NotifyCommand( int iNotifyCode )
{
	if (m_pWndEdit->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	switch (iNotifyCode)
	{
	case EN_UPDATE:
		return !m_bChangingText;
	}

	return FALSE;
}

BOOL CMFCToolBarComboBoxButton2::NotifyCommand( int iNotifyCode )
{
	if (m_pWndCombo->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	if (m_bFlat && iNotifyCode == 0)
	{
		return TRUE;
	}

	if (m_bFlat && m_pWndCombo->GetParent() != NULL)
	{
		m_pWndCombo->GetParent()->InvalidateRect(m_rectCombo);
		m_pWndCombo->GetParent()->UpdateWindow();
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

			m_pWndCombo->GetLBText(m_iSelIndex, m_strEdit);
			if (m_pWndEdit != NULL)
			{
				m_pWndEdit->SetWindowText(m_strEdit);
			}
		}

		if (m_pWndEdit != NULL)
		{
			m_pWndEdit->SetFocus();
		}

		return TRUE;

	case CBN_KILLFOCUS:
	case CBN_EDITUPDATE:
		return TRUE;

	case CBN_SETFOCUS:
		if (m_pWndEdit != NULL)
		{
			m_pWndEdit->SetFocus();
		}
		return TRUE;

	case CBN_SELCHANGE: // yurig: process selchange
		if (m_pWndEdit != NULL)
		{
			CString strEdit;
			m_pWndCombo->GetLBText(m_pWndCombo->GetCurSel(), strEdit);
			m_pWndEdit->SetWindowText(strEdit);
		}

		return TRUE;

	case CBN_EDITCHANGE:
		{
			m_pWndCombo->GetWindowText(m_strEdit);

			if (m_pWndEdit != NULL && m_pWndEdit->GetSafeHwnd() != NULL)
			{
				CString str;
				m_pWndEdit->GetWindowText(str);
				CComboBox* pBox = GetComboBox();
				if (pBox != NULL && pBox->GetSafeHwnd() != NULL)
				{
					int nCurSel = pBox->GetCurSel();
					int nNextSel = pBox->FindStringExact(nCurSel + 1, str);
					if (nNextSel == -1)
					{
						nNextSel = pBox->FindString(nCurSel + 1, str);
					}

					if (nNextSel != -1)
					{
						pBox->SetCurSel(nNextSel);
					}

					pBox->SetWindowText(str);
				}
			}
		}
		return TRUE;
	}

	return FALSE;

}