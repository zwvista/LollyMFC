#include "stdafx.h"
#include "DictConfig.h"
#include "Lolly.h"

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
{
    // replacement
    for(const auto& v : ptLang)
        if(v.first == _T("phraseReplace"))
            m_vReplacement.emplace_back(
                CString(v.second.get<wstring>(_T("<xmlattr>.before")).c_str()),
                CString(v.second.get<wstring>(_T("<xmlattr>.after")).c_str()));

    auto& ptDicts = ptLang.get_child(_T("dictionaries"));
    if(ptDicts.empty()) return;

    // ebwin
    for(const auto& v : ptDicts)
        if(v.first == _T("ebwin"))
            m_vstrDictsEBWin.push_back(v.second.data().c_str());

    auto AddDictGroups = [&](EDictImage nDictImage, const CString& strGroupName, const vector<CString>& vstrDictNames){
        if(vstrDictNames.empty()) return;

        auto& vobjItems = m_mapDictGroups[strGroupName];

        int nImageIndex = nDictImage;
        for(auto& strDictName : vstrDictNames){
            CUIDictItem objItem(strDictName, strGroupName, (EDictImage)nImageIndex);
            vobjItems.push_back(objItem);
            m_mapDictItems[strGroupName + strDictName] = objItem;
            if(nDictImage == DICTIMAGE_ONLINE || nDictImage == DICTIMAGE_OFFLINE || nDictImage == DICTIMAGE_LIVE)
                ++nImageIndex;
        }
    };

    // group
    CADORecordset2 rsDict(&theApp.m_db);
    CString sql;
    sql.Format(_T("SELECT * FROM DICTALL WHERE LANGID=%d "), nLangID);
    for(const auto& v : ptDicts)
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
            rsDict.Open(sql2);
            if(rsDict.GetRecordCount() != 0)
                for(rsDict.MoveFirst(); !rsDict.IsEof(); rsDict.MoveNext()){
                    auto strDict = rsDict.GetFieldValueAsString(_T("DICTNAME"));
                    vstrDictNames.push_back(strDict);
                    if(strGroupName == DICT_OFFLINE){
                        m_vstrDictsOffline.push_back(strDict);
                        m_vstrDictTablesOffline.push_back(rsDict.GetFieldValueAsString(_T("DICTTABLE")));
                    }
                }
            AddDictGroups(nDictImage, strGroupName, vstrDictNames);
        }

    // lingoes + special
    vector<CString> vstrSpecialDicts;
    for(const auto& v : ptDicts)
        if(v.first == _T("lingoes"))
            m_vstrDictsLingoes.push_back(v.second.data().c_str());
        else if(v.first == _T("special"))
            vstrSpecialDicts.push_back(v.second.data().c_str());
    if(!m_vstrDictsLingoes.empty())
        vstrSpecialDicts.push_back(_T("Lingoes"));
    boost::sort(vstrSpecialDicts);
    AddDictGroups(DICTIMAGE_SPECIAL, _T("Special"), vstrSpecialDicts);

    // custom
    vector<CString> vstrDictNamesCustom;
    for(const auto& v : ptDicts)
        if(v.first == _T("custom")){
            const auto& ptCustom = v.second;
            CString strDictName = ptCustom.get<wstring>(_T("<xmlattr>.name")).c_str();
            vstrDictNamesCustom.push_back(strDictName);
            auto& vDicts = m_mapDictsCustom[strDictName];
            for(const auto& v2 : ptCustom)
                if(v2.first == _T("dict"))
                vDicts.push_back(DictInfo(
                    v2.second.get<wstring>(_T("<xmlattr>.type")).c_str(),
                    v2.second.data().c_str()));
        }
    AddDictGroups(DICTIMAGE_CUSTOM, _T("Custom"), vstrDictNamesCustom);
}