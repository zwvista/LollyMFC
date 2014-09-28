// AutoCorrectTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "AutoCorrectTextDlg.h"
#include "afxdialogex.h"


// CAutoCorrectTextDlg dialog

IMPLEMENT_DYNAMIC(CAutoCorrectTextDlg, CDialog)

CAutoCorrectTextDlg::CAutoCorrectTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoCorrectTextDlg::IDD, pParent)
	, m_rsLang(&theApp.m_db)
	, m_rsAutoCorrect(&theApp.m_db)
{

}

CAutoCorrectTextDlg::~CAutoCorrectTextDlg()
{
}

void CAutoCorrectTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANG, m_cboLang);
	DDX_Control(pDX, IDC_EDIT_BEFORE, m_edtBefore);
	DDX_Control(pDX, IDC_EDIT_AFTER, m_edtAfter);
}


BEGIN_MESSAGE_MAP(CAutoCorrectTextDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG, &CAutoCorrectTextDlg::OnCbnSelchangeComboLang)
	ON_EN_CHANGE(IDC_EDIT_BEFORE, &CAutoCorrectTextDlg::OnEnChangeEditBefore)
	ON_BN_CLICKED(IDC_BUTTON_COPYTEXT, &CAutoCorrectTextDlg::OnBnClickedButtonCopytext)
END_MESSAGE_MAP()


// CAutoCorrectTextDlg message handlers


BOOL CAutoCorrectTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rsLang.Open(_T("SELECT * FROM LANGUAGES WHERE LANGID > 0 ORDER BY LANGID"));
	theApp.ComboBox_FillWithRS(m_cboLang, m_rsLang, _T("CHNNAME"), _T("LANGID"));
	theApp.ComboBox_SelectByValue(m_cboLang, theApp.m_lbuSettings.nLangID);
	OnCbnSelchangeComboLang();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoCorrectTextDlg::OnCbnSelchangeComboLang()
{
	int nSelectedLangID = m_cboLang.GetItemData(m_cboLang.GetCurSel());
	CString sql;
	sql.Format(_T("SELECT * FROM AUTOCORRECT WHERE LANGID=%d"), nSelectedLangID);
	m_rsAutoCorrect.Open(sql);
	OnEnChangeEditBefore();
}

void CAutoCorrectTextDlg::OnEnChangeEditBefore()
{
	m_edtBefore.GetWindowText(m_strText);
	m_strText = theApp.AutoCorrect(m_strText, m_rsAutoCorrect);
	m_edtAfter.SetWindowText(m_strText);
}

void CAutoCorrectTextDlg::OnBnClickedButtonCopytext()
{
	CopyTextToClipBoard(m_strText);
}
