#include "StdAfx.h"
#include "Lolly.h"
#include "Frhelper.h"

CFrhelper::CFrhelper(void)
	: m_hwndMain(NULL)
{
}

void CFrhelper::FindFrhelper()
{
	if(m_elemHtml.p) return;

	m_hwndMain = ::FindWindow(_T("TFormMainEudic"), NULL);
	if(m_hwndMain == NULL)	return;

	HWND hwndToolbar = ::FindWindowEx(m_hwndMain, NULL, _T("TbsSkinToolBar"), _T(""));
	HWND hwnd = ::FindWindowEx(m_hwndMain, NULL, _T("TbsSkinComboBox"), _T(""));
	m_hwndEditWord = ::FindWindowEx(m_hwndMain, NULL, _T("TbsCustomEdit"), _T(""));

	HWND hwndPnlMain = ::FindWindowEx(m_hwndMain, NULL, _T("TbsSkinPanel"), _T("panelMain"));

	hwnd = ::FindWindowEx(m_hwndMain, NULL, _T("TPanel"), _T("leftPanel"));
	m_hwndListWords = ::FindWindowEx(m_hwndMain, NULL, _T("TVirtualStringTree"), _T(""));

	hwnd = ::FindWindowEx(m_hwndMain, NULL, _T("Shell Embedding"), _T(""));
	hwnd = ::GetDlgItem(hwnd, 0);
	m_hwndHtml = ::GetDlgItem(hwnd, 0);

}

CString CFrhelper::GetContent()
{
	CString strHtml;
	do{
		UINT MSG = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
		DWORD lRes;
		::SendMessageTimeout(m_hwndHtml, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes);
		CComPtr<IHTMLDocument2> doc;
		HRESULT hres = ::ObjectFromLresult(lRes, __uuidof(IHTMLDocument2), 0, (void**)&doc);
		CComPtr<IHTMLElement> body;
		doc->get_body(&body);
		body->get_parentElement(&m_elemHtml);
		CComBSTR bstr;
		m_elemHtml->get_outerHTML(&bstr);
		strHtml = CString(bstr);
	}while(strHtml.IsEmpty());
	return strHtml;
}

CString CFrhelper::Search( LPCTSTR pszWord )
{
	FindFrhelper();
	::SendMessage(m_hwndEditWord, WM_SETTEXT, NULL, (LPARAM)pszWord);
	::PostMessage(m_hwndEditWord, WM_KEYDOWN, VK_RETURN, 0);
	::PostMessage(m_hwndEditWord, WM_KEYUP, VK_RETURN, 0);
	::Sleep(400);
	CString strLastWord, strDictWord, strText, strWord = pszWord;
	for(;;){
		strText = GetContent();
		::SendMessage(m_hwndEditWord, WM_GETTEXT, (WPARAM)1024, (LPARAM)(LPCTSTR)strDictWord.GetBufferSetLength(1024));
		strDictWord.ReleaseBuffer();
		if(!strDictWord.CompareNoCase(strWord) || strDictWord == strLastWord) break;
		strLastWord = strDictWord;
		::PostMessage(m_hwndListWords, WM_KEYDOWN, VK_DOWN, 0);
		::PostMessage(m_hwndListWords, WM_KEYUP, VK_DOWN, 0);
		::Sleep(400);
	}
	return strDictWord == strLastWord ? CString() : strText;
}

CString CFrhelper::Search( LPCTSTR pszWord, LPCTSTR pszTransform )
{
	CString strText = Search(pszWord);
	return strText;
}
