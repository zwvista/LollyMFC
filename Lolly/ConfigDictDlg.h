#pragma once
#include "afxcmn.h"


// CConfigDictDlg ダイアログ

class CConfigDictDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDictDlg)

public:
	CConfigDictDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CConfigDictDlg();

// ダイアログ データ
	enum { IDD = IDD_CONFIG_DICT };

    const CDictLangConfig* m_pConfig;
    vector<shared_ptr<CUIDict>> m_vpUIDicts;

protected:
    CWndResizer m_resizer;
    map<HTREEITEM, CString> m_mapItem2Key;

    HTREEITEM AddTreeNode(CTreeCtrl& wndTree, LPCTSTR lpszItem, const CString& strKey, int nImage, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
    HTREEITEM CloneTreeNode(CTreeCtrl& wndTree, HTREEITEM hItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
    void FillTreeA();
    void FillTreeB();
    void WithSelectedItem(function<HTREEITEM(HTREEITEM)> action);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CTreeCtrl m_treeA;
    CTreeCtrl m_treeB;
    afx_msg void OnBnClickedButtonAdd(UINT nID);
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonRemove();
    afx_msg void OnBnClickedButtonTop();
    afx_msg void OnBnClickedButtonUp();
    afx_msg void OnBnClickedButtonDown();
    afx_msg void OnBnClickedButtonBottom();
    afx_msg void OnClickTreeA(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBeginlabeleditTreeB(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();
};
