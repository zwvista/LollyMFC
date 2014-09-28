#include "StdAfx.h"
#include "Lolly.h"
#include "UTF8.h"

const CString CLollyMixin::m_strNoTrans = _T("<p style=\"color: #0000FF; font-weight: bold\">No translations were found.</p>");

void SLangBookUnitSettings::Init()
{
	CADORecordset2 rs(&theApp.m_db);
	CString sql;

	sql.Format(_T("SELECT * FROM LANGUAGES WHERE LANGID=%d"), nLangID);
	rs.Open(sql);
	strLangName = rs.GetFieldValueAsString(_T("CHNNAME"));
	nBookID = rs.GetFieldValueAsInt(_T("CURBOOKID"));

	sql.Format(_T("SELECT * FROM BOOKS WHERE BOOKID=%d"), nBookID);
	rs.Open(sql);
	strBookName = rs.GetFieldValueAsString(_T("BOOKNAME"));
	nUnitFrom = rs.GetFieldValueAsInt(_T("UNITFROM"));
	nPartFrom = rs.GetFieldValueAsInt(_T("PARTFROM"));
	nUnitTo = rs.GetFieldValueAsInt(_T("UNITTO"));
	nPartTo = rs.GetFieldValueAsInt(_T("PARTTO"));
}

void CLollyMixin::ComboBox_FillWithRS( CComboBox& cbo, CADORecordset2& rs, LPCTSTR pszDisplayField, LPCTSTR pszValueField /*= NULL*/ )
{
	if(rs.GetRecordCount() == 0) return;
	for(rs.MoveFirst(); !rs.IsEof(); rs.MoveNext()){
		cbo.AddString(rs.GetFieldValueAsString(pszDisplayField));
		if(pszValueField != NULL)
			cbo.SetItemData(cbo.GetCount() - 1, rs.GetFieldValueAsInt(pszValueField));
	}
}

void CLollyMixin::ComboBox_SelectByValue( CComboBox& cbo, int nValue )
{
	int i;
	for(i = cbo.GetCount() - 1; i >= 0; i--)
		if(cbo.GetItemData(i) == nValue)
			break;
	cbo.SetCurSel(i);
}

CString CLollyMixin::AutoCorrect( LPCTSTR pszText, CADORecordset2& rsAutoCorrect )
{
	return AutoCorrect(pszText, rsAutoCorrect, _T("INPUT"), _T("EXTENDED"));
}

CString CLollyMixin::AutoCorrect( LPCTSTR pszText, CADORecordset2& rsAutoCorrect, LPCTSTR pszCol1, LPCTSTR pszCol2 )
{
	CString str = pszText;
	if(rsAutoCorrect.GetRecordCount() > 0)
		for(rsAutoCorrect.MoveFirst(); !rsAutoCorrect.IsEof(); rsAutoCorrect.MoveNext())
			str.Replace(rsAutoCorrect.GetFieldValueAsString(pszCol1), rsAutoCorrect.GetFieldValueAsString(pszCol2));
	return str;
}

//typedef unsigned char BYTE;
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

CString EncodeToCodePage(UINT CodePage, CString strUnicode)
{
	LPCWSTR unicode = T2CW(strUnicode);
	int len = WideCharToMultiByte(CodePage, 0, unicode, -1, 0, 0, 0, 0);
	if (!len)
		return strUnicode;
	// 先將 unicode 資料轉換成 UTF-8
	char *utf8 = new char[len + 1];
	char *utf8temp = utf8;
	WideCharToMultiByte(CodePage, 0, unicode, -1, utf8, len + 1, 0, 0);
	utf8[len] = NULL;   
	// 再將 UTF-8 的資料以16進制字串的方式呈現
	// 並且組成 UrlEncode 的格式(每組16進制字串前面加上%) => %E7%BE%8E%E9%A3%9F
	CString strTemp, strEncodeData; 
	while (*utf8 != '\0') 
	{ 
		strTemp.Format(_T("%%%2x"), (BYTE)*utf8); 
		strEncodeData += strTemp; 
		++utf8; 
	} 

	delete []utf8temp;

	return CString(strEncodeData);
}

