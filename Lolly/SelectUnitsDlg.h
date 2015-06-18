#pragma once

// CSelectUnitsDlg dialog

class CSelectUnitsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectUnitsDlg)

public:
	CSelectUnitsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectUnitsDlg();

    BOOL m_bApplyActive;
	BOOL m_bApplyAll;
// Dialog Data
	enum { IDD = IDD_SELECT_UNITS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CADORecordset2 m_rsLang, m_rsBook;
	CComboBox m_cboLang;
	CComboBox m_cboBook;
	CEdit m_edtUnitFrom;
	CSpinButtonCtrl m_spnUnitFrom;
	CComboBox m_cboPartFrom;
	CStatic m_stcUnitInAllFrom;
	CEdit m_edtUnitTo;
	CSpinButtonCtrl m_spnUnitTo;
	CComboBox m_cboPartTo;
	CStatic m_stcUnitInAllTo;
	CButton m_chkTo;
	int m_nSelectedLangID;
	int m_nSelectedBookID;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLang();
	afx_msg void OnCbnSelchangeComboBook();
	afx_msg void OnDeltaposSpinUnitfrom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinUnitto(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTo();
	afx_msg void OnCbnSelchangeComboPartfrom();

	DECLARE_MESSAGE_MAP()
};
