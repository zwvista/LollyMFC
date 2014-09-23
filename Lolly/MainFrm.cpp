// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Lolly.h"
#include <wininet.h>
#include "MainFrm.h"
#include "WebBrowserFrame.h"
#include "WordsLessonsFrame.h"
#include "WordsLangFrame.h"
#include "WordsAtWillFrame.h"
#include "WordsBooksFrame.h"
#include "PhrasesLessonsFrame.h"
#include "PhrasesLangFrame.h"
#include "WordsLessonsEBFrame.h"
#include "WordsAtWillEBFrame.h"
#include "ExtractWebDictFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEYID_SPEAK			1
#define HOTKEYID_SPEAKCHINESE	2
#define HOTKEYID_SPEAKENGLISH	3

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_CLOSE, &CMainFrame::OnWindowClose)
	ON_COMMAND(ID_WINDOW_CLOSEALL, &CMainFrame::OnWindowCloseAll)
	ON_COMMAND_RANGE(FIRST_FAVORITE_COMMAND, LAST_FAVORITE_COMMAND, &CMainFrame::OnFavorite)
	ON_WM_HOTKEY()
	ON_COMMAND(ID_LEARN_WORDS_LESSONS, &CMainFrame::OnWindowNextWordsLessons)
	ON_COMMAND(ID_LEARN_WORDS_LANG, &CMainFrame::OnWindowNextWordsLang)
	ON_COMMAND(ID_LEARN_WORDS_ATWILL, &CMainFrame::OnWindowNextWordsAtWill)
	ON_COMMAND(ID_LEARN_WORDS_BOOKS, &CMainFrame::OnWindowNextWordsBooks)
	ON_COMMAND(ID_LEARN_PHRASES_LESSONS, &CMainFrame::OnWindowNextPhrasesLessons)
	ON_COMMAND(ID_LEARN_PHRASES_LANG, &CMainFrame::OnWindowNextPhrasesLang)
	ON_COMMAND(ID_LEARN_WORDS_LESSONS_EB, &CMainFrame::OnWindowNextWordsLessonsEB)
	ON_COMMAND(ID_LEARN_WORDS_ATWILL_EB, &CMainFrame::OnWindowNextWordsAtWillEB)
	ON_COMMAND(ID_WINDOW_NEW_CURRENT, &CMainFrame::OnNewCurrentWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW_CURRENT, &CMainFrame::OnUpdateNewCurrentWindow)
	ON_REGISTERED_MESSAGE(AFX_WM_AFTER_TASKBAR_ACTIVATE, OnAfterTaskbarActivate)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	//-----------------------
	// Set up Favorites menu:
	//-----------------------
	VERIFY (theApp.m_Favorites.CreateMenu (m_menuFavotites));

	BOOL b = ::RegisterHotKey(m_hWnd, HOTKEYID_SPEAK, MOD_CONTROL | MOD_SHIFT, VK_F10);
	b = ::RegisterHotKey(m_hWnd, HOTKEYID_SPEAKCHINESE, MOD_CONTROL | MOD_SHIFT, VK_F11);
	b = ::RegisterHotKey(m_hWnd, HOTKEYID_SPEAKENGLISH, MOD_CONTROL | MOD_SHIFT, VK_F12);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

BOOL CMainFrame::OnShowPopupMenu (CMFCPopupMenu* pMenuPopup)
{
	CMDIFrameWndEx::OnShowPopupMenu (pMenuPopup);

	if (pMenuPopup == NULL)
	{
		return TRUE;
	}

	CMFCPopupMenuBar* pMenuBar = pMenuPopup->GetMenuBar ();
	ASSERT_VALID (pMenuBar);

	for (int i = 0; i < pMenuBar->GetCount (); i ++)
	{
		CMFCToolBarButton* pButton = pMenuBar->GetButton (i);
		ASSERT_VALID (pButton);

		if (pButton->m_nID == ID_FAVORITS_DUMMY)
		{
			if (CMFCToolBar::IsCustomizeMode ())
			{
				return FALSE;
			}

			pMenuBar->ImportFromMenu (m_menuFavotites);
			pMenuPopup->SetMaxWidth (300);

			return TRUE;
		}
	}

	return TRUE;
}

