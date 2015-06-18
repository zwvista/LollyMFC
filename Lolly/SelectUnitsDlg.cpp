// SelectUnitsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "SelectUnitsDlg.h"


// CSelectUnitsDlg dialog

IMPLEMENT_DYNAMIC(CSelectUnitsDlg, CDialog)

CSelectUnitsDlg::CSelectUnitsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectUnitsDlg::IDD, pParent)
	, m_rsLang(&theApp.m_db)
	, m_rsBook(&theApp.m_db)
    , m_bApplyActive(TRUE)
	, m_bApplyAll(TRUE)
{

}

CSelectUnitsDlg::~CSelectUnitsDlg()
{
}

void CSelectUnitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANG, m_cboLang);
	DDX_Control(pDX, IDC_COMBO_BOOK, m_cboBook);
	DDX_Control(pDX, IDC_EDIT_UNITFROM, m_edtUnitFrom);
	DDX_Control(pDX, IDC_SPIN_UNITFROM, m_spnUnitFrom);
	DDX_Control(pDX, IDC_COMBO_PARTFROM, m_cboPartFrom);
	DDX_Control(pDX, IDC_STATIC_UNITINALLFROM, m_stcUnitInAllFrom);
	DDX_Control(pDX, IDC_EDIT_UNITTO, m_edtUnitTo);
	DDX_Control(pDX, IDC_SPIN_UNITTO, m_spnUnitTo);
	DDX_Control(pDX, IDC_COMBO_PARTTO, m_cboPartTo);
	DDX_Control(pDX, IDC_STATIC_UNITINALLTO, m_stcUnitInAllTo);
	DDX_Control(pDX, IDC_CHECK_TO, m_chkTo);
    DDX_Check(pDX, IDC_CHECK_APPLY_ACTIVE, m_bApplyActive);
	DDX_Check(pDX, IDC_CHECK_APPLY_ALL, m_bApplyAll);
}


BEGIN_MESSAGE_MAP(CSelectUnitsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG, &CSelectUnitsDlg::OnCbnSelchangeComboLang)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOK, &CSelectUnitsDlg::OnCbnSelchangeComboBook)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UNITFROM, &CSelectUnitsDlg::OnDeltaposSpinUnitfrom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UNITTO, &CSelectUnitsDlg::OnDeltaposSpinUnitto)
	ON_BN_CLICKED(IDOK, &CSelectUnitsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TO, &CSelectUnitsDlg::OnBnClickedCheckTo)
	ON_CBN_SELCHANGE(IDC_COMBO_PARTFROM, &CSelectUnitsDlg::OnCbnSelchangeComboPartfrom)
END_MESSAGE_MAP()


// CSelectUnitsDlg message handlers

BOOL CSelectUnitsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rsLang.Open(_T("SELECT * FROM LANGUAGES WHERE LANGID > 0 ORDER BY LANGID"));
	theApp.ComboBox_FillWithRS(m_cboLang, m_rsLang, _T("CHNNAME"), _T("LANGID"));
	theApp.ComboBox_SelectByValue(m_cboLang, theApp.m_lbuSettings.nLangID);
	OnCbnSelchangeComboLang();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectUnitsDlg::OnCbnSelchangeComboLang()
{
	m_nSelectedLangID = m_cboLang.GetItemData(m_cboLang.GetCurSel());
	CString sql;
	sql.Format(_T("SELECT * FROM BOOKS WHERE LANGID=%d ORDER BY BOOKID"), m_nSelectedLangID);
	m_rsBook.Open(sql);
	m_cboBook.ResetContent();
	theApp.ComboBox_FillWithRS(m_cboBook, m_rsBook, _T("BOOKNAME"), _T("BOOKID"));
	CString str;
	str.Format(_T("LANGID=%d"), m_nSelectedLangID);
	m_rsLang.FindFirst(str);
	int nCurBookID = m_rsLang.GetFieldValueAsInt(_T("CURBOOKID"));
	theApp.ComboBox_SelectByValue(m_cboBook, nCurBookID);
	OnCbnSelchangeComboBook();
}

