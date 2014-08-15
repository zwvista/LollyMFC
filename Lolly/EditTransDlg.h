#pragma once


// CEditTransDlg dialog

class CEditTransDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditTransDlg)

public:
	CEditTransDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTransDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_TRANS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTrans;
};
