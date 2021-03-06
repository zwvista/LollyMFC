// WordsEBView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "WordsEBView.h"
#include "SelectKanaDlg.h"


// CWordsEBView

IMPLEMENT_DYNCREATE(CWordsEBView, CView)

CWordsEBView::CWordsEBView()
	: m_hwndComboWord(NULL)
	, m_hwndButtonFind(NULL)
	, m_hwndComboDicts(NULL)
	, m_hwndlstWords(NULL)
	, m_hwndtbSearch(NULL)
{

}

CWordsEBView::~CWordsEBView()
{
}

BEGIN_MESSAGE_MAP(CWordsEBView, CView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWordsEBView drawing

void CWordsEBView::OnDraw(CDC* pDC)
{
}

// CWordsEBView message handlers

void CWordsEBView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	StartProcess(theApp.m_strEBWinExe, _T(""));//Start ms paint
	if(m_procChildApp == NULL)//no window for our process
	{
		MessageBox(_T("Cannot find Window"));
		return;
	}
	m_hwndChildApp = ::GetParent(m_hwndChildApp);

	::SetParent(m_hwndChildApp,m_hWnd);//set parent of ms paint to our dialog.
	SetWindowLong(m_hwndChildApp, GWL_STYLE, WS_VISIBLE);//erase title of ms paint window.

	HWND hwndcbSearch = ::GetDlgItem(m_hwndChildApp, 0xE81B);
	HWND hwnddlgSearch = ::GetDlgItem(hwndcbSearch, 0xE805);
	m_hwndComboWord = ::GetDlgItem(hwnddlgSearch, 0x3E8);
	m_hwndButtonFind = ::GetDlgItem(hwnddlgSearch, 0x3E9);
	m_hwndComboDicts = ::GetDlgItem(hwnddlgSearch, 0x427);
	HWND hwndFrame = ::GetDlgItem(m_hwndChildApp, 0xE900);
	m_hwndlstWords = ::GetDlgItem(hwndFrame, 0xE900);
	m_hwndtbSearch = ::GetDlgItem(hwndcbSearch, 0xE882);
}

void CWordsEBView::OnDestroy()
{
	::SendMessage(m_hwndChildApp, WM_CLOSE, 0, 0);
	CView::OnDestroy();
}

BOOL CWordsEBView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CWordsEBView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	::MoveWindow(m_hwndChildApp, 0, 0,cx, cy, true);
}

void CWordsEBView::Lookup( LPCTSTR pszWord )
{
	if(!m_hwndComboWord) return;
		
	CComboBox* pcboWord = (CComboBox*)CWnd::FromHandle(m_hwndComboWord);
	//do{
		::SendMessage(m_hwndButtonFind, BM_CLICK, 0, 0);
		pcboWord->ResetContent();
		pcboWord->AddString(pszWord);
		pcboWord->SetCurSel(0);
		::SendMessage(m_hwndButtonFind, BM_CLICK, 0, 0);
	//}while(pcboWord->GetCount() != 1);
}

void CWordsEBView::SelectEntry( LPCTSTR pszRef )
{
	if(pszRef == NULL || !_tcscmp(pszRef, _T(""))) return;

	vector<CString> vstrWords;
	GetWordList(vstrWords);
	int nItem = 0;
	for(; nItem < vstrWords.size(); nItem++)
		if(vstrWords[nItem].Find(pszRef) != -1)
			break;

	if(nItem == 0) return;

	LVITEM lvi, *_plvi;

	_plvi = (LVITEM*)VirtualAllocEx(m_procChildApp, NULL, sizeof(LVITEM),
		MEM_COMMIT, PAGE_READWRITE);

	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvi.state = LVIS_FOCUSED | LVIS_SELECTED;
	WriteProcessMemory(m_procChildApp, _plvi, &lvi, sizeof(LVITEM), NULL);
	::SendMessage(m_hwndlstWords, LVM_SETITEMSTATE, nItem, (LPARAM)_plvi);

	VirtualFreeEx(m_procChildApp, _plvi, 0, MEM_RELEASE);
	::SendMessage(m_hwndlstWords, WM_KEYDOWN, VK_RETURN, 1);
	::SendMessage(m_hwndlstWords, WM_KEYUP, VK_RETURN, 1);
}

