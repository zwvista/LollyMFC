// ReorderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "ReorderDlg.h"
#include "afxdialogex.h"


// CReorderDlg dialog

IMPLEMENT_DYNAMIC(CReorderDlg, CDialog)

CReorderDlg::CReorderDlg(CADORecordset2& rs, CString& strItemFieldName, CWnd* pParent /*=NULL*/)
	: CDialog(CReorderDlg::IDD, pParent)
	, m_rs(rs)
	, m_strItemFieldName(strItemFieldName)
{

}

CReorderDlg::~CReorderDlg()
{
}

void CReorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_lstItems);
}


BEGIN_MESSAGE_MAP(CReorderDlg, CDialog)
END_MESSAGE_MAP()


// CReorderDlg message handlers


BOOL CReorderDlg::OnInitDialog()
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


void CReorderDlg::OnOK()
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
