// SelectPicBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "SelectPicBookDlg.h"


// CSelectPicBookDlg dialog

IMPLEMENT_DYNAMIC(CSelectPicBookDlg, CDialog)

CSelectPicBookDlg::CSelectPicBookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPicBookDlg::IDD, pParent)
	, m_rsBook(&theApp.m_db)
{

}

CSelectPicBookDlg::~CSelectPicBookDlg()
{
}

void CSelectPicBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectPicBookDlg, CDialog)
END_MESSAGE_MAP()


// CSelectPicBookDlg message handlers

BOOL CSelectPicBookDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_gdBook.AttachGrid(this, IDC_GRID_BOOK);
	m_gdBook.SetAllowAddNew(FALSE);
	m_gdBook.SetHighlightRow(TRUE);
	m_gdBook.SetSH_Width(0);

	static SDataGridColumnInfo ci[] = {
		{ _T("BOOKNAME"), _T("BOOKNAME"), NULL, 200, 0, FALSE },
		{ _T("FILENAME"), _T("FILENAME"), NULL, 800, 0, FALSE },
		{ _T("NUMPAGES"), _T("NUMPAGES"), NULL, 100, 0, FALSE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	CString sql;
	sql.Format(_T("SELECT * FROM PICBOOKS WHERE LANGID=%d"), theApp.m_lbuSettings.nLangID);
	m_gdBook.BindData(&m_rsBook, sql, ci);
	m_gdBook.ResizeCols();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
