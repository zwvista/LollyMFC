#pragma once


// CNewWordsDlg dialog

class CNewWordsDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewWordsDlg)

public:
	CNewWordsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewWordsDlg();

// Dialog Data
	enum { IDD = IDD_NEW_WORDS };

protected:
	CWndResizer m_resizer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strWords;
	virtual BOOL OnInitDialog();
};
