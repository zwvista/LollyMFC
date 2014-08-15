#pragma once

// CSelectLessonsDlg dialog

class CSelectLessonsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectLessonsDlg)

public:
	CSelectLessonsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectLessonsDlg();

	BOOL m_bActiveInclude;
// Dialog Data
	enum { IDD = IDD_SELECT_LESSONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CADORecordset2 m_rsLang, m_rsBook;
	CComboBox m_cboLang;
	CComboBox m_cboBook;
	CEdit m_edtLessonFrom;
	CSpinButtonCtrl m_spnLessonFrom;
	CComboBox m_cboPartFrom;
	CStatic m_stcLessonInAllFrom;
	CEdit m_edtLessonTo;
	CSpinButtonCtrl m_spnLessonTo;
	CComboBox m_cboPartTo;
	CStatic m_stcLessonInAllTo;
	CButton m_chkTo;
	int m_nSelectedLangID;
	int m_nSelectedBookID;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLang();
	afx_msg void OnCbnSelchangeComboBook();
	afx_msg void OnDeltaposSpinLessonfrom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinLessonto(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTo();
	afx_msg void OnCbnSelchangeComboPartfrom();

	DECLARE_MESSAGE_MAP()
};
