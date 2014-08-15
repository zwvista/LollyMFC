// HtmlLessonFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "HtmlLessonFrame.h"
#include "StdioFileEx.h"


// CHtmlLessonFrame

IMPLEMENT_DYNCREATE(CHtmlLessonFrame, CLollyFrame)

CHtmlLessonFrame::CHtmlLessonFrame()
	: m_pView(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_HTMLLESSON;
}

CHtmlLessonFrame::~CHtmlLessonFrame()
{
}


BEGIN_MESSAGE_MAP(CHtmlLessonFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TB_OPEN, OnOpen)
	ON_COMMAND(ID_TB_SAVE, OnSave)
	ON_COMMAND(ID_TB_HL_BACK1, OnSetBack1)
	ON_COMMAND(ID_TB_HL_BACK2, OnSetBack2)
	ON_COMMAND(ID_TB_HL_REMOVEFORMAT, OnRemoveFormat)
	ON_COMMAND(ID_TB_SPEAK, OnSpeak)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHtmlLessonFrame message handlers


int CHtmlLessonFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateView(RUNTIME_CLASS(CHtmlEditView));
	m_pView = DYNAMIC_DOWNCAST(CHtmlEditView, GetDlgItem(AFX_IDW_PANE_FIRST));
	return 0;
}

LRESULT CHtmlLessonFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	if(wParam == m_nToolBarID){
		m_wndToolBar.ReplaceButton(ID_TB_HL_FILENAME, CMFCToolBarLabelButton(ID_TB_HL_FILENAME));
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_HL_FILENAME), _T(""), TRUE, FALSE);
	}
	return 0;
}

CString CHtmlLessonFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Html Lesson"));
	return str;
}

void CHtmlLessonFrame::OnOpen()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() != IDOK) return;
	m_strFileName = dlg.GetPathName();
	m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(ID_TB_HL_FILENAME), m_strFileName);
	m_wndToolBar.AdjustSizeImmediate();
	m_pView->Navigate2(m_strFileName);
}

void CHtmlLessonFrame::OnSave()
{
	//m_pView->SaveAs(NULL);
	m_pView->SaveAs(m_strFileName);
}

void CHtmlLessonFrame::OnSetBack1()
{
	m_pView->SetBackColor(_T("#FFFF00"));
}

void CHtmlLessonFrame::OnSetBack2()
{
	m_pView->SetBackColor(_T("#66FF66"));
}

void CHtmlLessonFrame::OnRemoveFormat()
{
	m_pView->RemoveFormat();
}

void CHtmlLessonFrame::OnSpeak()
{
	m_pView->Copy();
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_TTS_SPEAK);
}

void CHtmlLessonFrame::OnClose()
{
	if(m_pView->GetIsDirty() == S_OK)
		OnSave();

	CLollyFrame::OnClose();
}
