// NewPhrasesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "NewPhrasesDlg.h"
#include "afxdialogex.h"


// CNewPhrasesDlg dialog

IMPLEMENT_DYNAMIC(CNewPhrasesDlg, CDialog)

CNewPhrasesDlg::CNewPhrasesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPhrasesDlg::IDD, pParent)
{

}

CNewPhrasesDlg::~CNewPhrasesDlg()
{
}

void CNewPhrasesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PHRASES_AND_TRANSLATIONS, m_strPhrasesAndTranslations);
}


BEGIN_MESSAGE_MAP(CNewPhrasesDlg, CDialog)
END_MESSAGE_MAP()


// CNewPhrasesDlg message handlers

BOOL CNewPhrasesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_resizer.Hook(this);
	m_resizer.SetShowResizeGrip(TRUE);
	CRect rc;
	GetClientRect(rc);
	CSize sz(rc.Size());
	m_resizer.SetMinimumSize(_T("_root"), sz);
	m_resizer.SetAnchor(m_strPhrasesAndTranslations, ANCHOR_ALL);
	m_resizer.SetAnchor(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
