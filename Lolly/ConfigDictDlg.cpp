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
    ON_BN_CLICKED(IDC_BUTTON_UP, &CConfigDictDlg::OnBnClickedButtonUp)
    ON_BN_CLICKED(IDC_BUTTON_DOWN, &CConfigDictDlg::OnBnClickedButtonDown)
    ON_BN_CLICKED(IDC_BUTTON_BOTTOM, &CConfigDictDlg::OnBnClickedButtonBottom)
    ON_NOTIFY(NM_CLICK, IDC_TREE_A, &CConfigDictDlg::OnClickTreeA)
    ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE_B, &CConfigDictDlg::OnBeginlabeleditTreeB)
    ON_BN_CLICKED(IDOK, &CConfigDictDlg::OnBnClickedOk)
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

HTREEITEM CConfigDictDlg::AddTreeNode(CTreeCtrl& wndTree, LPCTSTR lpszItem, const CString& strKey, int nImage, HTREEITEM hParent /*= TVI_ROOT*/, HTREEITEM hInsertAfter /* = TVI_LAST */)
{
    HTREEITEM hItem = wndTree.InsertItem(lpszItem, nImage, nImage, hParent, hInsertAfter);
    m_mapItem2Key[hItem] = strKey;
    return hItem;
}

HTREEITEM CConfigDictDlg::CloneTreeNode(CTreeCtrl& tree, HTREEITEM hItem, HTREEITEM hParent /* = TVI_ROOT */, HTREEITEM hInsertAfter /* = TVI_LAST */)
{
    int nImage, nSelectedImage;
    tree.GetItemImage(hItem, nImage, nSelectedImage);
    auto hNewItem = AddTreeNode(m_treeB, tree.GetItemText(hItem), m_mapItem2Key.at(hItem), nImage, hParent, hInsertAfter);

    for(auto hItem2 = tree.GetChildItem(hItem); hItem2; hItem2 = tree.GetNextSiblingItem(hItem2)){
        tree.GetItemImage(hItem2, nImage, nSelectedImage);
        AddTreeNode(m_treeB, tree.GetItemText(hItem2), m_mapItem2Key.at(hItem2), nImage, hNewItem);
    }

    return hNewItem;
}

void CConfigDictDlg::FillTreeA()
{
    for(auto&& kv : m_pConfig->m_mapDictGroups){
        int nImageIndex = kv.second[0]->m_ImageIndex;
        if(nImageIndex == DICTIMAGE_OFFLINE || nImageIndex == DICTIMAGE_ONLINE || nImageIndex == DICTIMAGE_LIVE)
            --nImageIndex;
        auto hItem = AddTreeNode(m_treeA, kv.first, kv.first, nImageIndex);
        for(auto&& pUIDictItem : kv.second)
            AddTreeNode(m_treeA, pUIDictItem->m_strName, kv.first, pUIDictItem->m_ImageIndex, hItem);
        m_treeA.Expand(hItem, TVE_EXPAND);
    }
    m_treeA.EnsureVisible(m_treeA.GetChildItem(TVI_ROOT));
}

