#pragma once


struct SLangBookUnitSettings
{
    int nLangID, nBookID, nUnitFrom, nPartFrom, nUnitTo, nPartTo;
    CString strLangName, strBookName;
    void Init();
    int GetUnitPartFrom() const { return nUnitFrom * 10 + nPartFrom; }
    int GetUnitPartTo() const { return nUnitTo * 10 + nPartTo; }
    CString GetBookUnitsDesc() const {
        CString str;
        if(GetUnitPartFrom() == GetUnitPartTo())
            str.Format(_T("%s %d:%d"), strBookName, nUnitFrom, nPartFrom);
        else
            str.Format(_T("%s %d:%d--%d:%d"), strBookName, nUnitFrom, nPartFrom, nUnitTo, nPartTo);
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
CString GetDocumentText(CHtmlView* pView);
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

class CUIDict
{
public:
    CUIDict(){}
    virtual ~CUIDict(){}
};

class CUIDictItem : public CUIDict
{
public:
    CUIDictItem(const CString& n, const CString& t, EDictImage e)
        : m_strName(n), m_strType(t), m_ImageIndex(e) {}

    CString m_strName;
    CString m_strType;
    EDictImage m_ImageIndex;
};

class CUIDictCollection : public CUIDict
{
public:
    bool m_bIsPile;
    CString m_strName;
    vector<CUIDictItem> m_Items;
};
