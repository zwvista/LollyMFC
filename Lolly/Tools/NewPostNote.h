#pragma once


// CNewPostNoteDlg dialog

class CNewPostNoteDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewPostNoteDlg)

public:
	CNewPostNoteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewPostNoteDlg();

// Dialog Data
	enum { IDD = IDD_NEW_POST_NOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
