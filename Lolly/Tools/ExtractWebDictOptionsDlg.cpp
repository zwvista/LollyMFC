// ExtractWebDictOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "ExtractWebDictOptionsDlg.h"


// CExtractWebDictOptionsDlg dialog

IMPLEMENT_DYNAMIC(CExtractWebDictOptionsDlg, CDialog)

CExtractWebDictOptionsDlg::CExtractWebDictOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtractWebDictOptionsDlg::IDD, pParent)
	, m_nLangID(theApp.m_lbuSettings.nLangID)
	, m_bOverwriteDB(FALSE)
	, m_rsWord(&theApp.m_db)
	, m_rsDict(&theApp.m_db)
{

}

CExtractWebDictOptionsDlg::~CExtractWebDictOptionsDlg()
{
}

void CExtractWebDictOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_OVERWRITE, m_bOverwriteDB);
}


BEGIN_MESSAGE_MAP(CExtractWebDictOptionsDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BOOKUNITS, IDC_LANG, OnWordsChanged)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECKALLWORDS, IDC_UNCHECKSELECTEDDICTS, OnCheckSelected)
	ON_BN_CLICKED(IDOK, &CExtractWebDictOptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CExtractWebDictOptionsDlg message handlers

BOOL CExtractWebDictOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_BOOKUNITS, BST_CHECKED);
	SetDlgItemText(IDC_BOOKUNITS, theApp.m_lbuSettings.GetBookUnitsDesc());
	SetDlgItemText(IDC_LANG, theApp.m_lbuSettings.GetLangDesc());

	m_gdWord.AttachGrid(this, IDC_GRID_WORD);
	m_gdWord.SetAllowAddNew(FALSE);
	m_gdDict.AttachGrid(this, IDC_GRID_DICTNAME);
	m_gdDict.SetAllowAddNew(FALSE);

	OnWordsChanged(IDC_BOOKUNITS);

	static SDataGridColumnInfo ci[] = {
		{ _T("A"), _T(""), NULL, 30, 0, FALSE },
		{ _T("B"), _T("DICTNAME"), NULL, 150, 0, FALSE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	CString sql;
	sql.Format(_T("SELECT DICTNAME AS A, DICTNAME AS B FROM DICTALL WHERE LANGID=%d AND DICTTYPENAME='OFFLINE-ONLINE' ORDER BY DICTNAME"),
		theApp.m_lbuSettings.nLangID);
	m_gdDict.BindData(&m_rsDict, sql, ci);
	OnCheckSelected(IDC_CHECKALLDICTS);

	m_gdWord.ResizeCols();
	m_gdDict.ResizeCols();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExtractWebDictOptionsDlg::OnWordsChanged( UINT nID )
{
	static SDataGridColumnInfo ci[] = {
		{ _T("A"), _T(""), NULL, 30, 0, FALSE },
		{ _T("B"), _T("WORD"), NULL, 150, 0, FALSE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	CString sql;
	if(nID == IDC_BOOKUNITS)
		sql.Format(_T("SELECT WORD AS A, WORD AS B FROM WORDSBOOK WHERE BOOKID=%d AND UNIT*10+PART>=%d AND UNIT*10+PART<=%d"),
			theApp.m_lbuSettings.nBookID, theApp.m_lbuSettings.GetUnitPartFrom(), theApp.m_lbuSettings.GetUnitPartTo());
	else
		sql.Format(_T("SELECT WORD AS A, WORD AS B FROM WORDSLANG WHERE LANGID=%d"), theApp.m_lbuSettings.nLangID);
	sql += _T(" ORDER BY WORD");
	m_gdWord.BindData(&m_rsWord, sql, ci);
	OnCheckSelected(IDC_CHECKALLWORDS);
}

void CExtractWebDictOptionsDlg::OnCheckSelected( UINT nID )
{
	CExtractWebDictGrid& grid = nID < IDC_CHECKALLDICTS ? m_gdWord : m_gdDict;
	bool bNeedSelect = (nID - IDC_CHECKALLWORDS) % 4 > 1;
	bool bCheck = (nID - IDC_CHECKALLWORDS) % 2 == 0;
	CString str = bCheck ? _T("1") : _T("0");
	for(long i = 0; i < grid.GetNumberRows(); i++)
		if(!bNeedSelect || grid.IsSelected(0, i, NULL))
			grid.QuickSetText(0, i, str);
	grid.RedrawAll();
}

void CExtractWebDictOptionsDlg::GetAllChecked( CExtractWebDictGrid& grid, vector<CString>& vstr )
{
	for(long i = 0; i < grid.GetNumberRows(); i++){
		CString str = grid.QuickGetText(0, i);
		if(str == _T("1"))
			vstr.push_back(grid.QuickGetText(1, i));
	}
}

void CExtractWebDictOptionsDlg::OnBnClickedOk()
{
	GetAllChecked(m_gdWord, m_vSelectedWords);
	GetAllChecked(m_gdDict, m_vSelectedDicts);
	if(m_vSelectedWords.empty() || m_vSelectedDicts.empty())
		OnCancel();
	else
		OnOK();
}
