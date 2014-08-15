#pragma once

class CStartProcess
{
public:
	CStartProcess(LPCTSTR szCaption = NULL) 
		: m_hwndChildApp(NULL), m_procChildApp(NULL), m_dwThreadId(NULL)
		, m_szCaption(szCaption) {}

	void StartProcess(LPCTSTR program, LPCTSTR args);

	HWND m_hwndChildApp; //window handle
	HANDLE m_procChildApp; //process handle
	DWORD m_dwThreadId;
	LPCTSTR m_szCaption;
};
