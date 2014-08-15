// Text2PostDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "Text2PostDlg.h"
#include "afxdialogex.h"


// CText2PostDlg dialog

IMPLEMENT_DYNAMIC(CText2PostDlg, CDialog)

CText2PostDlg::CText2PostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CText2PostDlg::IDD, pParent)
{

}

CText2PostDlg::~CText2PostDlg()
{
}

void CText2PostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ORIG_TEXT, m_edtOrigText);
	DDX_Control(pDX, IDC_EDIT_NEW_POST, m_edtNewPost);
	DDX_Control(pDX, IDC_COMBO_PARAGRAPH_END, m_cboParagraphEnd);
}


BEGIN_MESSAGE_MAP(CText2PostDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLYPOSTFORMAT, &CText2PostDlg::OnBnClickedButtonApplypostformat)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CText2PostDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CText2PostDlg message handlers


BOOL CText2PostDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cboParagraphEnd.AddString(_T("Line Breaks"));
	m_cboParagraphEnd.AddString(_T("Blank Lines"));
	m_cboParagraphEnd.SetCurSel(1);

	m_resizer.Hook(this);
	m_resizer.SetShowResizeGrip(TRUE);
	CRect rc;
	GetClientRect(rc);
	m_resizer.SetMinimumSize(_T("_root"), rc.Size());

	m_resizer.CreateSplitContainer(_T("splitA"), IDC_LABEL1, IDC_EDIT_ORIG_TEXT);
	m_resizer.SetFixedPanel(_T("splitA"), 1);
	m_resizer.SetIsSplitterFixed(_T("splitA"), TRUE);
	m_resizer.CreateSplitContainer(_T("splitB"), IDC_LABEL2, IDC_EDIT_NEW_POST);
	m_resizer.SetFixedPanel(_T("splitB"), 1);
	m_resizer.SetIsSplitterFixed(_T("splitB"), TRUE);
	m_resizer.CreateSplitContainer(_T("splitMain"), _T("splitA"), _T("splitB"));
	m_resizer.SetIsSplitterFixed(_T("splitMain"), TRUE);
	m_resizer.SetAnchor(_T("splitMain"), ANCHOR_ALL);

	m_resizer.SetAnchor(IDC_LABEL3, ANCHOR_LEFT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDC_COMBO_PARAGRAPH_END, ANCHOR_LEFT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDC_BUTTON_APPLYPOSTFORMAT, ANCHOR_LEFT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDC_BUTTON_SAVE, ANCHOR_LEFT | ANCHOR_BOTTOM);
	m_resizer.SetAnchor(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	CString strBlogFolder = theApp.m_strAppDataFolder + _T("blog\\");
	m_strFmtPost = ReadAllTextFromFile(strBlogFolder + _T("PostFormat.txt"));
	m_strFmtPost.Replace(_T("{0}"), _T("%s"));
	m_strFmtParagraph1 = ReadAllTextFromFile(strBlogFolder + _T("ParagraphFormat1.txt"));
	m_strFmtParagraph1.Replace(_T("{0}"), _T("%s"));
	m_strFmtParagraph2 = ReadAllTextFromFile(strBlogFolder + _T("ParagraphFormat2.txt"));
	m_strFmtParagraph2.Replace(_T("{0}"), _T("%s"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CText2PostDlg::OnBnClickedButtonApplypostformat()
{
	CString strPs = m_cboParagraphEnd.GetCurSel() == 0 ?
		GetPostStrFormat1() : GetPostStrFormat2();
	strPs.TrimRight(_T("\r\n"));
	CString strNewPost;
	strNewPost.Format(m_strFmtPost, strPs);
	m_edtNewPost.SetWindowText(strNewPost);
}

void CText2PostDlg::OnBnClickedButtonSave()
{
	CFileDialog dlg(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files(*.xml)|*.xml||"));
	if(dlg.DoModal() == IDOK){
		CStdioFileEx f;
		f.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite);
		f.SetCodePage(65001);
		CString str;
		m_edtNewPost.GetWindowText(str);
		f.WriteString(str);
	}
}

CString CText2PostDlg::GetPostStrFormat1()
{
	CString strLine, strP, strPs;
	int nLineCount = m_edtOrigText.GetLineCount();
	for(int i = 0; i < nLineCount; ++i){
		int len = m_edtOrigText.LineLength(m_edtOrigText.LineIndex(i));
		m_edtOrigText.GetLine(i, strLine.GetBuffer(len), len);
		strLine.ReleaseBuffer(len);
		strLine.Trim();
		if(!strLine.IsEmpty()){
			strP.Format(m_strFmtParagraph1, HtmlEncode(strLine));
			strPs += strP + _T("\r\n");
		}
	}
	return strPs;
}

CString CText2PostDlg::GetPostStrFormat2()
{
	CString strText, strP, strPs;
	m_edtOrigText.GetWindowText(strText);
	wstring strText2(HtmlEncode(strText));
	wregex regBlankLine(_T("(?:^|\\r\\n)(?:\\s*(?:$|\\r\\n))+"));
	wsregex_token_iterator it(strText2.begin(), strText2.end(), regBlankLine, -1), end;
	while(it != end){
		CString strG(it++->str().c_str());
		if(strG.IsEmpty()) continue;
		vector<CString> vstrLines;
		SplitString(strG, _T("\r\n"), vstrLines);
		for(size_t i = 0; i < vstrLines.size() - 1; ++i){
			strP.Format(m_strFmtParagraph2, vstrLines[i].Trim());
			strPs += strP + _T("\r\n");
		}
		strP.Format(m_strFmtParagraph1, vstrLines.back().Trim());
		strPs += strP + _T("\r\n");
	}
	return strPs;
}
