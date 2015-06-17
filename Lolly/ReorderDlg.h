#pragma once

// CReorderDlg dialog

#include "DragAndDropListBox.h"

class CReorderDlg : public CDialog
{
	DECLARE_DYNAMIC(CReorderDlg)

public:
	CReorderDlg(CADORecordset2& rs, CString& strItemFieldName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReorderDlg();

// Dialog Data
	enum { IDD = IDD_REORDER };

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
