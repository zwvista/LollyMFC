#pragma once

class CDictLangConfig
{
public:
    CDictLangConfig(const wptree& ptLang, const map<CString, wptree>& mapDictGroupInfo, int nLangID);

    vector<pair<CString, CString>> m_vReplacement;
    vector<CString> m_vstrDictsEBWin;
    vector<CString> m_vstrDictsLingoes;
    map<CString, CUIDict> m_mapDictsCustom;
    vector<CString> m_vstrDictsOffline, m_vstrDictTablesOffline;
    vector<CADORecordset2> m_vrsDictAll;
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

