#pragma once

class CFrhelper
{
public:
	CFrhelper();
	void FindFrhelper();
	CString GetContent();
	CString Search(LPCTSTR pszWord);
	CString Search(LPCTSTR pszWord, LPCTSTR pszTransform);
    HWND m_hwndEditWord = NULL;
	HWND m_hwndListWords = NULL;
	HWND m_hwndHtml = NULL;
private:
	HWND m_hwndMain = NULL;
};
