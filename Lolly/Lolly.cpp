
// Lolly.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Lolly.h"
#include "MainFrm.h"

#include "WordsUnitsFrame.h"
#include "WordsLangFrame.h"
#include "WordsBooksFrame.h"
#include "WordsAtWillFrame.h"
#include "PhrasesUnitsFrame.h"
#include "PhrasesLangFrame.h"
#include "WordsUnitsEBFrame.h"
#include "WordsAtWillEBFrame.h"
#include "AuxAutoCorrectFrame.h"
#include "AuxBooksFrame.h"
#include "AuxDictionariesFrame.h"
#include "AuxPicBooksFrame.h"
#include "AuxWebExtractFrame.h"
#include "AuxWebTextFrame.h"
#include "SelectUnitsDlg.h"
#include "ExtractWebDictOptionsDlg.h"
#include "ExtractWebDictFrame.h"
#include "WebBrowserFrame.h"
#include "BlogPostFrame.h"
#include "SelectPicBookDlg.h"
#include "AutoCorrectTextDlg.h"
#include "Text2PostDlg.h"
#include "PicBookFrame.h"
#include "HtmlUnitFrame.h"
#include "TestDlg.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLollyApp

BEGIN_MESSAGE_MAP(CLollyApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CLollyApp::OnAppAbout)
	ON_COMMAND(ID_LEARN_SELECTUNITS, &CLollyApp::OnLearnSelectUnits)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_UNITS, &CLollyApp::OnLearnWordsUnits)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_LANG, &CLollyApp::OnLearnWordsLang)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_BOOKS, &CLollyApp::OnLearnWordsBooks)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_ATWILL, &CLollyApp::OnLearnWordsAtWill)
	ON_COMMAND(ID_WINDOW_NEW_PHRASES_UNITS, &CLollyApp::OnLearnPhrasesUnits)
	ON_COMMAND(ID_WINDOW_NEW_PHRASES_LANG, &CLollyApp::OnLearnPhrasesLang)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_UNITS_EB, &CLollyApp::OnLearnWordsUnitsEB)
	ON_COMMAND(ID_WINDOW_NEW_WORDS_ATWILL_EB, &CLollyApp::OnLearnWordsAtWillEB)
	ON_COMMAND(ID_AUXILIARY_AUTOCORRECT, &CLollyApp::OnAuxiliaryAutoCorrect)
	ON_COMMAND(ID_AUXILIARY_BOOKS, &CLollyApp::OnAuxiliaryBooks)
	ON_COMMAND(ID_AUXILIARY_DICTIONARIES, &CLollyApp::OnAuxiliaryDictionaries)
	ON_COMMAND(ID_AUXILIARY_PICBOOKS, &CLollyApp::OnAuxiliaryPicBooks)
	ON_COMMAND(ID_AUXILIARY_WEBEXTRACT, &CLollyApp::OnAuxiliaryWebExtract)
	ON_COMMAND(ID_AUXILIARY_WEBTEXT, &CLollyApp::OnAuxiliaryWebText)
	ON_COMMAND(ID_TOOLS_EXTRACTWEBDICT, &CLollyApp::OnToolsExtractWebDict)
	ON_COMMAND(ID_TOOLS_WEBBROWSER, &CLollyApp::OnToolsWebBrowser)
	ON_COMMAND(ID_TOOLS_BLOGPOST, &CLollyApp::OnToolsBlogPost)
	ON_COMMAND(ID_TOOLS_PICTUREBOOK, &CLollyApp::OnToolsPicBook)
	ON_COMMAND(ID_TOOLS_HTMLUNIT, &CLollyApp::OnToolsHtmlUnit)
	ON_COMMAND(ID_TOOLS_AUTOCORRECTTEXT, &CLollyApp::OnToolsAutoCorrectText)
	ON_COMMAND(ID_TOOLS_TEXT2POST, &CLollyApp::OnToolsText2Post)
	ON_COMMAND(ID_TOOLS_TEST, &CLollyApp::OnToolsTest)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CLollyApp::OnToolsOptions)
	ON_COMMAND(ID_TTS_SPEAK, &CLollyApp::OnTtsSpeak)
	ON_COMMAND(ID_TTS_STOPSPEAKING, &CLollyApp::OnTtsStopSpeaking)
	ON_COMMAND(ID_TTS_SPEAKCHINESE, &CLollyApp::OnTtsSpeakChinese)
	ON_COMMAND(ID_TTS_SPEAKENGLISH, &CLollyApp::OnTtsSpeakEnglish)
	ON_UPDATE_COMMAND_UI(ID_TTS_SPEAK, &CLollyApp::OnUpdateTtsSpeak)
	ON_UPDATE_COMMAND_UI(ID_TTS_STOPSPEAKING, &CLollyApp::OnUpdateTtsSpeak)
