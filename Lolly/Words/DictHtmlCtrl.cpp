// DictHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "DictHtmlCtrl.h"
#include "EditTransDlg.h"

// CDictHtmlCtrl

IMPLEMENT_DYNCREATE(CDictHtmlCtrl, CLollyHtmlView)

CDictHtmlCtrl::CDictHtmlCtrl()
: m_rsWord(&theApp.m_db)
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

CString CDictHtmlCtrl::GetTemplatedHtml(const CString& strWord, const CString& str)
{
    CString strHtml = m_pConfig->m_rsDict.GetFieldValueAsString(_T("TEMPLATE"));
    strHtml.Replace(_T("{0}"), strWord);
    strHtml.Replace(_T("{1}"), theApp.m_strAppDataFolderInHtml + "css/");
    strHtml.Replace(_T("{2}"), str);
    return strHtml;
};

void CDictHtmlCtrl::UpdateLiveHtml(LPCTSTR pszIfrId, LPCTSTR pszWord, LPCTSTR pszDict, LPCTSTR pszTranslation)
{
	CString strHtml = GetDocumentText(this);
    CString strText = theApp.GetLiveHtml(pszWord, pszDict, _istupper(strHtml[2]));
    if(pszIfrId == nullptr)
	    strHtml.Replace(strText, pszTranslation);
    else
    {
        FindDict(pszDict);
        CString strIfrContent = GetTemplatedHtml(pszWord, pszTranslation);
    }
	SetHTML(strHtml);
}

void CDictHtmlCtrl::FindDict(LPCTSTR pszDictName)
{
	CString str;
	str.Format(_T("DICTNAME='%s'"), pszDictName);
    m_pConfig->m_rsDict.FindFirst(str);
}

CString CDictHtmlCtrl::GetTranslation(const CString& strWord)
{
    CString sql;
    sql.Format(_T("SELECT * FROM [%s] WHERE WORD='%s'"),
        m_pConfig->m_rsDict.GetFieldValueAsString(_T("DICTTABLE")), DoubleApostrophe(strWord));
    m_rsWord.Open(sql);

    CString strTranslation;
    if(!m_rsWord.IsEof())
        strTranslation = m_rsWord.GetFieldValueAsString(_T("TRANSLATION"));
    return strTranslation;
}

