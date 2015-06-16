#pragma once

class CDictLangConfig
{
public:
    CDictLangConfig(){}
    CDictLangConfig(const wptree& ptLang, const map<CString, wptree>& mapDictGroupInfo, int nLangID);

    vector<pair<CString, CString>> m_vReplacement;
    vector<CString> m_vstrDictsEBWin;
    vector<CString> m_vstrDictsLingoes;
    map<CString, shared_ptr<CUIDict>> m_mapDictsCustom;
    vector<CString> m_vstrDictsOffline, m_vstrDictTablesOffline;
    CADORecordset2 m_rsDict;
    map<CString, vector<CUIDictItem>> m_mapDictGroups;
    map<CString, CUIDictItem> m_mapDictItems;
};

class CDictConfig
{
public:
    void Load(LPCTSTR lpszURI);
private:
    map<int, CDictLangConfig> m_mapLang2Config;
};

