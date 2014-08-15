// SelectLessonsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "SelectLessonsDlg.h"


// CSelectLessonsDlg dialog

IMPLEMENT_DYNAMIC(CSelectLessonsDlg, CDialog)

CSelectLessonsDlg::CSelectLessonsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectLessonsDlg::IDD, pParent)
	, m_rsLang(&theApp.m_db)
	, m_rsBook(&theApp.m_db)
	, m_bActiveInclude(TRUE)
{

}

CSelectLessonsDlg::~CSelectLessonsDlg()
{
}

void CSelectLessonsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANG, m_cboLang);
	DDX_Control(pDX, IDC_COMBO_BOOK, m_cboBook);
	DDX_Control(pDX, IDC_EDIT_LESSONFROM, m_edtLessonFrom);
	DDX_Control(pDX, IDC_SPIN_LESSONFROM, m_spnLessonFrom);
	DDX_Control(pDX, IDC_COMBO_PARTFROM, m_cboPartFrom);
	DDX_Control(pDX, IDC_STATIC_LESSONINALLFROM, m_stcLessonInAllFrom);
	DDX_Control(pDX, IDC_EDIT_LESSONTO, m_edtLessonTo);
	DDX_Control(pDX, IDC_SPIN_LESSONTO, m_spnLessonTo);
	DDX_Control(pDX, IDC_COMBO_PARTTO, m_cboPartTo);
	DDX_Control(pDX, IDC_STATIC_LESSONINALLTO, m_stcLessonInAllTo);
	DDX_Control(pDX, IDC_CHECK_TO, m_chkTo);
	DDX_Check(pDX, IDC_CHECK_ACTIVEINCLUDE, m_bActiveInclude);
}


BEGIN_MESSAGE_MAP(CSelectLessonsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG, &CSelectLessonsDlg::OnCbnSelchangeComboLang)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOK, &CSelectLessonsDlg::OnCbnSelchangeComboBook)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LESSONFROM, &CSelectLessonsDlg::OnDeltaposSpinLessonfrom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LESSONTO, &CSelectLessonsDlg::OnDeltaposSpinLessonto)
	ON_BN_CLICKED(IDOK, &CSelectLessonsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TO, &CSelectLessonsDlg::OnBnClickedCheckTo)
	ON_CBN_SELCHANGE(IDC_COMBO_PARTFROM, &CSelectLessonsDlg::OnCbnSelchangeComboPartfrom)
END_MESSAGE_MAP()


// CSelectLessonsDlg message handlers

BOOL CSelectLessonsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rsLang.Open(_T("SELECT * FROM LANGUAGES WHERE LANGID > 0 ORDER BY LANGID"));
	theApp.ComboBox_FillWithRS(m_cboLang, m_rsLang, _T("CHNNAME"), _T("LANGID"));
	theApp.ComboBox_SelectByValue(m_cboLang, theApp.m_lblSettings.nLangID);
	OnCbnSelchangeComboLang();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectLessonsDlg::OnCbnSelchangeComboLang()
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

void CSelectLessonsDlg::OnCbnSelchangeComboBook()
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

	int nNumLessons = m_rsBook.GetFieldValueAsInt(_T("NUMLESSONS"));
	str.Format(_T("(%d in all)"), nNumLessons);
	m_stcLessonInAllFrom.SetWindowText(str);
	m_stcLessonInAllTo.SetWindowText(str);
	m_spnLessonFrom.SetRange32(1, nNumLessons);
	m_spnLessonTo.SetRange32(1, nNumLessons);
	int nLessonFrom = m_rsBook.GetFieldValueAsInt(_T("LESSONFROM"));
	int nPartFrom = m_rsBook.GetFieldValueAsInt(_T("PARTFROM"));
	int nLessonTo = m_rsBook.GetFieldValueAsInt(_T("LESSONTO"));
	int nPartTo = m_rsBook.GetFieldValueAsInt(_T("PARTTO"));
	m_spnLessonFrom.SetPos32(nLessonFrom);
	m_cboPartFrom.SetCurSel(nPartFrom - 1);
	m_spnLessonTo.SetPos32(nLessonTo);
	m_cboPartTo.SetCurSel(nPartTo - 1);
	m_chkTo.SetCheck(nLessonFrom == nLessonTo && nPartFrom == nPartTo ? BST_UNCHECKED : BST_CHECKED);
	OnBnClickedCheckTo();
}

void CSelectLessonsDlg::OnDeltaposSpinLessonfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int iPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if(m_chkTo.GetCheck() != BST_CHECKED || iPos > m_spnLessonTo.GetPos32())
		m_spnLessonTo.SetPos32(iPos);
	*pResult = 0;
}

void CSelectLessonsDlg::OnDeltaposSpinLessonto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int iPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if(iPos < m_spnLessonFrom.GetPos32())
		m_spnLessonFrom.SetPos32(iPos);
	*pResult = 0;
}

void CSelectLessonsDlg::OnBnClickedOk()
{
	theApp.SetLangID(m_nSelectedLangID);
	m_rsLang.Edit();
	m_rsLang.SetFieldValue(_T("CURBOOKID"), m_nSelectedBookID);
	m_rsLang.Update();
	m_rsBook.Edit();
	m_rsBook.SetFieldValue(_T("LESSONFROM"), m_spnLessonFrom.GetPos32());
	m_rsBook.SetFieldValue(_T("PARTFROM"), m_cboPartFrom.GetCurSel() + 1);
	m_rsBook.SetFieldValue(_T("LESSONTO"), m_spnLessonTo.GetPos32());
	m_rsBook.SetFieldValue(_T("PARTTO"), m_cboPartTo.GetCurSel() + 1);
	m_rsBook.Update();
	OnOK();
}

void CSelectLessonsDlg::OnBnClickedCheckTo()
{
	bool bChecked = m_chkTo.GetCheck() == BST_CHECKED;
	m_edtLessonTo.EnableWindow(bChecked);
	m_spnLessonTo.EnableWindow(bChecked);
	m_stcLessonInAllTo.EnableWindow(bChecked);
	m_cboPartTo.EnableWindow(bChecked);
	if(!bChecked){
		m_spnLessonTo.SetPos32(m_spnLessonFrom.GetPos32());
		m_cboPartTo.SetCurSel(m_cboPartFrom.GetCurSel());
	}
}

void CSelectLessonsDlg::OnCbnSelchangeComboPartfrom()
{
	if(m_chkTo.GetCheck() != BST_CHECKED)
		m_cboPartTo.SetCurSel(m_cboPartFrom.GetCurSel());
}