END_MESSAGE_MAP()


// CLollyApp construction

CLollyApp::CLollyApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CLollyApp object

CLollyApp theApp;


// CLollyApp initialization

BOOL CLollyApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ZWStudio"));

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	m_defFont.SetFaceName(_T("SimSun"));
	m_defFont.SetHeightA(12);

	afxGlobalData.fontRegular.DeleteObject();
	afxGlobalData.fontRegular.Attach(::GetStockObject(DEFAULT_GUI_FONT));

	{
		m_imglstDictTypes.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 1);
		CBitmap bmp;
		bmp.LoadBitmap(IDR_TOOLBAR_DICTS);
		m_imglstDictTypes.Add(&bmp, RGB(192, 192, 192));
	}

	m_strAppDataFolderInHtml = m_strAppDataFolder = 
		GetProfileString(_T("Settings"), _T("AppDataFolder"), _T("")) + _T("\\");
	m_strAppDataFolderInHtml.Replace(_T("\\"), _T("/"));
	m_strXmlNotepadExe = GetProfileString(_T("Settings"), _T("XmlNotepadExe"), _T(""));
	m_strEBWinExe = GetProfileString(_T("Settings"), _T("EBWinExe"), _T(""));

	m_strLingoesClassName = GetProfileString(_T("Settings"), _T("LingoesClassName"), _T(""));
	m_strLingoesWindowName = GetProfileString(_T("Settings"), _T("LingoesWindowName"), _T(""));
	m_objLingoes.FindLingoes();

    m_strFrhelperClassName = GetProfileString(_T("Settings"), _T("FrhelperClassName"), _T(""));

	m_strJS = ReadAllTextFromFile(m_strAppDataFolder + "Lolly.js");

    CString strConnection;
    strConnection.Format(_T("Driver=SQLite3 ODBC Driver;Database=%sLolly.db;"), m_strAppDataFolder);
    //strConnection.Format(_T("DSN=Lolly;"));
	m_db.Open(strConnection);
	m_lbuSettings.nLangID = GetProfileInt(_T("Settings"), _T("LangID"), 3);

    m_objConfig.Load(m_strAppDataFolder + "Lolly.config");

	InitVoices();

	m_lbuSettings.Init();

	// Initialize favorites:
	m_Favorites.Load ();

	srand( (unsigned)time( NULL ) );

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}


// CLollyApp message handlers

int CLollyApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CLollyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CLollyApp customization load/save methods

void CLollyApp::PreLoadState()
{
}

void CLollyApp::LoadCustomState()
{
}

void CLollyApp::SaveCustomState()
{
}

// CLollyApp message handlers

CMDIChildWnd* CLollyApp::OnNewChild( CRuntimeClass* pClass )
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// create a new MDI child window
	return pFrame->CreateNewChild(pClass, IDR_MAINFRAME, 0, 
		::LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)));
}

void CLollyApp::OnLearnSelectUnits()
{
	CSelectUnitsDlg dlg;
	if(dlg.DoModal() == IDOK){
		m_lbuSettings.Init();
        if(dlg.m_bApplyAll)
            ((CMDIFrameWnd*)AfxGetMainWnd())->SendMessageToDescendants(WM_LBUSETTINGS_CHANGED);
		else if(dlg.m_bApplyActive){
			CFrameWnd* pFrameWnd = ((CMDIFrameWnd*)AfxGetMainWnd())->GetActiveFrame();
			if(pFrameWnd)
				pFrameWnd->SendMessage(WM_LBUSETTINGS_CHANGED);
		}
	}
}

void CLollyApp::OnLearnWordsUnits()
{
	OnNewChild(RUNTIME_CLASS(CWordsUnitsFrame));
}

void CLollyApp::OnLearnWordsLang()
{
	OnNewChild(RUNTIME_CLASS(CWordsLangFrame));
}

void CLollyApp::OnLearnWordsBooks()
{
	OnNewChild(RUNTIME_CLASS(CWordsBooksFrame));
}

void CLollyApp::OnLearnWordsAtWill()
{
	OnNewChild(RUNTIME_CLASS(CWordsAtWillFrame));
}

void CLollyApp::OnLearnPhrasesUnits()
{
	OnNewChild(RUNTIME_CLASS(CPhrasesUnitsFrame));
}

