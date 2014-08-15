#include "StdAfx.h"
#include "StartProcess.h"

/*************Global functions for hosting******************/
//Function to enumerate all windows.
int CALLBACK EnumWindowsProc(HWND hwnd, LPARAM param)
{
	DWORD pID;
	DWORD TpID = GetWindowThreadProcessId(hwnd, &pID);//get process id
	CStartProcess* pCP = (CStartProcess*)param;
	if (TpID == pCP->m_dwThreadId){
		if(pCP->m_szCaption != NULL){
			CString strCaption;
			CWnd::FromHandle(hwnd)->GetWindowText(strCaption);
			if(strCaption.Find(pCP->m_szCaption) == -1)
				return true;
		}
		pCP->m_hwndChildApp = hwnd;//hwnd is the window handle
		return false;
	}
	return true;
}


//Function to start a process and return the process handle
void CStartProcess::StartProcess(LPCTSTR program, LPCTSTR args)
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	if(!::CreateProcess(program, (LPTSTR)args, 
		NULL,  // process security
		NULL,  // thread security
		FALSE, // no inheritance
		0,     // no startup flags
		NULL,  // no special environment
		NULL,  // default startup directory
		&startupInfo,
		&processInfo)) return;

	// success
	m_procChildApp = processInfo.hProcess;
	m_dwThreadId = processInfo.dwThreadId;

	for(int i = 0; i < 60 && m_hwndChildApp == NULL; ++i){
		::Sleep(1000);
		::EnumWindows(&EnumWindowsProc, (LPARAM)this);//Iterate all windows
	}
}
