// DictHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "DictHtmlCtrl.h"

// CDictHtmlCtrl

IMPLEMENT_DYNCREATE(CDictHtmlCtrl, CLollyHtmlView)

CDictHtmlCtrl::CDictHtmlCtrl()
: m_rsDict(&theApp.m_db)
, m_rsWord(&theApp.m_db)
, m_bEmptyTrans(false)
, m_bAutomationDone(false)
{

}

CDictHtmlCtrl::~CDictHtmlCtrl()
{
}

void CDictHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CLollyHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDictHtmlCtrl, CLollyHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CDictHtmlCtrl message handlers

void CDictHtmlCtrl::OnDestroy()
{
	CWnd::OnDestroy();
}

int CDictHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CDictHtmlCtrl::UpdateLiveHtml( LPCTSTR pszWord, LPCTSTR pszDict, LPCTSTR pszTranslation )
{
	CString strHtml = GetDocumentText(this);
	strHtml.Replace(theApp.GetLiveHtml(pszWord, pszDict, _istupper(strHtml[2])), pszTranslation);
	SetHTML(strHtml);
}

void CDictHtmlCtrl::FindDict(LPCTSTR pszDictName)
{
	CString str;
	str.Format(_T("DICTNAME='%s'"), pszDictName);
	m_rsDict.FindFirst(str);
}

void CDictHtmlCtrl::UpdateHtml( const CString& strWord, CADORecordset2& rsAutoCorrect,
							   const vector<CString>& vstrLingoesDicts, const vector<CString>& vstrOfflineDicts,
							   const map<CString, vector<DictInfo>>& mapCustomDicts )
{
	auto GetTemplatedHtml = [&](const CString& str)->CString{
		CString strHtml = m_rsDict.GetFieldValueAsString(_T("TEMPLATE"));
		strHtml.Replace(_T("{0}"), strWord);
		strHtml.Replace(_T("{1}"), theApp.m_strAppDataFolderInHtml + "css/");
		strHtml.Replace(_T("{2}"), str);
		return strHtml;
	};

	auto GetTranslationHtml = [&]()->CString{
		CString sql;
		sql.Format(_T("SELECT * FROM [%s] WHERE WORD=N'%s'"), 
			m_rsDict.GetFieldValueAsString(_T("DICTTABLE")), DoubleApostrophe(strWord));
		m_rsWord.Open(sql);

		CString strTranslation;
		if(!m_rsWord.IsEof())
			strTranslation = m_rsWord.GetFieldValueAsString(_T("TRANSLATION"));

		m_bEmptyTrans = m_bEmptyTrans && strTranslation.IsEmpty();
		return GetTemplatedHtml(strTranslation);
	};

	auto GetDictURLForCurrentWord = [&]{
		return theApp.GetDictURLForWord(strWord, m_rsDict, rsAutoCorrect);
	};

	auto GetLingoesHtml = [&](bool bAll){
		return bAll ? theApp.m_objLingoes.Search(strWord) : theApp.m_objLingoes.Search(strWord, vstrLingoesDicts);
	};

	auto GetLiveHtml = [&](const CString& strDict)->CString{
		theApp.ExtractTranslation(vector<CString>(1, strWord), vector<CString>(1, strDict), true, this);
		return GetTemplatedHtml(theApp.GetLiveHtml(strWord, strDict,
			_istupper(m_rsDict.GetFieldValueAsString(_T("TEMPLATE"))[2])));
	};

	auto GetCustomHtml = [&](const vector<DictInfo>& vDictInfos)->CString{
		auto GetIFrameOfflineText = [&](LPCTSTR pszText)->CString{
			CString strText;
			CString str = pszText;
			str.Replace(_T("'"), _T("&#39;"));
			str.Replace(_T("\""), _T("\\\""));
			str.Replace(_T("\r\n"), _T("\\r\\n"));
			str.Replace(_T("\n"), _T("\\n"));
			strText.Format(_T("<iframe frameborder='0' style='width:100%%; display:block' onload='setFrameContent(this, \"%s\");'></iframe>\r\n"), str);
			return strText;
		};

		auto GetIFrameOnlineText = [&](LPCTSTR pszText)->CString{
			CString strText;
			strText.Format(_T("<iframe frameborder='1' style='width:100%%; height:500px; display:block' src='%s'></iframe>\r\n"), pszText);
			return strText;
		};

		CString strHtml = _T("<html><head><META content=\"IE=10.0000\" http-equiv=\"X-UA-Compatible\"></head><body>\r\n");

		auto AddOfflineDictText = [&](LPCTSTR pszDictName){
			FindDict(pszDictName);
			strHtml += GetIFrameOfflineText(GetTranslationHtml());
		};

		auto AddOnlineDictText = [&](LPCTSTR pszDictName){
			FindDict(pszDictName);
			strHtml += GetIFrameOnlineText(GetDictURLForCurrentWord());
		};

		for(const auto& info : vDictInfos)
		{
			auto& dictType = info.first;
			auto& dictA = info.second;
			if(dictA == DICT_LINGOES || dictA == DICT_LINGOESALL)
				strHtml += GetIFrameOfflineText(GetLingoesHtml(dictA == DICT_LINGOESALL));
			else if(dictA == DICT_OFFLINEALL)
				for(const auto& dictB : vstrOfflineDicts)
					AddOfflineDictText(dictB);
			else if(dictA == DICT_ONLINEALL)
				for(const auto& dictB : vstrOfflineDicts)
					AddOnlineDictText(dictB);
			else if(dictA == DICT_LIVEALL)
				for(const auto& dictB : vstrOfflineDicts)
					AddOfflineDictText(GetLiveHtml(dictB));
			else if(dictType == _T("Local") || dictType == _T("Offline"))
				AddOfflineDictText(dictA);
			else if(dictType == _T("Conjugator") || dictType == _T("Online") ||dictType == _T("Web"))
				AddOnlineDictText(dictA);
			else if(dictType == _T("Live"))
				AddOfflineDictText(GetLiveHtml(dictA));
		}
		strHtml.AppendFormat(_T("<script type=\"text/javascript\">\r\n%s\r\n</script>"), theApp.m_strJS);
		strHtml += _T("</body></html>\r\n");
		return strHtml;
	};

	m_bEmptyTrans = true;
	if(strWord.IsEmpty())
		SetHTML(_T(""));
	else if(m_nDictImage == DICTIMAGE_ONLINE || m_nDictImage == DICTIMAGE_CONJUGATOR || m_nDictImage == DICTIMAGE_WEB)
		m_bAutomationDone = false, Navigate(GetDictURLForCurrentWord());
	else
		SetHTML(
			m_nDictImage == DICTIMAGE_LOCAL || m_nDictImage == DICTIMAGE_OFFLINE ? GetTranslationHtml() :
			m_nDictImage == DICTIMAGE_LIVE ? GetLiveHtml(m_strDictName) :
			m_nDictImage == DICTIMAGE_CUSTOM ? GetCustomHtml(mapCustomDicts.at(m_strDictName)) :
			m_strDictName == DICT_OFFLINEALL || m_strDictName == DICT_ONLINEALL ||
				m_strDictName == DICT_LIVEALL ? GetCustomHtml(vector<DictInfo>(1, DictInfo(_T("Group"), m_strDictName))) :
			m_strDictName == DICT_LINGOES || m_strDictName == DICT_LINGOESALL ?
				GetLingoesHtml(m_strDictName == DICT_LINGOESALL) :
			_T("")
		);
}
