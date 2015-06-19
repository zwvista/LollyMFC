
// Lolly.h : main header file for the Lolly application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "LollyMixin.h"
#include "MFCToolBarButtons2.h"
#include "MFCToolBarComboBoxExButton.h"
#include "LollyFrame.h"
#include "DictHtmlCtrl.h"

#define WM_LBUSETTINGS_CHANGED		WM_USER + 1
#define WM_GRID_ROWCHANGE			WM_USER + 2

// CLollyApp:
// See Lolly.cpp for the implementation of this class
//

class CLollyApp : public CWinAppEx, public CLollyMixin
{
public:
	CLollyApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
protected:

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CMDIChildWnd* OnNewChild(CRuntimeClass* pClass);

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void ExtractTranslation(const vector<CString>& vstrWords, const vector<CString>& vstrDicts, bool bOverwrite, CDictHtmlCtrl* pDictHtmlCtrl = nullptr, const CString& strIfrId = _T(""));

	void SetLangID(int nLangID);

	afx_msg void OnAppAbout();
	afx_msg void OnLearnSelectUnits();
	afx_msg void OnLearnWordsUnits();
	afx_msg void OnLearnWordsLang();
	afx_msg void OnLearnWordsBooks();
	afx_msg void OnLearnWordsAtWill();
	afx_msg void OnLearnPhrasesUnits();
	afx_msg void OnLearnPhrasesLang();
	afx_msg void OnLearnWordsUnitsEB();
	afx_msg void OnLearnWordsAtWillEB();
	afx_msg void OnAuxiliaryAutoCorrect();
	afx_msg void OnAuxiliaryBooks();
	afx_msg void OnAuxiliaryDictionaries();
	afx_msg void OnAuxiliaryPicBooks();
	afx_msg void OnAuxiliaryWebExtract();
	afx_msg void OnAuxiliaryWebText();
	afx_msg void OnToolsExtractWebDict();
	afx_msg void OnToolsWebBrowser();
	afx_msg void OnToolsBlogPost();
	afx_msg void OnToolsPicBook();
	afx_msg void OnToolsHtmlUnit();
	afx_msg void OnToolsAutoCorrectText();
	afx_msg void OnToolsText2Post();
	afx_msg void OnToolsTest();
	afx_msg void OnToolsOptions();
	afx_msg void OnTtsSpeak();
	afx_msg void OnTtsStopSpeaking();
	afx_msg void OnTtsSpeakChinese();
	afx_msg void OnTtsSpeakEnglish();
	afx_msg void OnUpdateTtsSpeak(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

extern CLollyApp theApp;
