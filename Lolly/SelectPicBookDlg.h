#pragma once

#include "DataGrid.h"

// CSelectPicBookDlg dialog

class CSelectPicBookDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectPicBookDlg)

public:
	CSelectPicBookDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPicBookDlg();

// Dialog Data
	enum { IDD = IDD_SELECT_PICBOOK };

	CString GetBookName() {return m_rsBook.GetFieldValueAsString(_T("BOOKNAME"));}
	CString GetFileName() {return m_rsBook.GetFieldValueAsString(_T("FILENAME"));}
	int GetNumPages() {return m_rsBook.GetFieldValueAsInt(_T("NUMPAGES"));}
	int GetCurPage() {return m_rsBook.GetFieldValueAsInt(_T("CURPAGE"));}

protected:
	CDataGrid m_gdBook;
	CADORecordset2 m_rsBook;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
