#pragma once

// CLollyHtmlView html view

#define WM_HTMLVIEW_DOCCOMPLETE		WM_USER + 101
#define WM_HTMLVIEW_NEWWINDOW3		WM_USER + 102

struct SNewWindow3Params
{
	IDispatch **ppDisp;
	VARIANT_BOOL *Cancel;
	DWORD dwFlags;
	BSTR bstrUrlContext;
	BSTR bstrUrl;
};

class CLollyHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CLollyHtmlView)

protected:
	CLollyHtmlView();           // protected constructor used by dynamic creation
	virtual ~CLollyHtmlView();

public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	void SetHTML(LPCTSTR pszHtml);
	void SetBrowserOptions(LONG lVal) { m_lBrowserOptions = lVal; }
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);

protected:
	CString m_strHtml;
	LONG m_lBrowserOptions;
	void SetHTML();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()

	void NewWindow3(IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl); 
	DECLARE_EVENTSINK_MAP() 
public:
	virtual void OnDraw(CDC* /*pDC*/);
	virtual void OnInitialUpdate();
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
