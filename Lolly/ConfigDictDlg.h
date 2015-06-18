#pragma once
#include "afxcmn.h"


// CConfigDictDlg �_�C�A���O

class CConfigDictDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDictDlg)

public:
	CConfigDictDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CConfigDictDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CONFIG_DICT };

    const CDictLangConfig* m_pConfig;
    vector<CUIDict*> m_vpUIDicts;

protected:
    CWndResizer m_resizer;
    map<HTREEITEM, CString> m_mapItem2Key;

    HTREEITEM AddTreeNode(CTreeCtrl* pTree, LPCTSTR lpszItem, const CString& strKey, int nImage, HTREEITEM hParent = TVI_ROOT);
    void FillTreeA();
    void FillTreeB();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CTreeCtrl m_treeA;
    CTreeCtrl m_treeB;
    afx_msg void OnBnClickedButtonAdd(UINT nID);
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonRemove();
    afx_msg void OnBnClickedButtonTop();
};
