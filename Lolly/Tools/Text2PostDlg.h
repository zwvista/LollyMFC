#pragma once
#include "afxwin.h"


// CText2PostDlg dialog

class CText2PostDlg : public CDialog
{
	DECLARE_DYNAMIC(CText2PostDlg)

public:
	CText2PostDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CText2PostDlg();

// Dialog Data
	enum { IDD = IDD_TEXT_TO_POST };

protected:
	CWndResizer m_resizer;
	CString m_strFmtPost, m_strFmtParagraph1, m_strFmtParagraph2;
	CEdit m_edtOrigText;
	CEdit m_edtNewPost;
	CComboBox m_cboParagraphEnd;

	CString GetPostStrFormat1();
	CString GetPostStrFormat2();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApplypostformat();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
};
