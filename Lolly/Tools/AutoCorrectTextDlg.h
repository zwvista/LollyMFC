#pragma once


// CAutoCorrectTextDlg dialog

class CAutoCorrectTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoCorrectTextDlg)

public:
	CAutoCorrectTextDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoCorrectTextDlg();

// Dialog Data
	enum { IDD = IDD_AUTOCORRECT_TEXT };

protected:
	CADORecordset2 m_rsLang, m_rsAutoCorrect;
	CComboBox m_cboLang;
	CEdit m_edtBefore, m_edtAfter;
	CString m_strText;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboLang();
	afx_msg void OnEnChangeEditBefore();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCopytext();
};
