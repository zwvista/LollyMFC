
// PhrasesBaseFrame.cpp : implementation of the CPhrasesBaseFrame class
//

#include "stdafx.h"
#include "Lolly.h"
#include "PhrasesBaseFrame.h"
#include "NewPhrasesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhrasesBaseFrame

IMPLEMENT_DYNCREATE(CPhrasesBaseFrame, CLollyFrameGridOnly)

BEGIN_MESSAGE_MAP(CPhrasesBaseFrame, CLollyFrameGridOnly)
	ON_MESSAGE(WM_LBLSETTINGS_CHANGED, OnLblSettingsChanged)
	ON_COMMAND(ID_TB_SPEAK, OnSpeak)
	ON_COMMAND(ID_TB_KEEPSPEAK, OnKeepSpeak)
	ON_UPDATE_COMMAND_UI(ID_TB_KEEPSPEAK, OnUpdateKeepSpeak)
	ON_COMMAND(ID_TB_SPEAKTRANS, OnSpeakTrans)
	ON_COMMAND(ID_TB_KEEPSPEAKTRANS, OnKeepSpeakTrans)
	ON_UPDATE_COMMAND_UI(ID_TB_KEEPSPEAKTRANS, OnUpdateKeepSpeakTrans)
	ON_COMMAND(ID_TB_MULTIADD, OnMultiAdd)
END_MESSAGE_MAP()

// CPhrasesBaseFrame construction/destruction

CPhrasesBaseFrame::CPhrasesBaseFrame()
	: m_bKeepSpeak(false)
	, m_bKeepSpeakTrans(false)
{
}

LRESULT CPhrasesBaseFrame::OnLblSettingsChanged(WPARAM wParam, LPARAM lParam)
{
	CLollyFrameGridOnly::OnLblSettingsChanged(wParam, lParam);

	auto GetReplacement = [](int nLangID, vector<pair<CString, CString>>& replacement){
	};

	GetReplacement(m_lbuSettings.nLangID, m_vReplacement);
	GetReplacement(0, m_vReplacementChn);

	return 0;
}

CPhrasesBaseFrame::~CPhrasesBaseFrame()
{
}

void CPhrasesBaseFrame::OnMoveComplete()
{
	CLollyFrameGridOnly::OnMoveComplete();

	if(m_bKeepSpeak || m_bKeepSpeakTrans)
		SpeakPhraseTrans(m_bKeepSpeak, m_bKeepSpeakTrans);
}

void CPhrasesBaseFrame::OnSpeak()
{
	SpeakPhraseTrans(true, false);
}

void CPhrasesBaseFrame::OnKeepSpeak()
{
	if(m_bKeepSpeak = !m_bKeepSpeak)
		OnSpeak();
}

void CPhrasesBaseFrame::OnUpdateKeepSpeak( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(m_bKeepSpeak);
}

void CPhrasesBaseFrame::OnSpeakTrans()
{
	SpeakPhraseTrans(false, true);
}

void CPhrasesBaseFrame::OnKeepSpeakTrans()
{
	if(m_bKeepSpeakTrans = !m_bKeepSpeakTrans)
		OnSpeakTrans();
}

void CPhrasesBaseFrame::OnUpdateKeepSpeakTrans( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(m_bKeepSpeakTrans);
}

void CPhrasesBaseFrame::SpeakPhraseTrans( bool bPhrase, bool bTrans )
{
	if(m_rs.IsEof())
		return;
	CString strXml;
	if(bPhrase){
		auto strPhrase = m_rs.GetFieldValueAsString(_T("PHRASE"));
		if(!m_vReplacement.empty()){
			wstring wstr = strPhrase;
			CString str1, str2;
			for(const auto& kv : m_vReplacement){
				str1.Format(_T("(\\W|^)(%s)(\\W|$)"), kv.first);
				str2.Format(_T("$1%s$3"), kv.second);
				wstr = regex_replace(wstr, wregex(str1), wstring(str2));
			}
			strPhrase = wstr.c_str();
		}
		strXml += theApp.GetVoiceXml(m_lbuSettings.nLangID, strPhrase);
	}
	if(bTrans){
		auto strTranslation = m_rs.GetFieldValueAsString(_T("TRANSLATION"));
		for(const auto& kv : m_vReplacementChn)
			strTranslation.Replace(kv.first, kv.second);
		strXml += theApp.GetVoiceXml(0, strTranslation);
	}
	theApp.Speak(strXml);
}

void CPhrasesBaseFrame::OnMultiAdd()
{
	CNewPhrasesDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;

	SDataGridNewRecordInfo* nri = GetDataGridNewRecordInfo();
	vector<CString> vstrPhrasesAndTranslations;
	SplitString(dlg.m_strPhrasesAndTranslations, _T("\r\n"), vstrPhrasesAndTranslations);
	for(int i = 0; i < vstrPhrasesAndTranslations.size(); i += 2){
		nri[0].lpszText = vstrPhrasesAndTranslations[i];
		nri[1].lpszText = vstrPhrasesAndTranslations[i + 1];
		DoAddNew(nri);
	}
}