void CDictHtmlCtrl::UpdateHtml(const CString& strWord, CADORecordset2& rsAutoCorrect)
{
    auto GetTranslationHtml = [&]()->CString{
        auto strTranslation = GetTranslation(strWord);
        m_bEmptyTrans = m_bEmptyTrans && strTranslation.IsEmpty();
        return GetTemplatedHtml(strWord, strTranslation);
    };
    
    auto GetDictURLForCurrentWord = [&]{
		return theApp.GetDictURLForWord(strWord, m_pConfig->m_rsDict, rsAutoCorrect);
	};

    auto GetLingoesHtml = [&]{
        return theApp.m_objLingoes.Search(strWord);
    };

	auto GetLingoesAllHtml = [&]{
		return theApp.m_objLingoes.Search(strWord, m_pConfig->m_vstrDictsLingoes);
	};

    auto GetFrhelperHtml = [&]{
        return GetTemplatedHtml(strWord, theApp.m_objFrhelper.Search(strWord));
    };

	auto GetLiveHtml = [&](const CString& strDict, const CString& strIfrId)->CString{
        theApp.ExtractTranslation({strWord}, {strDict}, true, this, strIfrId);
		return GetTemplatedHtml(strWord, theApp.GetLiveHtml(strWord, strDict,
			_istupper(m_pConfig->m_rsDict.GetFieldValueAsString(_T("TEMPLATE"))[2])));
	};

	m_bEmptyTrans = true;
	if(strWord.IsEmpty())
		SetHTML(_T(""));
    else if(m_vpUIDictItems.size() == 1){
        auto pUIDictItem = m_vpUIDictItems[0];
        FindDict(pUIDictItem->m_strName);
        if(pUIDictItem->m_ImageIndex == DICTIMAGE_ONLINE && pUIDictItem->m_strName != DICT_FRHELPER ||
            pUIDictItem->m_ImageIndex == DICTIMAGE_CONJUGATOR || pUIDictItem->m_ImageIndex == DICTIMAGE_WEB)
            m_bAutomationDone = false, Navigate(GetDictURLForCurrentWord());
        else
            SetHTML(
                pUIDictItem->m_ImageIndex == DICTIMAGE_LOCAL || pUIDictItem->m_ImageIndex == DICTIMAGE_OFFLINE ? GetTranslationHtml() :
                pUIDictItem->m_ImageIndex == DICTIMAGE_LIVE ? GetLiveHtml(pUIDictItem->m_strName, CString()) :
                pUIDictItem->m_ImageIndex == DICTIMAGE_ONLINE && pUIDictItem->m_strName == DICT_FRHELPER ? GetFrhelperHtml() :
                pUIDictItem->m_strName == DICT_LINGOES ? GetLingoesHtml() :
                pUIDictItem->m_strName == DICT_LINGOESALL ? GetLingoesAllHtml() :
                _T("")
		    );
    }
    else
    {
        CString strHtml = _T("<html><head><META content=\"IE=10.0000\" http-equiv=\"X-UA-Compatible\"></head><body>\r\n");
        strHtml.AppendFormat(_T("<script type=\"text/javascript\">\r\n%s\r\n</script>"), theApp.m_strJS);

        for(int i = 0; i < m_vpUIDictItems.size(); ++i){
            auto pUIDictItem = m_vpUIDictItems[i];
            CString strIfrId;
            strIfrId.Format(_T("ifr%d"), i);

            auto GetIFrameOfflineText = [&](LPCTSTR pszText)->CString{
                CString strText;
                CString str = pszText;
                str.Replace(_T("'"), _T("&#39;"));
                str.Replace(_T("\""), _T("\\\""));
                str.Replace(_T("\r\n"), _T("\\r\\n"));
                str.Replace(_T("\n"), _T("\\n"));
                strText.Format(_T("<iframe id='%s' frameborder='0' style='width:100%%; display:block' onload='setFrameContent(this, \"%s\");'></iframe>\r\n"), strIfrId, str);
                return strText;
            };

            auto GetIFrameOnlineText = [&](LPCTSTR pszText)->CString{
                CString strText;
                strText.Format(_T("<iframe id='%s' frameborder='1' style='width:100%%; height:500px; display:block' src='%s'></iframe>\r\n"), strIfrId, pszText);
                return strText;
            };

            auto AddOfflineDictText = [&](LPCTSTR pszDictName){
                FindDict(pszDictName);
                strHtml += GetIFrameOfflineText(GetTranslationHtml());
            };

            auto AddOnlineDictText = [&](LPCTSTR pszDictName){
                FindDict(pszDictName);
                strHtml += GetIFrameOnlineText(GetDictURLForCurrentWord());
            };

            FindDict(pUIDictItem->m_strName);
            if(pUIDictItem->m_strName == DICT_LINGOES)
                strHtml += GetIFrameOfflineText(GetLingoesHtml());
            if(pUIDictItem->m_strName == DICT_LINGOESALL)
                strHtml += GetIFrameOfflineText(GetLingoesAllHtml());
            else if(pUIDictItem->m_strType == DICT_LOCAL || pUIDictItem->m_strType == DICT_OFFLINE)
                strHtml += GetIFrameOfflineText(GetTranslationHtml());
            else if(pUIDictItem->m_strType == DICT_LIVE)
                strHtml += GetIFrameOfflineText(GetLiveHtml(pUIDictItem->m_strName, strIfrId));
            else if(pUIDictItem->m_strName == DICT_FRHELPER)
                strHtml += GetIFrameOfflineText(GetFrhelperHtml());
            else
                strHtml += GetIFrameOnlineText(GetDictURLForCurrentWord());
        }
        strHtml += _T("</body></html>\r\n");
        SetHTML(strHtml);
    }
}

bool CDictHtmlCtrl::CanDeleteTranslation()
{
    return boost::algorithm::any_of(m_vpUIDictItems, [](const CUIDictItem* pUIDictItem){
        return pUIDictItem->m_strType == DICT_OFFLINE;
    });
}

bool CDictHtmlCtrl::CanEditTranslation()
{
    return m_vpUIDictItems.size() == 1 && m_vpUIDictItems[0]->m_strType == DICT_OFFLINE;
}

bool CDictHtmlCtrl::CanExtractAndOverriteTranslation()
{
    return boost::algorithm::any_of(m_vpUIDictItems, [](const CUIDictItem* pUIDictItem){
        return pUIDictItem->m_strType == DICT_OFFLINE;
    }) || m_vpUIDictItems.size() == 1 && m_vpUIDictItems[0]->m_strType == DICT_ONLINE;
}

bool CDictHtmlCtrl::CanExtractAndAppendTranslation()
{
    return m_vpUIDictItems.size() == 1 && m_vpUIDictItems[0]->m_strType == DICT_ONLINE;
}

