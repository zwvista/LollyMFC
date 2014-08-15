#pragma once

class CFrhelper
{
public:
	CFrhelper();
	void FindFrhelper();
	CString GetContent();
	CString Search(LPCTSTR pszWord);
	CString Search(LPCTSTR pszWord, LPCTSTR pszTransform);
	HWND m_hwndEditWord, m_hwndListWords, m_hwndHtml;
private:
	HWND m_hwndMain;
	CComPtr<IHTMLElement> m_elemHtml;
};
