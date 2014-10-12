// ReindexDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "ReindexDlg.h"
#include "afxdialogex.h"


// CReindexDlg dialog

IMPLEMENT_DYNAMIC(CReindexDlg, CDialog)

CReindexDlg::CReindexDlg(CADORecordset2& rs, CString& strItemFieldName, CWnd* pParent /*=NULL*/)
	: CDialog(CReindexDlg::IDD, pParent)
	, m_rs(rs)
	, m_strItemFieldName(strItemFieldName)
{

}

CReindexDlg::~CReindexDlg()
{
}

void CReindexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_lstItems);
}


BEGIN_MESSAGE_MAP(CReindexDlg, CDialog)
END_MESSAGE_MAP()


// CReindexDlg message handlers


BOOL CReindexDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_resizer.Hook(this);
	m_resizer.SetShowResizeGrip(TRUE);
	CRect rc;
	GetClientRect(rc);
	m_resizer.SetMinimumSize(_T("_root"), rc.Size());
	m_resizer.SetAnchor(IDC_LIST_ITEMS, ANCHOR_ALL);
	m_resizer.SetAnchor(IDOK, ANCHOR_RIGHT | ANCHOR_TOP);
	m_resizer.SetAnchor(IDCANCEL, ANCHOR_RIGHT | ANCHOR_TOP);

	int i = 0;
	for(m_rs.MoveFirst(); !m_rs.IsEof(); m_rs.MoveNext()){
		if(m_rs.GetFieldValueAsInt(_T("ID")) == 0) continue;
		m_lstItems.AddString(m_rs.GetFieldValueAsString(m_strItemFieldName));
		m_lstItems.SetItemData(m_lstItems.GetCount() - 1, i++);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CReindexDlg::OnOK()
{
	for(int i = 0, count = m_lstItems.GetCount(); i < count; ++i){
		int n = m_lstItems.GetItemData(i);
		m_rs.Move(n, CADORecordset2::bmFirst);
		m_rs.Edit();
		m_rs.SetFieldValue(_T("ORD"), i + 1);
		m_rs.Update();
	}

	CDialog::OnOK();
}
