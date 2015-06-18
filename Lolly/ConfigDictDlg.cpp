// ConfigDictDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Lolly.h"
#include "ConfigDictDlg.h"
#include "afxdialogex.h"


// CConfigDictDlg ダイアログ

IMPLEMENT_DYNAMIC(CConfigDictDlg, CDialog)

CConfigDictDlg::CConfigDictDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDictDlg::IDD, pParent)
{

}

CConfigDictDlg::~CConfigDictDlg()
{
}

void CConfigDictDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_A, m_treeA);
    DDX_Control(pDX, IDC_TREE_B, m_treeB);
}


BEGIN_MESSAGE_MAP(CConfigDictDlg, CDialog)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_ADD_PILE, IDC_BUTTON_ADD_ALL, &CConfigDictDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CConfigDictDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CConfigDictDlg::OnBnClickedButtonRemove)
    ON_BN_CLICKED(IDC_BUTTON_TOP, &CConfigDictDlg::OnBnClickedButtonTop)
END_MESSAGE_MAP()


// CConfigDictDlg メッセージ ハンドラー


BOOL CConfigDictDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_resizer.Hook(this);
    m_resizer.SetShowResizeGrip(TRUE);
    CRect rc;
    GetClientRect(rc);
    m_resizer.SetMinimumSize(_T("_root"), rc.Size());

    CUIntArray arr;
    for(UINT i = IDC_BUTTON_ADD_PILE; i <= IDC_BUTTON_BOTTOM; ++i)
        arr.Add(i);
    m_resizer.CreatePanel(_T("PanelA"), &arr, TRUE);
    m_resizer.CreateSplitContainer(_T("splitA"), IDC_TREE_A, _T("PanelA"));
    m_resizer.SetAnchor(_T("splitA"), ANCHOR_TOP | ANCHOR_RIGHT);
    m_resizer.SetFixedPanel(_T("splitA"), 2);
    m_resizer.SetIsSplitterFixed(_T("splitA"), TRUE);

    arr.RemoveAll();
    arr.Add(IDOK); arr.Add(IDCANCEL);
    m_resizer.CreatePanel(_T("PanelB"), &arr, TRUE);
    m_resizer.CreateSplitContainer(_T("splitB"), IDC_TREE_B, _T("PanelB"));
    m_resizer.SetAnchor(_T("splitB"), ANCHOR_TOP | ANCHOR_RIGHT);
    m_resizer.SetFixedPanel(_T("splitB"), 2);
    m_resizer.SetIsSplitterFixed(_T("splitB"), TRUE);

    m_resizer.CreateSplitContainer(_T("splitMain"), _T("splitA"), _T("splitB"));
    m_resizer.SetIsSplitterFixed(_T("splitMain"), TRUE);

    m_resizer.SetAnchor(_T("splitMain"), ANCHOR_ALL);

    m_treeA.SetImageList(&theApp.m_imglstDictTypes, TVSIL_NORMAL);
    m_treeB.SetImageList(&theApp.m_imglstDictTypes, TVSIL_NORMAL);
    FillTreeA();
    FillTreeB();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

HTREEITEM CConfigDictDlg::AddTreeNode(CTreeCtrl* pTree, LPCTSTR lpszItem, const CString& strKey, int nImage, HTREEITEM hParent /*= TVI_ROOT*/)
{
    HTREEITEM hItem = pTree->InsertItem(lpszItem, nImage, nImage, hParent);
    m_mapItem2Key[hItem] = strKey;
    return hItem;
}

void CConfigDictDlg::FillTreeA()
{
    for(auto&& kv : m_pConfig->m_mapDictGroups){
        int nImageIndex = kv.second[0]->m_ImageIndex;
        if(nImageIndex == DICTIMAGE_OFFLINE || nImageIndex == DICTIMAGE_ONLINE || nImageIndex == DICTIMAGE_LIVE)
            --nImageIndex;
        auto hItem = AddTreeNode(&m_treeA, kv.first, kv.first, nImageIndex);
        for(auto&& pUIDictItem : kv.second)
            AddTreeNode(&m_treeA, pUIDictItem->m_strName, kv.first, pUIDictItem->m_ImageIndex, hItem);
        m_treeA.Expand(hItem, TVE_EXPAND);
    }
    m_treeA.EnsureVisible(m_treeA.GetChildItem(TVI_ROOT));
}

void CConfigDictDlg::FillTreeB()
{
    for(auto&& pUIDict : m_vpUIDicts){
        if(auto pUIDictItem = dynamic_cast<CUIDictItem*>(pUIDict))
            AddTreeNode(&m_treeB, pUIDictItem->m_strName, pUIDictItem->m_strType, pUIDictItem->m_ImageIndex);
        else{
            auto pUIDictCol = dynamic_cast<CUIDictCollection*>(pUIDict);
            auto hItem = AddTreeNode(&m_treeB, pUIDictCol->m_strName,
                CString(pUIDictCol->IsPile() ? _T("Pile") : _T("Switch")), pUIDictCol->m_ImageIndex);
            for(auto&& pUIDictItem : pUIDictCol->m_vpUIDictItems)
                AddTreeNode(&m_treeB, pUIDictItem->m_strName, pUIDictItem->m_strType, pUIDictItem->m_ImageIndex, hItem);
            m_treeB.Expand(hItem, TVE_EXPAND);
        }
    }
}


void CConfigDictDlg::OnBnClickedButtonAdd(UINT nID)
{
    vector<HTREEITEM> vhCheckedItems;
    for(auto hGroup = m_treeA.GetChildItem(TVI_ROOT); hGroup; hGroup = m_treeA.GetNextSiblingItem(hGroup))
        for(auto hDict = m_treeA.GetChildItem(hGroup); hDict; hDict = m_treeA.GetNextSiblingItem(hDict))
            if(m_treeA.GetCheck(hDict))
                vhCheckedItems.push_back(hDict);
    if(vhCheckedItems.empty()) return;

    auto f = [&](HTREEITEM hItem){
        int nImage, nSelectedImage;
        m_treeA.GetItemImage(hItem, nImage, nSelectedImage);
        AddTreeNode(&m_treeB, m_treeA.GetItemText(hItem), m_mapItem2Key.at(hItem), nImage);
    };
    if(vhCheckedItems.size() == 1 || nID == IDC_BUTTON_ADD_ALL)
        for(auto hItem : vhCheckedItems)
            f(hItem);
    else{
        set<HTREEITEM> sethGroups;
        for(auto hItem : vhCheckedItems)
            sethGroups.insert(m_treeA.GetParentItem(hItem));
    }
}


void CConfigDictDlg::OnBnClickedButtonClear()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CConfigDictDlg::OnBnClickedButtonRemove()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CConfigDictDlg::OnBnClickedButtonTop()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
}
