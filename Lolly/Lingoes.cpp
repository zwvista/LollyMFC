#include "StdAfx.h"
#include "Lolly.h"
#include "Lingoes.h"

CLingoes::CLingoes(void)
	: m_hwndMain(NULL)
{
}

void CLingoes::FindLingoes()
{
	if(m_elemHtml.p) return;

	m_hwndMain = ::FindWindow(theApp.m_strLingoesClassName, theApp.m_strLingoesWindowName);
	if(m_hwndMain == NULL)	return;

	HWND hwndDlg = ::GetDlgItem(m_hwndMain, 0);
	m_hwndEditWord = ::GetDlgItem(hwndDlg, 0x67);
	m_hwndButtonSearch = ::GetDlgItem(hwndDlg, 0x68);

	hwndDlg = ::GetWindow(hwndDlg, GW_HWNDNEXT);
	m_hwndListWords = ::GetDlgItem(hwndDlg, 0x3F7);

	hwndDlg = ::GetWindow(hwndDlg, GW_HWNDNEXT);
	HWND hwnd = ::GetDlgItem(hwndDlg, 0x71);
	hwnd = ::GetDlgItem(hwnd, 0);
	hwnd = ::GetDlgItem(hwnd, 0);
	hwnd = ::GetDlgItem(hwnd, 0);
	UINT MSG = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	DWORD lRes;
	::SendMessageTimeout(hwnd, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes);
	CComPtr<IHTMLDocument2> doc;
	HRESULT hres = ::ObjectFromLresult(lRes, __uuidof(IHTMLDocument2), 0, (void**)&doc);
	CComPtr<IHTMLElement> body;
	doc->get_body(&body);
	body->get_parentElement(&m_elemHtml);
}

CString CLingoes::GetContent()
{
	CComBSTR bstr;
	m_elemHtml->get_outerHTML(&bstr);
	return CString(bstr);
}

CString CLingoes::Search( LPCTSTR pszWord )
{
	FindLingoes();
	::SendMessage(m_hwndEditWord, WM_SETTEXT, NULL, (LPARAM)pszWord);
	::SendMessage(m_hwndButtonSearch, BM_CLICK, 0, 0);
	return GetContent();
}

CString CLingoes::Search( LPCTSTR pszWord, vector<CString> vstrDicts )
{
	FindLingoes();

	const CString strMainWnd = _T("<DIV id=main_wnd>\r\n");
	const CString strDictArea = _T("<DIV id=lingoes_dictarea></DIV>\r\n");
	const CString strFoot = _T("<DIV style=\"PADDING-BOTTOM: 10px; LINE-HEIGHT: normal;");
	const CString strAd = _T("<DIV style=\"LINE-HEIGHT: normal; OVERFLOW-X: hidden;");

	CString strText = Search(pszWord);

	int p = strText.Find(strMainWnd) + strMainWnd.GetLength();
	CString strResult = strText.Left(p);
	strText = strText.Mid(p);

	CString str;
	bool bFoundOne = false;
	do{
		p = strText.Find(strDictArea);
		if(p == -1)
			str = strText, strText.Empty();
		else
			str = strText.Left(p), strText.Delete(0, p + strDictArea.GetLength());
		bool bFound = false;
		for(auto it = vstrDicts.begin(); it != vstrDicts.end(); ++it)
			if(str.Find(*it) != -1){
				bFound = true;
				bFoundOne = true;
				vstrDicts.erase(it);
				break;
			}
		if(bFound){
			if(strText.IsEmpty()){
				p = str.Find(strFoot);
				if(p != -1)
					str = str.Left(p);
			}
			p = str.Find(strAd);
			if(p != -1)
				str = str.Left(p) + str.Mid(str.Find(_T("</DIV>"), p) + 6);
			strResult += strDictArea + str;
		}
	}while(!strText.IsEmpty() && !vstrDicts.empty());
	if(bFoundOne)
		strResult += _T("</DIV></DIV></BODY></HTML>");
	else
		strResult.Format(_T("<HTML><BODY>%s</BODY></HTML>"), theApp.m_strNoTrans);
	return strResult;
}
