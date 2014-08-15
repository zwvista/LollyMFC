// EditTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "EditTransDlg.h"


// CEditTransDlg dialog

IMPLEMENT_DYNAMIC(CEditTransDlg, CDialog)

CEditTransDlg::CEditTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTransDlg::IDD, pParent)
	, m_strTrans(_T(""))
{

}

CEditTransDlg::~CEditTransDlg()
{
}

void CEditTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TRANS, m_strTrans);
}


BEGIN_MESSAGE_MAP(CEditTransDlg, CDialog)
END_MESSAGE_MAP()


// CEditTransDlg message handlers