CString CLollyMixin::GetDictURLForWord( LPCTSTR pszWord, CADORecordset2& rsDict, CADORecordset2& rsAutoCorrect )
{
	CString strConv = rsDict.GetFieldValueAsString(_T("CHCONV"));
	CString strWord =
		strConv == _T("UTF-8") ? EncodeToUTF8(pszWord) :
		strConv == _T("BASIC") ? AutoCorrect(pszWord, rsAutoCorrect, _T("EXTENDED"), _T("BASIC")) :
		strConv == _T("INPUT") ? AutoCorrect(pszWord, rsAutoCorrect, _T("EXTENDED"), _T("INPUT")) :
		strConv == _T("Korean") ? EncodeToCodePage(949, pszWord) :
		strConv == _T("Western European") ? EncodeToCodePage(28591, pszWord) :
		strConv == _T("Russian") ? EncodeToCodePage(1251, pszWord) :
		pszWord;
	CString strURL = rsDict.GetFieldValueAsString(_T("URL"));
	strURL.Replace(_T("{0}"), strWord);
	return strURL;
}

CString ExtractFromHtml(LPCTSTR pszText, LPCTSTR pszTransform, LPCTSTR pszDefault /* = _T("") */)
{
	wstring strText = pszText;

	CString strTransform = pszTransform;
	vector<CString> vstrs;
	SplitString(strTransform, _T("\r\n"), vstrs);
	ASSERT(vstrs.size() % 2 == 0);
	wregex reg(vstrs[0]);
	wsregex_token_iterator it(strText.begin(), strText.end(), reg, 1), end;
	if(it == end) return _T("");

	strText = it->str();
	auto f = [&](CString strReplacer){
		strReplacer.Replace(_T("\\r"), _T("\r"));
		strReplacer.Replace(_T("\\n"), _T("\n"));
		if(strReplacer == _T("<delete>"))
			strReplacer = _T("");
		strText = regex_replace(strText, reg, wstring(strReplacer));
	};

	f(vstrs[1]);
	if(vstrs.size() > 2)
		for(size_t i = 2; i < vstrs.size();){
			wregex reg(vstrs[i++]);
			f(vstrs[i++]);
		}
	
	CString strText2 = strText.c_str();
	return strText2.IsEmpty() ? CString(pszDefault) : strText2;
}

CString CLollyMixin::ExtractFromWeb(CHtmlView* pView, CADORecordset2& rsExtract, CString strDefault /* = _T("") */)
{
	CString strText = GetDocumentText(pView);
	CString strTransform = rsExtract.GetFieldValueAsString(_T("TRANSFORM_WIN"));
	return ExtractFromHtml(strText, strTransform, strDefault);
}

