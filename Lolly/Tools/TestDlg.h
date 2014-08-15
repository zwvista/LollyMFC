#pragma once
#include "afxwin.h"


// CTestDlg dialog

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestDlg();

// Dialog Data
	enum { IDD = IDD_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUltralingua();
	afx_msg void OnBnClickedLingoes();
	CListBox m_lst;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedEbtest();
};
