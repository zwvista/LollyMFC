#pragma once

#include "ADOEvents.h"
#include "AutoFont.h"
#include "Lingoes.h"
#include "Frhelper.h"
#include "FavoritesManager.h"
#include "WndResizer.h"
#include "StdioFileEx.h"

struct SLangBookLessonSettings
{
	int nLangID, nBookID, nLessonFrom, nPartFrom, nLessonTo, nPartTo;
	CString strLangName, strBookName;
	void Init();
	int GetLessonPartFrom() const {return nLessonFrom * 10 + nPartFrom;}
	int GetLessonPartTo() const {return nLessonTo * 10 + nPartTo;}
	CString GetBookLessonsDesc() const {
		CString str;
		if(GetLessonPartFrom() == GetLessonPartTo())
			str.Format(_T("%s %d:%d"), strBookName, nLessonFrom, nPartFrom);
		else
			str.Format(_T("%s %d:%d--%d:%d"), strBookName, nLessonFrom, nPartFrom, nLessonTo, nPartTo);
		return str;
	}
	CString GetLangDesc() const {
		CString str;
		str.Format(_T("%s"), strLangName);
		return str;
	}
};

struct SDataGridNewRecordInfo
{
	int nCol;
	LPCTSTR lpszText;
};

enum EDictImage
{
	DICTIMAGE_OFFLINEALL, // 0
	DICTIMAGE_OFFLINE, // 1
	DICTIMAGE_ONLINEALL = DICTIMAGE_OFFLINE + 9, // 10
	DICTIMAGE_ONLINE, // 11
	DICTIMAGE_LIVEALL = DICTIMAGE_ONLINE + 9, // 20
	DICTIMAGE_LIVE, // 21
	DICTIMAGE_CUSTOM = DICTIMAGE_LIVE + 9, // 30
	DICTIMAGE_LOCAL, // 31
	DICTIMAGE_SPECIAL, // 32
	DICTIMAGE_CONJUGATOR, // 33
	DICTIMAGE_WEB, // 34
};

typedef pair<CString, CString> DictInfo;

#define DICT_DEFAULT _T("DEFAULT")
#define DICT_ONLINE _T("Online")
#define DICT_ONLINEALL _T("ONLINE")
#define DICT_LINGOES _T("Lingoes")
#define DICT_LINGOESALL _T("LINGOES")
#define DICT_WEB _T("Web")
#define DICT_OFFLINE _T("Offline")
#define DICT_OFFLINEALL _T("OFFLINE")
#define DICT_LIVE _T("Live")
#define DICT_LIVEALL _T("LIVE")

inline CString DoubleApostrophe(LPCTSTR psz)
{
	CString str = psz; 
	str.Replace(_T("'"), _T("''")); 
	return str; 
}
CString ReadAllTextFromFile(LPCTSTR pszFileName);
void SplitString(const CString& str, LPCTSTR pszDelim, vector<CString>& vstrs);
CString GetTextFromClipBoard();
void CopyTextToClipBoard(const CString& strText);
CString GetDocumentText(CHtmlView* pView, bool bBody = false);
CString GetDocumentTitle(CHtmlView* pView);
inline CString HtmlEncode(CString strText)
{
	strText.Replace(_T("&"), _T("&amp;"));
	strText.Replace(_T("<"), _T("&lt;"));
	strText.Replace(_T(">"), _T("&gt;"));
	strText.Replace(_T("\""), _T("&quot;"));
	strText.Replace(_T("'"), _T("&apos;"));
	return strText;
}
CString ExtractFromHtml(LPCTSTR pszText, LPCTSTR pszTransform, LPCTSTR pszDefault = _T(""));

class CLollyMixin
{
public:
	CADODatabase m_db;
	SLangBookLessonSettings m_lblSettings;
	CAutoFont m_defFont;
	static const CString m_strNoTrans;
	CImageList m_imglstDictTypes;
	CLingoes m_objLingoes;
	CFrhelper m_objFrhelper;
	CString m_strAppDataFolder, m_strAppDataFolderInHtml, m_strJS;
	wptree m_ptConfig;
	CString m_strXmlNotepadExe, m_strEBWinExe;
	CString m_strLingoesClassName, m_strLingoesWindowName;
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

	wptree GetConfig(int nLangID);
	wptree GetConfigDicts(int nLangID);
protected:
	vector<CAdapt<CComPtr<ISpVoice> > > m_vcpVoices;
	vector<CString> m_vstrVoiceNames;

	CString AutoCorrect(LPCTSTR pszText, CADORecordset2& rsAutoCorrect, LPCTSTR pszCol1, LPCTSTR pszCol2);
	void InitVoices();
};