bool CLollyMixin::DoWebAutomation( CHtmlView* pView, const CString& strAutomation, LPCTSTR lpWord )
{
	CComQIPtr<IHTMLDocument2> doc = pView->GetHtmlDocument();
	CComPtr<IHTMLElementCollection> all;
	doc->get_all(&all);

	vector<CString> vstrs, vstrs2;
	SplitString(strAutomation, _T("\r\n"), vstrs);
	for(const CString& str : vstrs){
		SplitString(str, _T(","), vstrs2);
		const CString& strElemType = vstrs2[0];
		const CString& strElemAttrName = vstrs2[1];
		const CString& strElemAttr = vstrs2[2];
		CComDispatchDriver item;
		CComQIPtr<IHTMLElement> elem;
		if(strElemAttrName.IsEmpty()){
			all->item(CComVariant(strElemAttr), CComVariant(0), &item);
			elem = item;
		}
		else{
			long len;
			all->get_length(&len);
			for(long i = 0; i < len; ++i){
				CComBSTR bstr;
				all->item(CComVariant(i), CComVariant(0), &item);
				elem = item;
				if(strElemAttrName == _T("class")){
					elem->get_className(&bstr);
					if(CString(bstr) == strElemAttr)
						break;
				}
				else if(strElemAttrName == _T("title")){
					elem->get_title(&bstr);
					if(CString(bstr) == strElemAttr)
						break;
				}
				else if(strElemAttrName == _T("href")){
					CComQIPtr<IHTMLAnchorElement> elem2 = elem;
					if(elem2){
						elem2->get_href(&bstr);
						if(CString(bstr).Right(strElemAttr.GetLength()) == strElemAttr)
							break;
					}
				}
				item.Release();
			}
		}
		if(!elem)
			return false;
		if(strElemType == _T("Anchor") || strElemType == _T("Button"))
			elem->click();
		else if(strElemType == _T("Input")){
			CComQIPtr<IHTMLInputElement> elem2 = elem;
			elem2->put_value(CComBSTR(lpWord));
		}
		else if(strElemType == _T("OptionButton")){
			CComQIPtr<IHTMLOptionButtonElement> elem2 = elem;
			elem2->put_value(CComBSTR(vstrs2[3]));
		}
		else if(strElemType == _T("Select")){
			CComQIPtr<IHTMLSelectElement> elem2 = elem;
			elem2->put_value(CComBSTR(vstrs2[3]));
		}
	}
	return true;
}

CString CLollyMixin::GetLiveHtml( LPCTSTR pszWord, LPCTSTR pszDict, bool bUpperCase )
{
	CString str;
	str.Format(bUpperCase ?
		_T("<P style=\"FONT-WEIGHT: bold; COLOR: rgb(0,0,255)\">Extracting translation of the word \"%s\" from the web dictionary \"%s\"</P>") :
		_T("<p style=\"color: rgb(0, 0, 255); font-weight: bold;\">Extracting translation of the word \"%s\" from the web dictionary \"%s\"</p>"),
		pszWord, pszDict);
	return str;
}

CString GetTextFromClipBoard()
{
	CString str;
	if(OpenClipboard(NULL)){
		HANDLE hData = ::GetClipboardData( CF_UNICODETEXT );
		str = (wchar_t *)::GlobalLock( hData );
		::GlobalUnlock( hData );
		::CloseClipboard();
	}
	return str;
}

void CopyTextToClipBoard(const CString& strText) 
{ 
	HGLOBAL h; 
	LPTSTR arr; 

	size_t bytes = (strText.GetLength()+1)*sizeof(TCHAR); 
	h=GlobalAlloc(GMEM_MOVEABLE, bytes); 
	arr=(LPTSTR)GlobalLock(h); 
	ZeroMemory(arr,bytes); 
	_tcscpy_s(arr, strText.GetLength()+1, strText); 
	GlobalUnlock(h); 

	::OpenClipboard (NULL); 
	EmptyClipboard(); 
	SetClipboardData(CF_UNICODETEXT, h); 
	CloseClipboard(); 
} 

void SplitString( const CString& strText, LPCTSTR pszDelim, vector<CString>& vstrs )
{
	vstrs.clear();
	for(int p1 = 0, p2 = 0;;){
		p2 = strText.Find(pszDelim, p1);
		if(p2 == -1){
			vstrs.push_back(strText.Mid(p1));
			break;
		}
		vstrs.push_back(strText.Mid(p1, p2 - p1));
		p1 = p2 + _tcslen(pszDelim);
	}
}

CString GetDocumentText( CHtmlView* pView )
{
	CComQIPtr<IHTMLDocument2> doc = pView->GetHtmlDocument();
	CComPtr<IHTMLElement> body;
	CComBSTR bstr;
	doc->get_body(&body);
	CComPtr<IHTMLElement> parent;
	body->get_parentElement(&parent);
	parent->get_outerHTML(&bstr);
	return CString(bstr);
}