BOOL CMainFrame::OnDrawMenuImage (CDC* pDC, const CMFCToolBarMenuButton* pMenuButton, const CRect& rectImage)
{
	if (theApp.m_Favorites.GetSysImages () == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenuButton);

	int iIcon = -1;

	if (pMenuButton->m_nID >= FIRST_FAVORITE_COMMAND &&
		pMenuButton->m_nID <= LAST_FAVORITE_COMMAND)
	{
		iIcon = theApp.m_Favorites.GetIDIcon (pMenuButton->m_nID);
	}
	else if (IsFavoritesMenu (pMenuButton))	// Maybe, favorits folder?
	{
		iIcon = theApp.m_Favorites.GetFolderIcon ();
	}

	if (iIcon == -1)
	{
		CLollyFrame* pFrame = DYNAMIC_DOWNCAST(CLollyFrame, GetActiveFrame());
		return pFrame == NULL ? FALSE : pFrame->OnDrawMenuImage(pDC, pMenuButton, rectImage);
	}

	::ImageList_Draw (theApp.m_Favorites.GetSysImages (), iIcon, 
		pDC->GetSafeHdc (), 
		rectImage.left + (rectImage.Width () - theApp.m_Favorites.GetSysImageSize ().cx) / 2, 
		rectImage.top + (rectImage.Height () - theApp.m_Favorites.GetSysImageSize ().cy) / 2, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CMainFrame::IsFavoritesMenu (const CMFCToolBarMenuButton* pMenuButton)
{
	if (pMenuButton == NULL || pMenuButton->m_nID != (UINT) -1)
	{
		return FALSE;
	}

	ASSERT_VALID (pMenuButton);
	const CObList& lstCommands = pMenuButton->GetCommands ();
	
	for (POSITION pos = lstCommands.GetHeadPosition (); pos != NULL;)
	{
		CMFCToolBarButton* pCmd = (CMFCToolBarButton*) lstCommands.GetNext (pos);
		ASSERT_VALID (pCmd);

		if ((pCmd->m_nID >= FIRST_FAVORITE_COMMAND &&
			pCmd->m_nID <= LAST_FAVORITE_COMMAND) ||
			IsFavoritesMenu (DYNAMIC_DOWNCAST (CMFCToolBarMenuButton, pCmd)))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMainFrame::OnMenuButtonToolHitTest (CMFCToolBarButton* pButton, TOOLINFO* pTI)
{
	ASSERT_VALID (pButton);
	
	if (pButton->m_nID < FIRST_FAVORITE_COMMAND ||
		pButton->m_nID > LAST_FAVORITE_COMMAND)
	{
		return FALSE;
	}

	ASSERT (pTI != NULL);

	CString strText = theApp.m_Favorites.GetURLofID (pButton->m_nID);

	pTI->lpszText = (LPTSTR) ::calloc ((strText.GetLength () + 1), sizeof (TCHAR));
	lstrcpy (pTI->lpszText, strText);

	return TRUE;
}

void CMainFrame::OnClose()
{
	::UnregisterHotKey(m_hWnd, HOTKEYID_SPEAK);
	::UnregisterHotKey(m_hWnd, HOTKEYID_SPEAKCHINESE);
	::UnregisterHotKey(m_hWnd, HOTKEYID_SPEAKENGLISH);
	ShowWindow(SW_HIDE);
	OnWindowCloseAll();
	CMDIFrameWndEx::OnClose();
}

void CMainFrame::OnWindowClose()
{
	CFrameWnd* pFrameWnd = GetActiveFrame();
	if(pFrameWnd != this)
		pFrameWnd->SendMessage(WM_CLOSE);
}

void CMainFrame::OnWindowCloseAll()
{
	for(;;){
		CFrameWnd* pFrameWnd = GetActiveFrame();
		if(pFrameWnd == this) break;
		pFrameWnd->SendMessage(WM_CLOSE);
	}
}

void CMainFrame::OnFavorite(UINT nID)
{
	CString strURL = theApp.m_Favorites.GetURLofID (nID);
	CWebBrowserFrame* pNewFrame = (CWebBrowserFrame*)theApp.OnNewChild(RUNTIME_CLASS(CWebBrowserFrame));
	pNewFrame->Navigate2(strURL);
}


void CMainFrame::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	switch(nHotKeyId){
	case HOTKEYID_SPEAK:		SendMessage(WM_COMMAND, ID_TTS_SPEAK); break;
	case HOTKEYID_SPEAKCHINESE: SendMessage(WM_COMMAND, ID_TTS_SPEAKCHINESE); break;
	case HOTKEYID_SPEAKENGLISH: SendMessage(WM_COMMAND, ID_TTS_SPEAKENGLISH); break;
	}

	CMDIFrameWndEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CMainFrame::FindOrCreateChild(CRuntimeClass* pRuntimeClass, UINT nID)
{
	CMDIChildWnd* pWnd = MDIGetActive();
	if(pWnd == nullptr){
		PostMessage(WM_COMMAND, nID);
		return;
	}

	while((pWnd = (CMDIChildWnd*)pWnd->GetWindow(GW_HWNDNEXT)) != nullptr)
		if(pWnd->IsKindOf(pRuntimeClass))
			break;

	if(pWnd == nullptr)
		PostMessage(WM_COMMAND, nID);
	else
		pWnd->MDIActivate();
}

void CMainFrame::OnWindowNextWordsLessons()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsLessonsFrame), ID_WINDOW_NEW_WORDS_LESSONS);
}

void CMainFrame::OnWindowNextWordsLang()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsLangFrame), ID_WINDOW_NEW_WORDS_LANG);
}

