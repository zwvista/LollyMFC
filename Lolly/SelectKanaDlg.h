#pragma once
#include "afxwin.h"


// CSelectKanaDlg dialog

class CSelectKanaDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectKanaDlg)

public:
	CSelectKanaDlg(LPCTSTR pszWord, set<CString>& sstrKanas);   // standard constructor
	virtual ~CSelectKanaDlg();
	CString m_strSelectedKana;

// Dialog Data
	enum { IDD = IDD_SELECT_KANA };

protected:
	LPCTSTR m_pszWord;
	set<CString>& m_sstrKanas;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cboKanas;
protected:
	virtual void OnOK();
};