void CWordsEBView::ChooseDict( LPCTSTR pszDict )
{
	if(!m_hwndComboWord) return;
	CComboBox* pcboDicts = (CComboBox*)CWnd::FromHandle(m_hwndComboDicts);
	int n = pcboDicts->FindStringExact(0, pszDict);
	if(n != -1){
		//won't take any effect
		//pcboDicts->SetCurSel(n);

		::SendMessage(m_hwndChildApp, WM_COMMAND, 33101 + n, 0);
		::SendMessage(m_hwndButtonFind, BM_CLICK, 0, 0);
	}
}

CString CWordsEBView::FindKana( LPCTSTR pszWord )
{
	CString strKana;
	Lookup(pszWord);
	vector<CString> vstrWords;
	GetWordList(vstrWords);
	if(!vstrWords.empty()){
		set<CString> sstrKanas;
		for(CString& str : vstrWords){
			str.TrimRight(_T("(和英)"));
			int n = str.Find(_T("【"));
			sstrKanas.insert(n == -1 ? str : str.Left(n));
		}
		if(sstrKanas.size() == 1)
			strKana = *sstrKanas.begin();
		else{
			CSelectKanaDlg dlg(pszWord, sstrKanas);
			dlg.DoModal();
			strKana = dlg.m_strSelectedKana;
		}
	}
	return strKana;
}

void CWordsEBView::GetWordList(vector<CString>& vstrWords)
{
	int count=(int)::SendMessage(m_hwndlstWords, LVM_GETITEMCOUNT, 0, 0);

	LVITEM lvi, *_plvi;
	TCHAR subitem[512], *_subitem;
	//unsigned long pid;
	//HANDLE process;
	//GetWindowThreadProcessId(m_hwndlstWords, &pid);
	//process = OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|
	//	PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION, FALSE, pid);

	_plvi = (LVITEM*)VirtualAllocEx(m_procChildApp, NULL, sizeof(LVITEM),
		MEM_COMMIT, PAGE_READWRITE);
	_subitem = (TCHAR*)VirtualAllocEx(m_procChildApp, NULL, 512, MEM_COMMIT,
		PAGE_READWRITE);

	lvi.cchTextMax = 512;
	for(int i = 0; i < count; i++) {
		lvi.iSubItem = 1;
		lvi.pszText = _subitem;
		WriteProcessMemory(m_procChildApp, _plvi, &lvi, sizeof(LVITEM), NULL);
		::SendMessage(m_hwndlstWords, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_plvi);
		ReadProcessMemory(m_procChildApp, _subitem, subitem, 512, NULL);
		CString strWord(subitem);
		strWord.Replace(_T("-"), _T(""));
		strWord.Replace(_T("・"), _T(""));
		strWord.Replace(_T("･"), _T(""));
		vstrWords.push_back(strWord);
	}

	VirtualFreeEx(m_procChildApp, _plvi, 0, MEM_RELEASE);
	VirtualFreeEx(m_procChildApp, _subitem, 0, MEM_RELEASE);
	//CloseHandle(process);
}

void CWordsEBView::SearchAllDicts(bool bAll)
{
	TBBUTTONINFO tbbi, *_ptbbi;
	ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
	tbbi.cbSize = sizeof(TBBUTTONINFO);
	tbbi.dwMask = TBIF_STATE;
	_ptbbi = (TBBUTTONINFO*)VirtualAllocEx(m_procChildApp, NULL, sizeof(TBBUTTONINFO),
		MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(m_procChildApp, _ptbbi, &tbbi, sizeof(TBBUTTONINFO), NULL);
	LRESULT lResult = ::SendMessage(m_hwndtbSearch, TB_GETBUTTONINFO , (WPARAM)32905, (LPARAM)_ptbbi);
	ReadProcessMemory(m_procChildApp, _ptbbi, &tbbi, sizeof(TBBUTTONINFO), NULL);
	VirtualFreeEx(m_procChildApp, _ptbbi, 0, MEM_RELEASE);
	if(!(tbbi.fsState & TBSTATE_CHECKED) == bAll)
		::SendMessage(m_hwndChildApp, WM_COMMAND, 32905, 0);
}