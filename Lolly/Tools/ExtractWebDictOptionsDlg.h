#pragma once

#include "ExtractWebDictGrid.h"
// CExtractWebDictOptionsDlg dialog

class CExtractWebDictOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CExtractWebDictOptionsDlg)

public:
	CExtractWebDictOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExtractWebDictOptionsDlg();

// Dialog Data
	enum { IDD = IDD_EXTRACT_WEB_DICT_OPTIONS };

	int m_nLangID;
	vector<CString> m_vSelectedWords;
	vector<CString> m_vSelectedDicts;
	BOOL m_bOverwriteDB;

protected:
	CExtractWebDictGrid m_gdWord, m_gdDict;
	CADORecordset2 m_rsWord, m_rsDict;

	void GetAllChecked(CExtractWebDictGrid& grid, vector<CString>& vstr);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnWordsChanged(UINT nID);
	afx_msg void OnCheckSelected(UINT nID);
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};
