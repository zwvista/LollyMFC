
// BooksFrm.cpp : implementation of the CWordsBaseFrame class
//

#include "stdafx.h"
#include "Lolly.h"
#include "WordsBaseFrame.h"
#include "NewWordsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordsBaseFrame

IMPLEMENT_DYNCREATE(CWordsBaseFrame, CLollyFrameGrid)

BEGIN_MESSAGE_MAP(CWordsBaseFrame, CLollyFrameGrid)
	ON_WM_CREATE()
    ON_MESSAGE(WM_LBLSETTINGS_CHANGED, OnLblSettingsChanged)
    ON_COMMAND(ID_TB_SPEAK, OnSpeak)
	ON_COMMAND(ID_TB_KEEPSPEAK, OnKeepSpeak)
	ON_UPDATE_COMMAND_UI(ID_TB_SPEAK, OnUpdateSpeak)
	ON_UPDATE_COMMAND_UI(ID_TB_KEEPSPEAK, OnUpdateKeepSpeak)
	ON_COMMAND(ID_TB_MULTIADD, OnMultiAdd)
	ON_COMMAND_RANGE(ID_TB_DICTS_OFFLINEALL, ID_TB_DICTS_OFFLINEALL + 50, OnDicts)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_DICTS_OFFLINEALL, ID_TB_DICTS_OFFLINEALL + 50, OnUpdateDicts)
	ON_COMMAND_RANGE(ID_TB_WL_UP, ID_TB_WL_DOWN, OnWordLevelChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TB_WL_UP, ID_TB_WL_DOWN, OnUpdateWordLevelChange)
	ON_COMMAND(ID_TB_REFRESH, OnRefresh)
END_MESSAGE_MAP()

// CWordsBaseFrame construction/destruction

CWordsBaseFrame::CWordsBaseFrame()
	: m_bKeepSpeak(false)
	, m_nDictIndex(-1)
	, m_rsWordLevel(&theApp.m_db)
{
}

CWordsBaseFrame::~CWordsBaseFrame()
{
}

void CWordsBaseFrame::OnMoveComplete()
{
	CLollyFrameGrid::OnMoveComplete();

	m_strWord = m_rs.IsEof() ? CString() : m_rs.GetFieldValueAsString(_T("WORD"));
	if(m_bKeepSpeak)
		OnSpeak();
}

void CWordsBaseFrame::OnSpeak()
{
	theApp.Speak(m_lbuSettings.nLangID, m_strWord);
}

void CWordsBaseFrame::OnKeepSpeak()
{
	if(m_bKeepSpeak = !m_bKeepSpeak)
		OnSpeak();
}

void CWordsBaseFrame::OnUpdateSpeak( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(theApp.CanSpeak(m_lbuSettings.nLangID));
}

void CWordsBaseFrame::OnUpdateKeepSpeak( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(theApp.CanSpeak(m_lbuSettings.nLangID));
	pCmdUI->SetCheck(m_bKeepSpeak);
}

void CWordsBaseFrame::OnMultiAdd()
{
	CNewWordsDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;

	SDataGridNewRecordInfo* nri = GetDataGridNewRecordInfo();
	vector<CString> vstrWords;
	SplitString(dlg.m_strWords, _T("\r\n"), vstrWords);
	for(CString strWord : vstrWords){
		if(strWord.Trim().IsEmpty()) continue;
		nri[0].lpszText = strWord;
		DoAddNew(nri);
	}
	OnAddComplete();
}

void CWordsBaseFrame::LoadTables()
{
	m_wndGrid.m_sigCellFormatting.connect(std::bind(&CWordsBaseFrame::DataGridCellFormatting, this, 
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	CLollyFrameGrid::LoadTables();
}

int CWordsBaseFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CLollyFrameGrid::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBarDicts.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarDicts.LoadToolBar(IDR_TOOLBAR_DICTS))
		return -1;
	m_wndToolBarDicts.EnableDocking(CBRS_ALIGN_ANY);
	m_dockManager.DockPane(&m_wndToolBarDicts);

	return 0;
}

void CWordsBaseFrame::CreateDictCtrls()
{
    m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_DICTS), _T("Dictionaries"));
}

void CWordsBaseFrame::AddDict(UINT nID, CUIDict* pUIDict)
{
	int count = m_wndToolBarDicts.GetCount();
	if(count > 0) --count;
	m_wndToolBarDicts.InsertButton(CMFCToolBarButton(nID, CMFCToolBar::GetDefaultImage(ID_TB_DICTS_OFFLINEALL + pUIDict->m_ImageIndex)));
	m_wndToolBarDicts.SetToolBarBtnText(count, pUIDict->m_strName);
	m_wndToolBarDicts.EnableCustomizeButton(TRUE, (UINT)-1, _T(""));
	m_wndToolBarDicts.AdjustSizeImmediate();
}

int CWordsBaseFrame::GetDictIndexByID( UINT nID )
{
	const CObList& buttons = m_wndToolBarDicts.GetAllButtons();
	int i = 0;
	for(POSITION pos = buttons.GetHeadPosition(); pos != NULL; i++){
		CMFCToolBarButton* pButton = (CMFCToolBarButton*) buttons.GetNext(pos);
		if(pButton == NULL) return -1;
		if(pButton->m_nID == nID) return i;
	}
	return -1;
}

int CWordsBaseFrame::RemoveDict( UINT nID )
{
	int i = GetDictIndexByID(nID);
	if(m_nDictIndex == i)
		SelectDict(i - 1);
	m_wndToolBarDicts.RemoveButton(i);
	m_wndToolBarDicts.AdjustSizeImmediate();
	return i;
}

void CWordsBaseFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CLollyFrameGrid::OnUpdateFrameTitle(bAddToTitle);
	// workaround for the toolbar problem
	m_wndToolBarDicts.AdjustSizeImmediate();
}

void CWordsBaseFrame::OnDicts( UINT nID )
{
	SelectDict(GetDictIndexByID(nID));
}

void CWordsBaseFrame::OnUpdateDicts( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_wndToolBarDicts.GetButton(m_nDictIndex)->m_nID);
}

void CWordsBaseFrame::SelectDict( int nIndex )
{
	m_nDictIndex = nIndex;
	OnDictSelected();
}

void CWordsBaseFrame::SelectNextDict( bool bForward, int nDicts )
{
	//if(nDicts == 0) return;
	//int nIndex = m_pmnuDicts->GetCurSel() + (bForward ? 1 : -1);
	//nIndex = (nIndex + nDicts) % nDicts;
	//SelectDict(nIndex);
}

void CWordsBaseFrame::OnGridItemEntered( const CString& strGridItem )
{
	SDataGridNewRecordInfo* nri = GetDataGridNewRecordInfo();
	nri[0].lpszText = strGridItem;
	DoAddNew(nri);
	m_edtGridItem.SetFocus();
	OnAddComplete();
}

BOOL CWordsBaseFrame::OnDrawMenuImage( CDC* pDC, const CMFCToolBarMenuButton* pMenuButton, const CRect& rectImage )
{
	if(pMenuButton->m_nID >= ID_TB_DICTS_AVAILABLE && pMenuButton->m_nID <= ID_TB_DICTS_AVAILABLE + 50){
		auto sizeMenuImage = CMFCToolBar::GetMenuImageSize();
		CPoint ptImageOffset((rectImage.Width() - sizeMenuImage.cx) / 2, (rectImage.Height() - sizeMenuImage.cy) / 2);
		auto pt = rectImage.TopLeft() + ptImageOffset;
		theApp.m_imglstDictTypes.Draw(pDC, m_mapDictID2Info[pMenuButton->m_nID].second, pt, ILD_TRANSPARENT);
		return TRUE;
	}
	return FALSE;
}

void CWordsBaseFrame::DataGridCellFormatting( int col,long row,CUGCell *cell )
{
	auto nColWord = GetDataGridNewRecordInfo()[0].nCol;
	auto pszWord = m_wndGrid.QuickGetText(nColWord, row);
	if(pszWord){
		auto nLevel = GetWordLevel(pszWord);
		cell->SetBackColor(
			nLevel == 3 ? RGB(105, 105, 105) :
			nLevel == 2 ? RGB(0, 128, 128) :
			nLevel == 1 ? RGB(0, 128, 0) :
			nLevel == -1 ? RGB(255, 140, 0) :
			nLevel == -2 ? RGB(255, 0, 255) :
			nLevel == -3 ? RGB(255, 0, 0) :
			cell->GetBackColor());
		cell->SetTextColor(nLevel == 0 ? cell->GetTextColor() : RGB(0xFF, 0xFF, 0xFF));
	}
}

void CWordsBaseFrame::OnWordLevelChange( UINT nID )
{
	auto nDelta = nID == ID_TB_WL_UP ? 1 : -1;
	set<long> setSelectedRows;

	int startCol;long startRow;int endCol;long endRow;
	int rt = m_wndGrid.EnumFirstBlock(&startCol, &startRow, &endCol, &endRow);
	while(rt == UG_SUCCESS){
		for(long i = startRow; i <= endRow; i++)
			setSelectedRows.insert(i);
		rt = m_wndGrid.EnumNextBlock(&startCol, &startRow, &endCol, &endRow);
	}
	auto nColWord = GetDataGridNewRecordInfo()[0].nCol;

	for(long row : setSelectedRows){
		auto pszWord = m_wndGrid.QuickGetText(nColWord, row);
		auto nLevel = GetWordLevel(pszWord);
		auto nNewLevel = boost::algorithm::clamp(nLevel + nDelta, -3, 3);
		if(nLevel != nNewLevel){
			CString sql;
			sql.Format(_T("UPDATE WORDSLANG SET LEVEL=%d WHERE LANGID=%d AND WORD='%s'"),
				nNewLevel, m_lbuSettings.nLangID, DoubleApostrophe(pszWord));
			m_rsWordLevel.Open(sql);
			m_mapWord2Level[pszWord] = nNewLevel;
		}
	}
	m_wndGrid.RedrawAll();
}

int CWordsBaseFrame::GetWordLevel( LPCTSTR pszWord )
{
	if(m_mapWord2Level.count(pszWord) == 0){
		CString sql;
		sql.Format(_T("SELECT LEVEL FROM WORDSLANG WHERE LANGID=%d AND WORD='%s'"),
			m_lbuSettings.nLangID, DoubleApostrophe(pszWord));
		m_rsWordLevel.Open(sql);
		m_mapWord2Level[pszWord] = m_rsWordLevel.IsEof() ? 0 : m_rsWordLevel.GetFieldValueAsInt(_T("LEVEL"));
	}
	return m_mapWord2Level.at(pszWord);
}

void CWordsBaseFrame::OnRefresh()
{
	m_mapWord2Level.clear();
	m_mapDictID2Info.clear();
	CLollyFrameGrid::OnRefresh();
}

LRESULT CWordsBaseFrame::OnLblSettingsChanged(WPARAM wParam, LPARAM lParam)
{
    CLollyFrameGrid::OnLblSettingsChanged(wParam, lParam);
    LoadDicts();
    return 0;
}