#pragma once
#include "afxwin.h"


// CFilterDlg dialog

class CFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CADORecordset2& rsAutoCorrect, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterDlg();

// Dialog Data
	enum { IDD = IDD_FILTER };

	CString m_strFilter;
	int m_nScope;
    BOOL m_bMatchWholeWords;

protected:
	CComboBox m_cboFilter;
	CADORecordset2& m_rsAutoCorrect;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