void CConfigDictDlg::FillTreeB()
{
    for(auto&& pUIDict : m_vpUIDicts){
        if(auto pUIDictItem = dynamic_cast<CUIDictItem*>(pUIDict.get()))
            AddTreeNode(m_treeB, pUIDictItem->m_strName, pUIDictItem->m_strType, pUIDictItem->m_ImageIndex);
        else{
            auto pUIDictCol = dynamic_cast<CUIDictCollection*>(pUIDict.get());
            auto hItem = AddTreeNode(m_treeB, pUIDictCol->m_strName,
                CString(pUIDictCol->IsPile() ? _T("Pile") : _T("Switch")), pUIDictCol->m_ImageIndex);
            for(auto&& pUIDictItem : pUIDictCol->m_vpUIDictItems)
                AddTreeNode(m_treeB, pUIDictItem->m_strName, pUIDictItem->m_strType, pUIDictItem->m_ImageIndex, hItem);
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

    if(vhCheckedItems.size() == 1 || nID == IDC_BUTTON_ADD_ALL)
        for(auto hItem : vhCheckedItems)
            CloneTreeNode(m_treeA, hItem, TVI_ROOT);
    else{
        set<HTREEITEM> sethGroups;
        for(auto hItem : vhCheckedItems)
            sethGroups.insert(m_treeA.GetParentItem(hItem));
        CString strType = nID == IDC_BUTTON_ADD_PILE ? _T("Pile") : _T("Switch");
        CString strName = strType;
        if(sethGroups.size() == 1)
            strName = m_treeA.GetItemText(*sethGroups.begin()) + _T("_") + strName;
        auto hParent = AddTreeNode(m_treeB, strName, strType, DICTIMAGE_CUSTOM);
        for(auto hItem : vhCheckedItems)
            CloneTreeNode(m_treeA, hItem, hParent);
    }
}


void CConfigDictDlg::OnBnClickedButtonClear()
{
    m_treeB.DeleteAllItems();
    FillTreeB();
}

void CConfigDictDlg::WithSelectedItem(function<HTREEITEM(HTREEITEM)> action)
{
    auto hItem = m_treeB.GetSelectedItem();
    if(hItem == NULL || m_treeB.GetParentItem(hItem) != NULL) return;
    auto hItem2 = action(hItem);
    if(hItem2 == NULL) return;
    auto hItem3 = CloneTreeNode(m_treeB, hItem, TVI_ROOT, hItem2);
    m_treeB.DeleteItem(hItem);
    m_treeB.SelectItem(hItem3);
}

void CConfigDictDlg::OnBnClickedButtonRemove()
{
    WithSelectedItem([&](HTREEITEM hItem)->HTREEITEM{
        m_treeB.DeleteItem(hItem);
        return NULL;
    });
}

void CConfigDictDlg::OnBnClickedButtonTop()
{
    WithSelectedItem([&](HTREEITEM hItem)->HTREEITEM{
        auto hItem2 = m_treeB.GetPrevSiblingItem(hItem);
        return hItem2 == NULL ? NULL : TVI_FIRST;
    });
}

void CConfigDictDlg::OnBnClickedButtonUp()
{
    WithSelectedItem([&](HTREEITEM hItem)->HTREEITEM{
        auto hItem2 = m_treeB.GetPrevSiblingItem(hItem);
        if(hItem2 == NULL) return NULL;
        hItem2 = m_treeB.GetPrevSiblingItem(hItem2);
        return hItem2 == NULL ? TVI_FIRST : hItem2;
    });
}

void CConfigDictDlg::OnBnClickedButtonDown()
{
    WithSelectedItem([&](HTREEITEM hItem)->HTREEITEM{
        auto hItem2 = m_treeB.GetNextSiblingItem(hItem);
        return hItem2 == NULL ? NULL : hItem2;
    });
}

void CConfigDictDlg::OnBnClickedButtonBottom()
{
    WithSelectedItem([&](HTREEITEM hItem)->HTREEITEM{
        auto hItem2 = m_treeB.GetNextSiblingItem(hItem);
        return hItem2 == NULL ? NULL : TVI_LAST;
    });
}

void CConfigDictDlg::OnClickTreeA(NMHDR *pNMHDR, LRESULT *pResult)
{
    UINT uFlags = 0;
    CPoint pt(0, 0);
    GetCursorPos(&pt);
    m_treeA.ScreenToClient(&pt);
    HTREEITEM hItem = m_treeA.HitTest(pt, &uFlags);
    if(NULL != hItem && (TVHT_ONITEMSTATEICON  & uFlags))
    {
        m_treeA.SelectItem(hItem);
        auto hItemParent = m_treeA.GetParentItem(hItem);
        if(hItemParent == NULL){
            BOOL fCheck = !m_treeA.GetCheck(hItem);
            for(auto hDict = m_treeA.GetChildItem(hItem); hDict; hDict = m_treeA.GetNextSiblingItem(hDict))
                m_treeA.SetCheck(hDict, fCheck);
        }
        else{
            BOOL fCheck = TRUE;
            for(auto hDict = m_treeA.GetChildItem(hItemParent); hDict; hDict = m_treeA.GetNextSiblingItem(hDict))
                if((hDict == hItem) == m_treeA.GetCheck(hDict))
                    fCheck = FALSE;
            m_treeA.SetCheck(hItemParent, fCheck);
        }
    }
    *pResult = 0;
}

void CConfigDictDlg::OnBeginlabeleditTreeB(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    *pResult = m_treeB.GetParentItem(pTVDispInfo->item.hItem) != NULL;
}

void CConfigDictDlg::OnBnClickedOk()
{
    m_vpUIDicts.clear();
    int nImage, nSelectedImage;
    for(auto hItem = m_treeB.GetChildItem(TVI_ROOT); hItem; hItem = m_treeB.GetNextSiblingItem(hItem))
        if(m_treeB.GetChildItem(hItem) == NULL){
            m_treeB.GetItemImage(hItem, nImage, nSelectedImage);
            m_vpUIDicts.push_back(shared_ptr<CUIDict>(new CUIDictItem(
                m_treeB.GetItemText(hItem), m_mapItem2Key.at(hItem), (EDictImage)nImage
            )));
        }
        else{
            vector<shared_ptr<CUIDictItem>> vpUIDictItems;
            for(auto hItem2 = m_treeB.GetChildItem(hItem); hItem2; hItem2 = m_treeB.GetNextSiblingItem(hItem2)){
                m_treeB.GetItemImage(hItem2, nImage, nSelectedImage);
                vpUIDictItems.push_back(make_shared<CUIDictItem>(
                    m_treeB.GetItemText(hItem2), m_mapItem2Key.at(hItem2), (EDictImage)nImage
                ));
            }
            m_treeB.GetItemImage(hItem, nImage, nSelectedImage);
            m_vpUIDicts.push_back(shared_ptr<CUIDict>(new CUIDictCollection(
                m_treeB.GetItemText(hItem), m_mapItem2Key.at(hItem), (EDictImage)nImage, vpUIDictItems
            )));
        }

    CDialog::OnOK();
}
