// SelectKanaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "SelectKanaDlg.h"


// CSelectKanaDlg dialog

IMPLEMENT_DYNAMIC(CSelectKanaDlg, CDialog)

CSelectKanaDlg::CSelectKanaDlg(LPCTSTR pszWord, set<CString>& sstrKanas)
	: CDialog(CSelectKanaDlg::IDD, NULL)
	, m_pszWord(pszWord)
	, m_sstrKanas(sstrKanas)
{

}

CSelectKanaDlg::~CSelectKanaDlg()
{
}

void CSelectKanaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KANAS, m_cboKanas);
}


BEGIN_MESSAGE_MAP(CSelectKanaDlg, CDialog)
END_MESSAGE_MAP()


// CSelectKanaDlg message handlers

BOOL CSelectKanaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_WORD, m_pszWord);
	for(const CString& str : m_sstrKanas)
		m_cboKanas.AddString(str);
	m_cboKanas.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectKanaDlg::OnOK()
{
	m_cboKanas.GetLBText(m_cboKanas.GetCurSel(), m_strSelectedKana);

	CDialog::OnOK();
}