void CMainFrame::OnWindowNextWordsAtWill()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsAtWillFrame), ID_WINDOW_NEW_WORDS_ATWILL);
}

void CMainFrame::OnWindowNextWordsBooks()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsBooksFrame), ID_WINDOW_NEW_WORDS_BOOKS);
}

void CMainFrame::OnWindowNextPhrasesLessons()
{
	FindOrCreateChild(RUNTIME_CLASS(CPhrasesLessonsFrame), ID_WINDOW_NEW_PHRASES_LESSONS);
}

void CMainFrame::OnWindowNextPhrasesLang()
{
	FindOrCreateChild(RUNTIME_CLASS(CPhrasesLangFrame), ID_WINDOW_NEW_PHRASES_LANG);
}

void CMainFrame::OnWindowNextWordsLessonsEB()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsLessonsEBFrame), ID_WINDOW_NEW_WORDS_LESSONS_EB);
}

void CMainFrame::OnWindowNextWordsAtWillEB()
{
	FindOrCreateChild(RUNTIME_CLASS(CWordsAtWillEBFrame), ID_WINDOW_NEW_WORDS_ATWILL_EB);
}

void CMainFrame::OnNewCurrentWindow()
{
	theApp.OnNewChild(MDIGetActive()->GetRuntimeClass());
}

void CMainFrame::OnUpdateNewCurrentWindow( CCmdUI* pCmdUI )
{
	CMDIChildWnd* pWnd = MDIGetActive();
	pCmdUI->Enable(pWnd != NULL && !pWnd->IsKindOf(RUNTIME_CLASS(CExtractWebDictFrame)));
}

LRESULT CMainFrame::OnAfterTaskbarActivate(WPARAM wParam, LPARAM lParam)
{
	HWND hwndMDIChild = (HWND)lParam;
	if(hwndMDIChild != NULL && ::IsWindow(hwndMDIChild))
	{
		::SetFocus(hwndMDIChild);
	}
	return 0;
}
