#pragma once


// CDataGridView view
#include "DataGrid.h"

class CDataGridView : public CFormView
{
public:
	enum{ IDD = IDD_DATAGRID_FORM };

protected:
	CDataGridView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDataGridView)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。

	CEdit* m_pedtGridItem;
	CDataGrid* m_pDataGrid;
    CWndResizer m_resizer;

public:
	virtual ~CDataGridView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


