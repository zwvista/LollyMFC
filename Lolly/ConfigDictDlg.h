#pragma once


// CConfigDictDlg �_�C�A���O

class CConfigDictDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDictDlg)

public:
	CConfigDictDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CConfigDictDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CONFIG_DICT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
