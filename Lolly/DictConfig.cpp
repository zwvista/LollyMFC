#include "stdafx.h"
#include "Lolly.h"
#include "DictConfig.h"

void CDictConfig::Load(LPCTSTR lpszURI)
{
    CString strConfig = ReadAllTextFromFile(lpszURI);
    wptree ptConfig;
    read_xml(wstringstream((LPCTSTR)strConfig), ptConfig);
    ptConfig = ptConfig.get_child(_T("configuration"));

    // group
    map<CString, wptree> mapDictGroupInfo;
    for(const auto& v : ptConfig.get_child(_T("dictInfo")))
        if(v.first == _T("group"))
            mapDictGroupInfo[v.second.data().c_str()] = v.second;

    for(const auto& v : ptConfig)
        if(v.first == _T("language")){
            auto& ptLang = v.second;
            int nLangID = ptLang.get<int>(_T("<xmlattr>.id"));
            m_mapLang2Config[nLangID] = CDictLangConfig(ptLang, mapDictGroupInfo, nLangID);
        }
}

CDictLangConfig::CDictLangConfig(const wptree& ptLang, const map<CString, wptree>& mapDictGroupInfo, int nLangID)
    : m_rsDict(&theApp.m_db)
{
    // replacement
    for(const auto& v : ptLang)
        if(v.first == _T("phraseReplace"))
            m_vReplacement.emplace_back(
                CString(v.second.get<wstring>(_T("<xmlattr>.before")).c_str()),
                CString(v.second.get<wstring>(_T("<xmlattr>.after")).c_str()));

    auto& ptDicts = ptLang.get_child_optional(_T("dictionaries"));
    if(!ptDicts) return;

    // ebwin
    for(auto& v : *ptDicts)
        if(v.first == _T("ebwin"))
            m_vstrDictsEBWin.push_back(v.second.data().c_str());

    auto AddDictGroup = [&](EDictImage nDictImage, const CString& strGroupName, const vector<CString>& vstrDictNames){
        if(vstrDictNames.empty()) return;

        auto& vobjItems = m_mapDictGroups[strGroupName];

        int nImageIndex = nDictImage;
        for(auto& strDictName : vstrDictNames){
            auto pItem = make_shared<CUIDictItem>(strDictName, strGroupName, (EDictImage)nImageIndex);
            vobjItems.push_back(pItem);
            m_mapDictItems[strGroupName + strDictName] = pItem;
            if(nDictImage == DICTIMAGE_ONLINE || nDictImage == DICTIMAGE_OFFLINE || nDictImage == DICTIMAGE_LIVE)
                ++nImageIndex;
        }
    };

    // group
    CString sql;
    sql.Format(_T("SELECT * FROM DICTALL WHERE LANGID=%d "), nLangID);
    for(auto& v : *ptDicts)
        if(v.first == _T("group")){
            CString strGroupName = v.second.data().c_str();
            auto& ptGroupInfo = mapDictGroupInfo.at(strGroupName);
            CString strDictType = ptGroupInfo.get<wstring>(_T("<xmlattr>.dictType")).c_str();
            auto nDictImage = (EDictImage)ptGroupInfo.get<int>(_T("<xmlattr>.imageIndex"));
            vector<CString> vstrDictNames;
            CString sql2;
            if(strGroupName == DICT_WEB)
                sql2 = sql + _T("AND DICTTYPENAME = 'WEB' ORDER BY DICTNAME");
            else
                sql2.Format(_T("%sAND DICTTYPENAME = '%s' ORDER BY ORD"), sql, strDictType);
            m_rsDict.Open(sql2);
            if(m_rsDict.GetRecordCount() != 0)
                for(m_rsDict.MoveFirst(); !m_rsDict.IsEof(); m_rsDict.MoveNext()){
                    auto strDict = m_rsDict.GetFieldValueAsString(_T("DICTNAME"));
                    vstrDictNames.push_back(strDict);
                    if(strGroupName == DICT_OFFLINE){
                        m_vstrDictsOffline.push_back(strDict);
                        m_vstrDictTablesOffline.push_back(m_rsDict.GetFieldValueAsString(_T("DICTTABLE")));
                    }
                }
            AddDictGroup(nDictImage, strGroupName, vstrDictNames);
        }

    // lingoes + special
    vector<CString> vstrSpecialDicts;
    for(auto& v : *ptDicts)
        if(v.first == _T("lingoes"))
            m_vstrDictsLingoes.push_back(v.second.data().c_str());
        else if(v.first == _T("special"))
            vstrSpecialDicts.push_back(v.second.data().c_str());
    if(!m_vstrDictsLingoes.empty())
        vstrSpecialDicts.push_back(_T("Lingoes"));
    boost::sort(vstrSpecialDicts);
    AddDictGroup(DICTIMAGE_SPECIAL, _T("Special"), vstrSpecialDicts);

    // custom
    vector<CString> vstrDictNamesCustom;
    for(auto& v : *ptDicts)
        if(v.first == _T("custom")){
            auto& ptCustom = v.second;
            CString strDictNameCustom = ptCustom.get<wstring>(_T("<xmlattr>.name")).c_str();
            CString strDictTypeCustom = ptCustom.get<wstring>(_T("<xmlattr>.type")).c_str();
            vstrDictNamesCustom.push_back(strDictNameCustom);
            auto& vDicts = m_mapDictsCustom[strDictNameCustom];
            vector<shared_ptr<CUIDictItem>> vpItems;
            for(const auto& v2 : ptCustom)
                if(v2.first == _T("dict")){
                    CString strDictName = v2.second.data().c_str();
                    CString strDictType = v2.second.get<wstring>(_T("<xmlattr>.type")).c_str();
                    auto vpItems2 =
                        strDictName == DICT_OFFLINEALL ? m_mapDictGroups.at(DICT_OFFLINE) :
                        strDictName == DICT_ONLINEALL ? m_mapDictGroups.at(DICT_ONLINE) :
                        strDictName == DICT_LIVEALL ? m_mapDictGroups.at(DICT_LIVE) :
                        vector<shared_ptr<CUIDictItem>>{m_mapDictItems.at(strDictType + strDictName)};
                    vpItems.insert(vpItems.end(), vpItems2.begin(), vpItems2.end());
                }
            m_mapDictsCustom[strDictNameCustom] = shared_ptr<CUIDict>(
                new CUIDictCollection(strDictTypeCustom, strDictNameCustom, DICTIMAGE_CUSTOM, vpItems));
        }
    AddDictGroup(DICTIMAGE_CUSTOM, _T("Custom"), vstrDictNamesCustom);
    m_rsDict.Open(sql);
}