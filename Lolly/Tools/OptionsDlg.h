#pragma once


// COptionsDlg ダイアログ

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~COptionsDlg();

// ダイアログ データ
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
