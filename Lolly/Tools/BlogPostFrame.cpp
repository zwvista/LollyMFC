// BlogPostFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "BlogPostFrame.h"


// CBlogPostFrame

IMPLEMENT_DYNCREATE(CBlogPostFrame, CLollyFrame)

	CBlogPostFrame::CBlogPostFrame()
	: m_pView(NULL)
	, m_bSourceView(false)
	, m_bLineOnly(false)
	, m_pcboPatternNames(NULL)
{
	m_nToolBarID = IDR_TOOLBAR_BLOGPOST;
}

CBlogPostFrame::~CBlogPostFrame()
{
}


BEGIN_MESSAGE_MAP(CBlogPostFrame, CLollyFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND(ID_TB_BP_TOGGLEVIEW, OnSourceView)
	ON_UPDATE_COMMAND_UI(ID_TB_BP_TOGGLEVIEW, OnUpdateSourceView)
	ON_COMMAND(ID_TB_BP_LINEONLY, OnLineOnly)
	ON_UPDATE_COMMAND_UI(ID_TB_BP_LINEONLY, OnUpdateLineOnly)
	ON_COMMAND(ID_TB_BP_NEWWORD, OnNewWord)
	ON_COMMAND(ID_TB_BP_NEWPATTERN, OnNewPattern)
	ON_COMMAND(ID_TB_BP_PATTERNNAMES, OnPatternNames)
	ON_COMMAND(ID_TB_BP_ORIGINAL, OnOriginal)
	ON_COMMAND(ID_TB_BP_DEFINITION, OnDefinition)
	ON_COMMAND(ID_TB_BP_TRANSLATION, OnTranslation)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_BP_NEWWORD, ID_TB_BP_TRANSLATION, OnUpdateTreeViewButtons)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_BP_B, ID_TB_BP_I, OnUpdateSourceViewButtons)
END_MESSAGE_MAP()

// CBlogPostFrame message handlers

int CBlogPostFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLollyFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateView(RUNTIME_CLASS(CBlogPostView));
	m_pView = DYNAMIC_DOWNCAST(CBlogPostView, GetDlgItem(AFX_IDW_PANE_FIRST));
	return 0;
}

LRESULT CBlogPostFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	if(wParam == m_nToolBarID){
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_TOGGLEVIEW), _T("Source View"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_LINEONLY), _T("Line Only"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_NEWWORD), _T("Word"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_NEWPATTERN), _T("Pattern"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_ORIGINAL), _T("Original"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_DEFINITION), _T("Definition"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_TRANSLATION), _T("Translation"), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_B), _T("B "), TRUE, FALSE);
		m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_BP_I), _T("I "), TRUE, FALSE);
		InitPatterns();
	}
	return 0;
}

void CBlogPostFrame::OnDestroy()
{
	CLollyFrame::OnDestroy();
}

CString CBlogPostFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Blog Post"));
	return str;
}

void CBlogPostFrame::InitPatterns()
{
	m_wndToolBar.ReplaceButton(ID_TB_BP_PATTERNNAMES, CMFCToolBarComboBoxButton2(ID_TB_BP_PATTERNNAMES, 0, CBS_DROPDOWNLIST, 400, 400));
	m_pcboPatternNames = (CMFCToolBarComboBoxButton2*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_BP_PATTERNNAMES));

	CStdioFileEx f;
	f.SetCodePage(65001);
	f.Open(theApp.m_strAppDataFolder + "blog\\SentPatterns.txt", CFile::modeRead);
	CString strLine;
	// the first line begins with an invisible char: \65279
	f.ReadString(strLine);
	while(f.ReadString(strLine)){
		strLine.Trim();
		if(!strLine.IsEmpty())
			m_vstrPatterns.push_back(strLine);
	}
	f.Close();

	m_pcboPatternNames->AddItem(_T("No Sentence Pattern"));
	wregex reg(_T("<a href=\".+?\">(.+?)</a>"));
	for(const CString& strPattern : m_vstrPatterns){
		wstring str(strPattern);
		wsmatch m;
		regex_match(str, m, reg);
		m_pcboPatternNames->AddItem(m[1].str().c_str());
	}
	m_pcboPatternNames->SelectItem(0);
}

void CBlogPostFrame::OnNewWord()
{
	CString strXml("<line>"
		"<original>（）：</original><definition></definition><translation></translation>"
		"</line>");
	NewNote(strXml, m_bLineOnly);
}

void CBlogPostFrame::OnNewPattern()
{
	int index = m_pcboPatternNames->GetCurSel();
	CString strXml, strTemp;
	if(index > 0)
		strTemp.Format(_T("<line>%s</line>"), m_vstrPatterns[index - 1]);
	strXml.Format(
		_T("<note><line>")
		_T("<original>～：</original><definition></definition><translation></translation>")
		_T("</line>%s</note>"),
		strTemp);
	NewNote(strXml, true);
}

void CBlogPostFrame::NewNote(const CString& strXml, bool bNoNoteTag)
{
	CString strNote;
	strNote.Format(bNoNoteTag ? _T("%s") : _T("<note>%s</note>"), strXml);
	m_pView->NewNote(strNote);
	m_bLineOnly = false;
	m_pcboPatternNames->SelectItem(0);
}

void CBlogPostFrame::OnOriginal()
{
	CString strXml("<original>（）：</original>");
	m_pView->NewNote(strXml);
}

void CBlogPostFrame::OnDefinition()
{
	CString strXml("<definition></definition>");
	m_pView->NewNote(strXml);
}

void CBlogPostFrame::OnTranslation()
{
	CString strXml("<translation></translation>");
	m_pView->NewNote(strXml);
}

void CBlogPostFrame::OnSourceView()
{
	m_bSourceView = !m_bSourceView;
	m_pView->ToggleView();
}

