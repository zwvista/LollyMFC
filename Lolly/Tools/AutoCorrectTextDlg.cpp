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

    m_resizer.Hook(this);
    m_resizer.SetShowResizeGrip(TRUE);
    CRect rc;
    GetClientRect(rc);
    m_resizer.SetMinimumSize(_T("_root"), rc.Size());

    m_resizer.CreateSplitContainer(_T("splitA"), IDC_LABEL1, IDC_EDIT_BEFORE);
    m_resizer.SetFixedPanel(_T("splitA"), 1);
    m_resizer.SetIsSplitterFixed(_T("splitA"), TRUE);
    m_resizer.CreateSplitContainer(_T("splitB"), IDC_LABEL2, IDC_EDIT_AFTER);
    m_resizer.SetFixedPanel(_T("splitB"), 1);
    m_resizer.SetIsSplitterFixed(_T("splitB"), TRUE);
    m_resizer.CreateSplitContainer(_T("splitMain"), _T("splitA"), _T("splitB"));
    m_resizer.SetIsSplitterFixed(_T("splitMain"), TRUE);
    m_resizer.SetAnchor(_T("splitMain"), ANCHOR_ALL);

    m_resizer.SetAnchor(IDC_LABEL3, ANCHOR_LEFT | ANCHOR_BOTTOM);
    m_resizer.SetAnchor(IDC_COMBO_LANG, ANCHOR_LEFT | ANCHOR_BOTTOM);
    m_resizer.SetAnchor(IDC_BUTTON_COPYTEXT, ANCHOR_LEFT | ANCHOR_BOTTOM);
    m_resizer.SetAnchor(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);

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
