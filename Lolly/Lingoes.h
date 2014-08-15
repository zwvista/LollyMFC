#pragma once

class CLingoes
{
public:
	CLingoes();
	void FindLingoes();
	CString GetContent();
	CString Search(LPCTSTR pszWord);
	CString Search(LPCTSTR pszWord, vector<CString> vstrDicts);
	HWND m_hwndEditWord, m_hwndButtonSearch, m_hwndListWords;
private:
	HWND m_hwndMain;
	CComPtr<IHTMLElement> m_elemHtml;
};
