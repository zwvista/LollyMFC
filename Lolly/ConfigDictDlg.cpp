// ConfigDictDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Lolly.h"
#include "ConfigDictDlg.h"
#include "afxdialogex.h"


// CConfigDictDlg ダイアログ

IMPLEMENT_DYNAMIC(CConfigDictDlg, CDialog)

CConfigDictDlg::CConfigDictDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDictDlg::IDD, pParent)
{

}

CConfigDictDlg::~CConfigDictDlg()
{
}

void CConfigDictDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDictDlg, CDialog)
END_MESSAGE_MAP()


// CConfigDictDlg メッセージ ハンドラー
