#pragma once


// CNewPhrasesDlg dialog

class CNewPhrasesDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewPhrasesDlg)

public:
	CNewPhrasesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewPhrasesDlg();

// Dialog Data
	enum { IDD = IDD_NEW_PHRASES };

protected:
	CWndResizer m_resizer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPhrasesAndTranslations;
	virtual BOOL OnInitDialog();
};
