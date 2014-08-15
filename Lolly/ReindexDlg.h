#pragma once

// CReindexDlg dialog

#include "DragAndDropListBox.h"

class CReindexDlg : public CDialog
{
	DECLARE_DYNAMIC(CReindexDlg)

public:
	CReindexDlg(CADORecordset2& rs, CString& strItemFieldName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReindexDlg();

// Dialog Data
	enum { IDD = IDD_REINDEX };

protected:
	CWndResizer m_resizer;
	CDragAndDropListBox m_lstItems;
	CADORecordset2& m_rs;
	CString& m_strItemFieldName;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
