#pragma once

#include "ADOEvents.h"
#include "AutoFont.h"
#include "Lingoes.h"
#include "Frhelper.h"
#include "FavoritesManager.h"
#include "WndResizer.h"
#include "StdioFileEx.h"
#include "LollyCommon.h"
#include "DictConfig.h"

class CLollyMixin
{
public:
	CADODatabase m_db;
	SLangBookUnitSettings m_lbuSettings;
	CAutoFont m_defFont;
	static const CString m_strNoTrans;
	CImageList m_imglstDictTypes;
	CLingoes m_objLingoes;
	CFrhelper m_objFrhelper;
	CString m_strAppDataFolder, m_strAppDataFolderInHtml, m_strJS;
    CDictConfig m_objConfig;
	CString m_strXmlNotepadExe, m_strEBWinExe;
    CString m_strLingoesClassName, m_strLingoesWindowName;
	CString m_strFrhelperClassName;
	CFavoritesManager	m_Favorites;

	void ComboBox_FillWithRS(CComboBox& cbo, CADORecordset2& rs, LPCTSTR pszDisplayField, LPCTSTR pszValueField = NULL);
	void ComboBox_SelectByValue(CComboBox& cbo, int nValue);

	CString AutoCorrect(LPCTSTR pszText, CADORecordset2& rsAutoCorrect);
	CString GetDictURLForWord(LPCTSTR pszWord, CADORecordset2& rsDict, CADORecordset2& rsAutoCorrect);
	CString ExtractFromWeb(CHtmlView* pView, CADORecordset2& rsExtract, CString strDefault = _T(""));
	bool DoWebAutomation(CHtmlView* pView, const CString& strAutomation, LPCTSTR lpWord);
	CString GetLiveHtml(LPCTSTR pszWord, LPCTSTR pszDict, bool bUpperCase);

	void OpenDictTable(const CString& strWord, CADORecordset2& rsWord, CADORecordset2& rsDict);
	void UpdateDictTable(CHtmlView* pHtmlView, CADORecordset2& rsWord, CADORecordset2& rsDict, bool bAppend);

	CString GetVoiceXml(int nLangID, LPCTSTR psz);
	void Speak(LPCTSTR psz, bool bPurge = true) {Speak(0, psz, bPurge);}
	void Speak(int nLangID, LPCTSTR psz, bool bPurge = true);
	bool CanSpeak(int nLangID);
protected:
	vector<CAdapt<CComPtr<ISpVoice> > > m_vcpVoices;
	vector<CString> m_vstrVoiceNames;

	CString AutoCorrect(LPCTSTR pszText, CADORecordset2& rsAutoCorrect, LPCTSTR pszCol1, LPCTSTR pszCol2);
	void InitVoices();
};
