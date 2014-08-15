// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg dialog

IMPLEMENT_DYNAMIC(CFilterDlg, CDialog)

CFilterDlg::CFilterDlg(CADORecordset2& rsAutoCorrect, CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDlg::IDD, pParent)
	, m_rsAutoCorrect(rsAutoCorrect)
	, m_nScope(0)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_FILTER, m_strFilter);
	DDX_Control(pDX, IDC_COMBO_FILTER, m_cboFilter);
	DDX_CBIndex(pDX, IDC_COMBO_SCOPE, m_nScope);
	DDX_Control(pDX, IDC_COMBO_SCOPE, m_cboScope);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
END_MESSAGE_MAP()


// CFilterDlg message handlers


BOOL CFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cboScope.AddString(_T("Words/Phrases"));
	m_cboScope.AddString(_T("Translations"));
	m_cboScope.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDlg::OnOK()
{
	CDialog::OnOK();
	m_strFilter = DoubleApostrophe(theApp.AutoCorrect(m_strFilter, m_rsAutoCorrect));
}
