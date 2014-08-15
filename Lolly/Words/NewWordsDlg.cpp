// NewWordsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "NewWordsDlg.h"
#include "afxdialogex.h"


// CNewWordsDlg dialog

IMPLEMENT_DYNAMIC(CNewWordsDlg, CDialog)

CNewWordsDlg::CNewWordsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewWordsDlg::IDD, pParent)
{

}

CNewWordsDlg::~CNewWordsDlg()
{
}

void CNewWordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WORDS, m_strWords);
}


BEGIN_MESSAGE_MAP(CNewWordsDlg, CDialog)
END_MESSAGE_MAP()


// CNewWordsDlg message handlers


BOOL CNewWordsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_resizer.Hook(this);
	m_resizer.SetShowResizeGrip(TRUE);
	CRect rc;
	GetClientRect(rc);
	CSize sz(rc.Size());
	m_resizer.SetMinimumSize(_T("_root"), sz);
	m_resizer.SetAnchor(IDC_EDIT_WORDS, ANCHOR_ALL);
	m_resizer.SetAnchor(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
