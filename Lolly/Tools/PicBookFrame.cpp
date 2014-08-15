// PicBookFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "PicBookFrame.h"


// CPicBookFrame

IMPLEMENT_DYNCREATE(CPicBookFrame, CLollyFrame)

CPicBookFrame::CPicBookFrame()
	: m_nIDLineWrite(0)
	, m_pedtPageNo(NULL)
	, m_pView(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_PICBOOK;
}

CPicBookFrame::~CPicBookFrame()
{
}


BEGIN_MESSAGE_MAP(CPicBookFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND_RANGE(ID_TB_MOVEPREVIOUS, ID_TB_MOVENEXT, &CPicBookFrame::OnMove)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVEPREVIOUS, &CPicBookFrame::OnUpdateMovePrevious)
	ON_UPDATE_COMMAND_UI(ID_TB_MOVENEXT, &CPicBookFrame::OnUpdateMoveNext)
	ON_COMMAND_RANGE(ID_TB_PB_LINE1, ID_TB_PB_LINE2, &CPicBookFrame::OnLine)
	ON_COMMAND(ID_TB_PB_WRITE, &CPicBookFrame::OnWrite)
	ON_COMMAND(ID_TB_PB_PAGENO, &CPicBookFrame::OnPageNo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_PB_LINE1, ID_TB_PB_WRITE, &CPicBookFrame::OnUpdateLineWrite)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CPicBookFrame message handlers

int CPicBookFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CPicBookView));
	m_pView = DYNAMIC_DOWNCAST(CPicBookView, GetDlgItem(AFX_IDW_PANE_FIRST));
	return 0;
}

LRESULT CPicBookFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	if(wParam == m_nToolBarID){
		m_wndToolBar.ReplaceButton(ID_TB_PB_BOOKNAME, CMFCToolBarLabelButton(ID_TB_PB_BOOKNAME));
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_PB_BOOKNAME), _T(""), TRUE, FALSE);
		m_wndToolBar.ReplaceButton(ID_TB_PB_PAGENO, CMFCToolBarEditBoxButton2(ID_TB_PB_PAGENO, -1, ES_AUTOHSCROLL, 50));
		m_pedtPageNo = (CMFCToolBarEditBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_PB_PAGENO));
		m_wndToolBar.ReplaceButton(ID_TB_PB_PAGECOUNT, CMFCToolBarLabelButton(ID_TB_PB_PAGECOUNT));
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_PB_PAGECOUNT), _T(""), TRUE, FALSE);
	}
	return 0;
}

void CPicBookFrame::OnDestroy()
{
	CLollyFrame::OnDestroy();

	CString sql;
	sql.Format(_T("UPDATE PICBOOKS SET CURPAGE = %d WHERE BOOKNAME = '%s'"), m_nPageNo, m_strBookName);
	theApp.m_db.Execute(sql);
}

CString CPicBookFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Pic Book"));
	return str;
}

void CPicBookFrame::Init( const CString& strBookName, const CString& strFileName, int nNumPages, int nCurPage )
{
	m_strBookName = strBookName;
	m_strFileName = strFileName;
	m_nPageCount = nNumPages;

	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_PB_BOOKNAME), m_strBookName);
	CString str;
	str.Format(_T(" / %d"), m_nPageCount);
	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_PB_PAGECOUNT), str);

	SetPageNo(nCurPage);
}

void CPicBookFrame::OnUpdateMovePrevious( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_nPageNo > 1);
}

void CPicBookFrame::OnUpdateMoveNext( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(m_nPageNo < m_nPageCount);
}

void CPicBookFrame::OnMove(UINT nID)
{
	SetPageNo(m_nPageNo + (nID == ID_TB_MOVEPREVIOUS ? -1 : 1));
}

void CPicBookFrame::SetPageNo( int nPageNo )
{
	m_nPageNo = nPageNo;
	CString str;
	str.Format(_T("%d"), m_nPageNo);
	m_pedtPageNo->SetContents(str);
	m_wndToolBar.AdjustSizeImmediate();
	((CPicBookView*)GetActiveView())->OpenFile();
	SetTimer(100, 700, NULL);
}

void CPicBookFrame::OnTimer(UINT_PTR nIDEvent)
{
	static bool bFileOpened = false;
	if(!bFileOpened){
		HWND hwndDlg = ::FindWindowEx(NULL, NULL, _T("#32770"), _T("´ò¿ª"));
		HWND hwndComboEx = ::FindWindowEx(hwndDlg, NULL, _T("ComboBoxEx32"), NULL);
		CString str;
		str.Format(m_strFileName, m_nPageNo);
		::SendMessage(hwndComboEx, WM_SETTEXT, NULL, (LPARAM)(LPCTSTR)str);
		HWND hwndOpen = ::GetDlgItem(hwndDlg, IDOK);
		::SendMessage(hwndOpen, BM_CLICK, 0, 0);
		KillTimer(100);
		SetTimer(100, 200, NULL);
	}
	else{
		OnWrite();
		KillTimer(100);
	}
	bFileOpened = !bFileOpened;
}

void CPicBookFrame::OnSetFocus(CWnd* pOldWnd)
{
	CLollyFrame::OnSetFocus(pOldWnd);

	m_pView->AutoCorrectWriting();
}

void CPicBookFrame::OnUpdateLineWrite( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nIDLineWrite);
}

void CPicBookFrame::OnPageNo()
{
	CString str;
	m_pedtPageNo->GetEditBox()->GetWindowText(str);
	int n;
	if(_stscanf(str, _T("%d"), &n) == 1 && n >= 1 && n <= m_nPageCount){
		SetPageNo(n);
	}
	else{
		str.Format(_T("%d"), m_nPageNo);
		m_pedtPageNo->SetContents(str);
	}
}

void CPicBookFrame::OnLine( UINT nID )
{
	m_nIDLineWrite = nID;
	m_pView->SelectLine(nID == ID_TB_PB_LINE1);
}

void CPicBookFrame::OnWrite()
{
	m_nIDLineWrite = ID_TB_PB_WRITE;
	m_pView->BeginWrite();
}
