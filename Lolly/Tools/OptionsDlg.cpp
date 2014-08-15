// OptionsDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Lolly.h"
#include "OptionsDlg.h"


// COptionsDlg ダイアログ

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{

}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()


// COptionsDlg メッセージ ハンドラ