CString GetDocumentTitle( CHtmlView* pView )
{
	CComQIPtr<IHTMLDocument2> doc = pView->GetHtmlDocument();
	CComBSTR bstr;
	doc->get_title(&bstr);
	return CString(bstr);
}

void CLollyMixin::OpenDictTable( const CString& strWord, CADORecordset2& rsWord, CADORecordset2& rsDict )
{
	CString sql;
	sql.Format(_T("SELECT * FROM [%s] WHERE WORD=N'%s'"),
		rsDict.GetFieldValueAsString(_T("DICTTABLE")), DoubleApostrophe(strWord));
	rsWord.Open(sql);
	if(rsWord.IsEof()){
		rsWord.AddNew();
		rsWord.SetFieldValue(_T("WORD"), strWord);
		rsWord.Update();
	}
}

void CLollyMixin::UpdateDictTable( CHtmlView* pHtmlView, CADORecordset2& rsWord, CADORecordset2& rsDict, bool bAppend )
{
	CString strText = ExtractFromWeb(pHtmlView, rsDict, bAppend ? CString() : m_strNoTrans); 

	if(bAppend)
		strText = rsWord.GetFieldValueAsString(_T("TRANSLATION")) + strText;

	rsWord.Edit();
	rsWord.SetFieldValue(_T("TRANSLATION"), strText);
	rsWord.Update();
}

void CLollyMixin::InitVoices()
{
	CADORecordset2 rs(&m_db);
	rs.Open(_T("SELECT * FROM LANGUAGES ORDER BY LANGID"));
	m_vcpVoices.resize(rs.GetRecordCount());
	CString strAttr;
	for(; !rs.IsEof(); rs.MoveNext()){
		CString strVoice = rs.GetFieldValueAsString(_T("VOICE"));
		int nLangID = rs.GetFieldValueAsInt(_T("LANGID"));
		m_vstrVoiceNames.push_back(strVoice);
		CComPtr<ISpObjectToken> cpToken;
		strAttr.Format(_T("Name=%s"), strVoice);
		if(SUCCEEDED(SpFindBestToken(SPCAT_VOICES, strAttr, NULL, &cpToken))){
			m_vcpVoices[nLangID].m_T.CoCreateInstance(CLSID_SpVoice);
			m_vcpVoices[nLangID].m_T->SetVoice(cpToken);
		}
	}
}

CString CLollyMixin::GetVoiceXml( int nLangID, LPCTSTR psz )
{
	CString strResult;
	strResult.Format(_T("<VOICE REQUIRED='NAME=%s'/>%s"), m_vstrVoiceNames[nLangID], psz);
	return strResult;
}

void CLollyMixin::Speak( int nLangID, LPCTSTR psz, bool bPurge /*= true*/ )
{
	HRESULT hr = S_OK;
	DWORD dwFlags = SPF_ASYNC;
	if(bPurge)
		dwFlags |= SPF_PURGEBEFORESPEAK;
	hr = m_vcpVoices[nLangID].m_T->Speak(psz, dwFlags, NULL);
}

bool CLollyMixin::CanSpeak( int nLangID )
{
	return m_vcpVoices[nLangID].m_T != NULL;
}

CString ReadAllTextFromFile(LPCTSTR pszFileName)
{
	CString strText;
	CStdioFileEx f;
	f.SetCodePage(65001);
	f.Open(pszFileName, CFile::modeRead | CFile::typeText);
	CString strLine;
	while(f.ReadString(strLine))
		strText += strLine + _T("\r\n");
	strText.TrimRight(_T("\r\n"));
	return strText;
}

wptree CLollyMixin::GetConfig(int nLangID)
{
	return boost::find_if(m_ptConfig, [=](const wptree::value_type& v){
		return v.first == _T("language") && v.second.get<int>(_T("<xmlattr>.id")) == nLangID;
	})->second;
}

wptree CLollyMixin::GetConfigDicts(int nLangID)
{
	return GetConfig(nLangID).get_child(_T("dictionaries"));
}
