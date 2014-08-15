
// PhrasesLangFrame.cpp : implementation of the CPhrasesLangFrame class
//

#include "stdafx.h"
#include "Lolly.h"

#include "PhrasesLangFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhrasesLangFrame

IMPLEMENT_DYNCREATE(CPhrasesLangFrame, CPhrasesBaseFrame)

BEGIN_MESSAGE_MAP(CPhrasesLangFrame, CPhrasesBaseFrame)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CPhrasesLangFrame construction/destruction

CPhrasesLangFrame::CPhrasesLangFrame()
{
	m_nToolBarID = IDR_TOOLBAR_PHRASESLANG;
}

CPhrasesLangFrame::~CPhrasesLangFrame()
{
}

CString CPhrasesLangFrame::GetSQL()
{
	CString sql;
	switch(m_nFilterScope){
	case 0:
		sql.Format(_T("SELECT PHRASES.ID, PHRASES.BOOKID, BOOKS.BOOKNAME, PHRASES.LESSON, PHRASES.PART, PHRASES.[INDEX], PHRASES.PHRASE,")
			_T("PHRASES.[TRANSLATION] FROM (PHRASES INNER JOIN BOOKS ON PHRASES.BOOKID = BOOKS.BOOKID)")
			_T("WHERE (BOOKS.LANGID = %d) AND (PHRASES.PHRASE LIKE '%%%s%%')"),
			m_lblSettings.nLangID, m_strFilter);
		break;
	case 1:
		sql.Format(_T("SELECT PHRASES.ID, PHRASES.BOOKID, BOOKS.BOOKNAME, PHRASES.LESSON, PHRASES.PART, PHRASES.[INDEX], PHRASES.PHRASE,")
			_T("PHRASES.[TRANSLATION] FROM (PHRASES INNER JOIN BOOKS ON PHRASES.BOOKID = BOOKS.BOOKID)")
			_T("WHERE (BOOKS.LANGID = %d) AND (PHRASES.[TRANSLATION] LIKE '%%%s%%')"),
			m_lblSettings.nLangID, m_strFilter);
		break;
	}
	return sql;
}

SDataGridColumnInfo* CPhrasesLangFrame::GetDataGridColumnInfo()
{
	static SDataGridColumnInfo ci[] = {
		{ _T("BOOKNAME"), _T("BOOKNAME"), _T("BOOKNAME, LESSON, PART, [INDEX]"), 2, 10, FALSE },
		{ _T("LESSON"), _T("LESSON"), NULL, 75, 0, FALSE },
		{ _T("PART"), _T("PART"), NULL, 75, 0, FALSE },
		{ _T("INDEX"), _T("INDEX"), NULL, 75, 0, FALSE },
		{ _T("PHRASE"), _T("PHRASE"), _T("PHRASE"), 4, 10, TRUE },
		{ _T("TRANSLATION"), _T("TRANSLATION"), _T("[TRANSLATION]"), 4, 10, TRUE },
		{ NULL, NULL, NULL, 0, 0, TRUE },
	};
	return ci;
}

CString CPhrasesLangFrame::GetFrameText() const
{
	CString str;
	str.Format(_T("Phrases (%s)"), m_lblSettings.GetLangDesc());
	return str;
}

int CPhrasesLangFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_wndGrid.SetAllowAddNew(FALSE);
	m_wndGrid.SetAllowDelete(FALSE);

	if (CPhrasesBaseFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

LRESULT CPhrasesLangFrame::OnToolbarReset( WPARAM wParam, LPARAM lParam )
{
	CPhrasesBaseFrame::OnToolbarReset(wParam, lParam);
	if(wParam == m_nToolBarID)
		CreateFilterCtrls();
	return 0;
}