void CSelectUnitsDlg::OnCbnSelchangeComboBook()
{
	m_nSelectedBookID = m_cboBook.GetItemData(m_cboBook.GetCurSel());
	CString str;
	str.Format(_T("BOOKID=%d"), m_nSelectedBookID);
	m_rsBook.FindFirst(str);

	m_cboPartFrom.ResetContent();
	m_cboPartTo.ResetContent();
	CString strParts = m_rsBook.GetFieldValueAsString(_T("PARTS"));
	for(int curPos = 0;;){
		CString str = strParts.Tokenize(_T(" "), curPos);
		if(str.IsEmpty()) break;
		m_cboPartFrom.AddString(str);
		m_cboPartTo.AddString(str);
	}

	int nNumUnits = m_rsBook.GetFieldValueAsInt(_T("UNITSINBOOK"));
	str.Format(_T("(%d in all)"), nNumUnits);
	m_stcUnitInAllFrom.SetWindowText(str);
	m_stcUnitInAllTo.SetWindowText(str);
	m_spnUnitFrom.SetRange32(1, nNumUnits);
	m_spnUnitTo.SetRange32(1, nNumUnits);
	int nUnitFrom = m_rsBook.GetFieldValueAsInt(_T("UNITFROM"));
	int nPartFrom = m_rsBook.GetFieldValueAsInt(_T("PARTFROM"));
	int nUnitTo = m_rsBook.GetFieldValueAsInt(_T("UNITTO"));
	int nPartTo = m_rsBook.GetFieldValueAsInt(_T("PARTTO"));
	m_spnUnitFrom.SetPos32(nUnitFrom);
	m_cboPartFrom.SetCurSel(nPartFrom - 1);
	m_spnUnitTo.SetPos32(nUnitTo);
	m_cboPartTo.SetCurSel(nPartTo - 1);
	m_chkTo.SetCheck(nUnitFrom == nUnitTo && nPartFrom == nPartTo ? BST_UNCHECKED : BST_CHECKED);
	OnBnClickedCheckTo();
}

void CSelectUnitsDlg::OnDeltaposSpinUnitfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int iPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if(m_chkTo.GetCheck() != BST_CHECKED || iPos > m_spnUnitTo.GetPos32())
		m_spnUnitTo.SetPos32(iPos);
	*pResult = 0;
}

void CSelectUnitsDlg::OnDeltaposSpinUnitto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int iPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if(iPos < m_spnUnitFrom.GetPos32())
		m_spnUnitFrom.SetPos32(iPos);
	*pResult = 0;
}

void CSelectUnitsDlg::OnBnClickedOk()
{
	theApp.SetLangID(m_nSelectedLangID);
	m_rsLang.Edit();
	m_rsLang.SetFieldValue(_T("CURBOOKID"), m_nSelectedBookID);
	m_rsLang.Update();
	m_rsBook.Edit();
	m_rsBook.SetFieldValue(_T("UNITFROM"), m_spnUnitFrom.GetPos32());
	m_rsBook.SetFieldValue(_T("PARTFROM"), m_cboPartFrom.GetCurSel() + 1);
	m_rsBook.SetFieldValue(_T("UNITTO"), m_spnUnitTo.GetPos32());
	m_rsBook.SetFieldValue(_T("PARTTO"), m_cboPartTo.GetCurSel() + 1);
	m_rsBook.Update();
	OnOK();
}

void CSelectUnitsDlg::OnBnClickedCheckTo()
{
	bool bChecked = m_chkTo.GetCheck() == BST_CHECKED;
	m_edtUnitTo.EnableWindow(bChecked);
	m_spnUnitTo.EnableWindow(bChecked);
	m_stcUnitInAllTo.EnableWindow(bChecked);
	m_cboPartTo.EnableWindow(bChecked);
	if(!bChecked){
		m_spnUnitTo.SetPos32(m_spnUnitFrom.GetPos32());
		m_cboPartTo.SetCurSel(m_cboPartFrom.GetCurSel());
	}
}

void CSelectUnitsDlg::OnCbnSelchangeComboPartfrom()
{
	if(m_chkTo.GetCheck() != BST_CHECKED)
		m_cboPartTo.SetCurSel(m_cboPartFrom.GetCurSel());
}
