#pragma once


// CConfigDictDlg ダイアログ

class CConfigDictDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDictDlg)

public:
	CConfigDictDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CConfigDictDlg();

// ダイアログ データ
	enum { IDD = IDD_CONFIG_DICT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
