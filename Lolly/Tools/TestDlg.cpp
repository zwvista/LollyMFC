// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "TestDlg.h"

// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDC_ULTRALINGUA, &CTestDlg::OnBnClickedUltralingua)
	ON_BN_CLICKED(IDC_LINGOES, &CTestDlg::OnBnClickedLingoes)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_EBTEST, &CTestDlg::OnBnClickedEbtest)
END_MESSAGE_MAP()


// CTestDlg message handlers


HWND hwndMain = NULL;

BOOL CALLBACK EnumWindowsProc2(HWND hwnd, LPARAM lParam)
{
	CString strText;
	::GetWindowText(hwnd, strText.GetBufferSetLength(256), 256);
	strText.ReleaseBuffer();
	if(strText == _T("Ultralingua") && ::GetDlgItem(hwnd, 0xE900)){
		hwndMain = hwnd;
		return FALSE;
	}
	return TRUE;
}

void CTestDlg::OnBnClickedUltralingua()
{
	//::EnumWindows(EnumWindowsProc2, 0);
	//HWND hwnd = ::GetDlgItem(hwndMain, 0xE900);
	//hwnd = ::GetDlgItem(hwnd, 0x3F4);

	//CRect rect;
	//::GetWindowRect(hwnd, rect);
	//int x = rect.right - 5;
	//int y = rect.bottom - 5;

	//CString sql;

	//for(;;){
	//	::SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 8));
	//	for(int j = 0; j <= 10; j++)
	//		::SendMessage(hwnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(j * 100, 8));
	//	::SendMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(1000, 8));
	//	::SendMessage(hwndMain, WM_COMMAND, 57634, 0);

	//	CString fromClipboard;
	//	if ( OpenClipboard() ) 
	//	{
	//		HANDLE hData = GetClipboardData( CF_UNICODETEXT );
	//		fromClipboard = (wchar_t *)GlobalLock( hData );
	//		GlobalUnlock( hData );
	//		CloseClipboard();
	//	}
	//	if(fromClipboard.IsEmpty())
	//		continue;
	//	fromClipboard.Replace(_T("'"), _T("''"));
	//	sql.Format(_T("INSERT INTO ESP (ABC) VALUES ('%s')"), fromClipboard);
	//	theApp.m_db.Execute(sql);
	//	if(fromClipboard.Left(6) != _T("zumilo"))
	//		::SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
	//	else
	//		break;
	//}
}

void CTestDlg::OnBnClickedLingoes()
{
	wregex r(_T("10\\.5pt\"><B>(.+?)</B></SPAN> (<SUP style=\"LINE-HEIGHT: normal; FONT-SIZE: 83%\">(.+?)</SUP> )*&nbsp;(<BR>(.+?))*</DIV>[^\\n]+\\n[^\\n]+\\n<DIV style=\"MARGIN: 4px 0px\">(([^\\n]|\\n)+?)</DIV>"));
	LPARAM lParam = 1 | 0x50 << 16 | 1 << 24;

	int a[] = {1, 3, 5, 6};
	CString str, strTranslation, strWord, strWord2, sql;

	m_lst.ResetContent();
	for(;;){
		while(strWord == strWord2){
			::SendMessage(theApp.m_objLingoes.m_hwndEditWord, WM_GETTEXT, (WPARAM)1024, (LPARAM)(LPCTSTR)strWord.GetBufferSetLength(1024));
			strWord.ReleaseBuffer();
		}
		strWord2 = strWord;
		//m_lst.AddString(strWord);

		wstring wstr = theApp.m_objLingoes.GetContent();
		strTranslation.Empty();
		for(wsregex_token_iterator it(wstr.begin(), wstr.end(), r, a), end; it != end;){
			str.Format(_T("<WORD>%s</WORD>"), it++->str().c_str());
			strTranslation += str;
			str.Format(_T("<SUP>%s</SUP>"), it++->str().c_str());
			if(str != _T("<SUP></SUP>"))
				strTranslation += str;
			str.Format(_T("<NOTE>%s</NOTE>"), it++->str().c_str());
			if(str != _T("<NOTE></NOTE>"))
				strTranslation += str;
			str.Format(_T("<TRANSLATION>%s</TRANSLATION>"), it++->str().c_str());
			strTranslation += str;
		}

		sql.Format(_T("INSERT INTO ZLINGOES_KOR (WORD, [TRANSLATION]) VALUES ('%s', '%s')"),
			DoubleApostrophe(strWord), DoubleApostrophe(strTranslation));
		theApp.m_db.Execute(sql);
		
		if(strWord == _T("힘차다"))
			break;
		else
			::PostMessage(theApp.m_objLingoes.m_hwndListWords, WM_KEYDOWN, VK_DOWN, lParam);
	}
}	

void CTestDlg::OnBnClickedButton1()
{
	CADORecordset2 rs(&theApp.m_db);
	rs.Open(_T("SELECT * FROM [@SP-EN WORDREFERENCE]"));
	wregex reg(_T("<SPAN class=clickable.+?>"));
	wstring replacer(_T("<SPAN>"));
	wregex reg2(_T("\\(<A title=conjugation.+?</A>\\)"));
	wstring replacer2;
	wstring wstr, wstr2;
	for(; !rs.IsEof(); rs.MoveNext()){
		wstr = rs.GetFieldValueAsString(_T("TRANSLATION"));
		wstr2 = regex_replace(wstr, reg, replacer);
		wstr2 = regex_replace(wstr2, reg2, replacer2);
		if(wstr2 != wstr){
			rs.Edit();
			rs.SetFieldValue(_T("TRANSLATION"), CString(wstr2.c_str()));
			rs.Update();
		}
	}
}