bool CDictHtmlCtrl::DoDeleteTranslation(const CString& strWord)
{
    vector<CUIDictItem*> vpUIDictItems;
    for(auto pUIDictItem : m_vpUIDictItems)
        if(pUIDictItem->m_strType == DICT_OFFLINE)
            vpUIDictItems.push_back(pUIDictItem);
    bool bIsPlural = vpUIDictItems.size() > 1;

    CString strMsg;
    strMsg.Format(_T("The translation%s of the word \"%s\" in the following dictionar%s\n\n"),
        bIsPlural ? _T("s") : _T(""), bIsPlural ? _T("ies") : _T("y"));
    for(auto&& pUIDictItem : vpUIDictItems)
        strMsg.AppendFormat(_T("\t%s\n"), pUIDictItem->m_strName);
    strMsg += _T("\nwill be DELETED. Are you sure?");
    if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) != IDYES) return false;

    CString sql;
    for(auto pUIDictItem : vpUIDictItems){
        FindDict(pUIDictItem->m_strName);
        sql.Format(_T("UPDATE [%s] SET [TRANSLATION]='%s' WHERE WORD='%s'"),
            m_pConfig->m_rsDict.GetFieldValueAsString(_T("DICTTABLE")), theApp.m_strNoTrans,
            DoubleApostrophe(strWord));
        theApp.m_db.Execute(sql);
    }
}

bool CDictHtmlCtrl::DoEditTranslation(const CString& strWord)
{
    FindDict(m_vpUIDictItems[0]->m_strName);
    auto strTranslation = GetTranslation(strWord);
    CEditTransDlg dlg;
    dlg.m_strTrans = strTranslation;
    if(dlg.DoModal() != IDOK) return false;

    m_rsWord.Edit();
    m_rsWord.SetFieldValue(_T("TRANSLATION"), dlg.m_strTrans);
    m_rsWord.Update();
    return true;
}

bool CDictHtmlCtrl::DoExtractTranslation(const CString& strWord, bool bOverwrite)
{
    CString strMsg;
    if(m_vpUIDictItems.size() == 1 && m_vpUIDictItems[0]->m_strType == DICT_ONLINE){
        auto strDictName = m_vpUIDictItems[0]->m_strName;
        FindDict(strDictName);
        strMsg.Format(_T("The translation from the url \"%s\" will be EXTRACTED and %s ")
            _T("the translation of the word \"%s\" in the dictionary \"%s\". Are you sure?"),
            GetLocationURL(), bOverwrite ? _T("used to REPLACE") : _T("APPENDED to"),
            strWord, strDictName);
        if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) != IDYES) return false;

        theApp.OpenDictTable(strWord, m_rsWord, m_pConfig->m_rsDict);
        theApp.UpdateDictTable(this, m_rsWord, m_pConfig->m_rsDict, !bOverwrite);
    }
    else{
        vector<CUIDictItem*> vpUIDictItems;
        vector<CString> vstrDictsOffline;
        for(auto pUIDictItem : m_vpUIDictItems)
            if(pUIDictItem->m_strType == DICT_OFFLINE){
                vpUIDictItems.push_back(pUIDictItem);
                vstrDictsOffline.push_back(pUIDictItem->m_strName);
            }
        bool bIsPlural = vpUIDictItems.size() > 1;

        if(!m_bEmptyTrans){
            strMsg.Format(_T("The translation%s of the word \"%s\" in the following dictionar%s\n\n"),
                bIsPlural ? _T("s") : _T(""), bIsPlural ? _T("ies") : _T("y"));
            for(auto& pUIDictItem : vpUIDictItems)
                strMsg.AppendFormat(_T("\t%s\n"), pUIDictItem->m_strName);
            strMsg += _T("\nwill be DELETED and EXTRACTED from the web again. Are you sure?");
            if(MessageBox(strMsg, _T(""), MB_YESNO + MB_ICONQUESTION) != IDYES) return false;

            theApp.ExtractTranslation({strWord}, vstrDictsOffline, true);
        }
    }
    return true;
}

void CDictHtmlCtrl::DoWebAutomation(const CString& strWord)
{
    if(m_vpUIDictItems.size() != 1) return;

    auto strDictType = m_vpUIDictItems[0]->m_strType;
    FindDict(m_vpUIDictItems[0]->m_strName);
    if((strDictType == DICTIMAGE_ONLINE || strDictType == DICTIMAGE_WEB)){
        CString strAutomation = m_pConfig->m_rsDict.GetFieldValueAsString(_T("AUTOMATION"));
        if(!strAutomation.IsEmpty() && !m_bAutomationDone){
            theApp.DoWebAutomation(this, strAutomation, strWord);
            m_bAutomationDone = true;
        }
    }
}