void CLollyApp::OnLearnPhrasesLang()
{
	OnNewChild(RUNTIME_CLASS(CPhrasesLangFrame));
}

void CLollyApp::OnLearnWordsUnitsEB()
{
	OnNewChild(RUNTIME_CLASS(CWordsUnitsEBFrame));
}

void CLollyApp::OnLearnWordsAtWillEB()
{
	OnNewChild(RUNTIME_CLASS(CWordsAtWillEBFrame));
}

void CLollyApp::OnAuxiliaryAutoCorrect()
{
	OnNewChild(RUNTIME_CLASS(CAuxAutoCorrectFrame));
}

void CLollyApp::OnAuxiliaryBooks()
{
	OnNewChild(RUNTIME_CLASS(CAuxBooksFrame));
}

void CLollyApp::OnAuxiliaryDictionaries()
{
	OnNewChild(RUNTIME_CLASS(CAuxDictionariesFrame));
}

void CLollyApp::OnAuxiliaryPicBooks()
{
	OnNewChild(RUNTIME_CLASS(CAuxPicBooksFrame));
}

void CLollyApp::OnAuxiliaryWebExtract()
{
	OnNewChild(RUNTIME_CLASS(CAuxWebExtractFrame));
}

void CLollyApp::OnAuxiliaryWebText()
{
	OnNewChild(RUNTIME_CLASS(CAuxWebTextFrame));
}

void CLollyApp::OnToolsExtractWebDict()
{
	CExtractWebDictOptionsDlg dlg;
	if(dlg.DoModal() == IDOK)
		ExtractTranslation(dlg.m_vSelectedWords, dlg.m_vSelectedDicts, dlg.m_bOverwriteDB != FALSE);
}

void CLollyApp::OnToolsWebBrowser()
{
	OnNewChild(RUNTIME_CLASS(CWebBrowserFrame));
}

void CLollyApp::OnToolsBlogPost()
{
	OnNewChild(RUNTIME_CLASS(CBlogPostFrame));
}

void CLollyApp::OnToolsPicBook()
{
	CSelectPicBookDlg dlg;
	if(dlg.DoModal() == IDOK){
		CPicBookFrame* pFrame = DYNAMIC_DOWNCAST(CPicBookFrame,
			OnNewChild(RUNTIME_CLASS(CPicBookFrame)));
		pFrame->Init(dlg.GetBookName(), dlg.GetFileName(), dlg.GetNumPages(), dlg.GetCurPage());

	}
}

void CLollyApp::OnToolsHtmlUnit()
{
	OnNewChild(RUNTIME_CLASS(CHtmlUnitFrame));
}

void CLollyApp::ExtractTranslation(const vector<CString>& vstrWords, const vector<CString>& vstrDicts, bool bOverwrite, CDictHtmlCtrl* pDictHtmlCtrl /* = nullptr */, const CString& strIfrId /* = _T("") */)
{
	for(const CString& strDict : vstrDicts){
		CExtractWebDictFrame* pFrame = DYNAMIC_DOWNCAST(CExtractWebDictFrame,
			OnNewChild(RUNTIME_CLASS(CExtractWebDictFrame)));
		pFrame->Init(vstrWords, strDict, bOverwrite, pDictHtmlCtrl, strIfrId);
	}
}

void CLollyApp::OnToolsAutoCorrectText()
{
	CAutoCorrectTextDlg dlg;
	dlg.DoModal();
}

void CLollyApp::OnToolsText2Post()
{
	CText2PostDlg dlg;
	dlg.DoModal();
}

void CLollyApp::OnToolsTest()
{
	CTestDlg dlg;
	if(dlg.DoModal() == IDOK){
	}
}

void CLollyApp::OnToolsOptions()
{
	COptionsDlg dlg;
	if(dlg.DoModal() == IDOK){
	}
}

void CLollyApp::SetLangID( int nLangID )
{
	WriteProfileInt(_T("Settings"), _T("LangID"), m_lbuSettings.nLangID = nLangID);
}

void CLollyApp::OnTtsSpeak()
{
	Speak(m_lbuSettings.nLangID, GetTextFromClipBoard());
}

void CLollyApp::OnTtsStopSpeaking()
{
	Speak(m_lbuSettings.nLangID, _T(""));
}

afx_msg void CLollyApp::OnTtsSpeakChinese()
{
	Speak(0, GetTextFromClipBoard());
}

void CLollyApp::OnTtsSpeakEnglish()
{
	Speak(1, GetTextFromClipBoard());
}

void CLollyApp::OnUpdateTtsSpeak( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(CanSpeak(m_lbuSettings.nLangID));
}
