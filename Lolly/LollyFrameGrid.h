
// BooksFrm.h : interface of the CLollyFrameGrid class
//

#pragma once

#include "DataGrid.h"

class CLollyFrameGrid : public CLollyFrame, protected CADORecordsetEventHandler
{
	DECLARE_DYNCREATE(CLollyFrameGrid)
public:
	CLollyFrameGrid();

// Attributes
public:

// Operations
public:

// Overrides
public:

// Implementation
public:
	virtual ~CLollyFrameGrid();
	CEdit m_edtGridItem;
	CDataGrid m_wndGrid;

// Generated message map functions
protected:
	SLangBookUnitSettings m_lbuSettings;
	CADORecordset2 m_rs;
	CADORecordset2 m_rsAutoCorrect;
	CMFCToolBarEditBoxButton2 *m_pedtRecordPos;
	int m_nRecordPos, m_nRecordCount;
	int m_nFilterScope;
	CString m_strFilter;
	bool m_bHasFilter, m_bHasMultiAdd, m_bHasReorder;
	bool m_bTimerNavigate, m_bIsReordering;
	CMFCToolBarButton* m_pbtnNavigate;
	CString m_strReorderItemFieldName;
    const CDictLangConfig *m_pConfig, *m_pConfigChn;

	virtual CString GetSQL() {return CString();}
	virtual SDataGridColumnInfo* GetDataGridColumnInfo() {return NULL;}
	virtual SDataGridNewRecordInfo* GetDataGridNewRecordInfo() {return NULL;}
	virtual void OnGridItemEntered(const CString& strGridItem) {}

	virtual void MoveComplete(EventReasonEnum adReason, struct Error *pError, EventStatusEnum *adStatus, struct _Recordset *pRecordset);
	virtual void OnMoveComplete();
	virtual void LoadTables();

	void KeepGridRowWithRS();
	void CreateFilterCtrls();
	void DoAddNew(SDataGridNewRecordInfo* nri);
	void DoUpdateFilter();
	void SetTimerNavigate();
	void DoNavigate();

	afx_msg LRESULT OnLblSettingsChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMoveFirst();
	afx_msg void OnUpdateMoveFirst(CCmdUI* pCmdUI);
	afx_msg void OnMovePrevious();
	afx_msg void OnUpdateMovePrevious(CCmdUI* pCmdUI);
	afx_msg void OnRecordPos();
	afx_msg void OnMoveNext();
	afx_msg void OnUpdateMoveNext(CCmdUI* pCmdUI);
	afx_msg void OnMoveLast();
	afx_msg void OnUpdateMoveLast(CCmdUI* pCmdUI);
	afx_msg void OnAddNew();
	afx_msg void OnUpdateAddNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMultiAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnRefresh();
	afx_msg void OnTimerNavigate();
	afx_msg void OnUpdateTimerNavigate(CCmdUI* pCmdUI);
	afx_msg void OnNavigate(UINT nID);
	afx_msg void OnFilterSet(); 
	afx_msg void OnFilterRemove(); 
	afx_msg void OnUpdateFilter(CCmdUI* pCmdUI);
	afx_msg void OnReorder(); 
	afx_msg void OnUpdateReorder(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
