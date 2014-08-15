// LollyHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "Lolly.h"
#include "LollyHtmlView.h"
#include <atlsafe.h>
#include <exdispid.h>

// CLollyHtmlView

IMPLEMENT_DYNCREATE(CLollyHtmlView, CHtmlView)

CLollyHtmlView::CLollyHtmlView()
	: m_lBrowserOptions(0)
{
}

CLollyHtmlView::~CLollyHtmlView()
{
}

void CLollyHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLollyHtmlView, CHtmlView)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CLollyHtmlView, CHtmlView)
	ON_EVENT(CLollyHtmlView, AFX_IDW_PANE_FIRST,DISPID_NEWWINDOW3,NewWindow3,VTS_PDISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP() 


// CLollyHtmlView message handlers

void CLollyHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	CHtmlView::OnDocumentComplete(lpszURL);
	// The document is not complete until all of its data has been retrieved
	if(GetReadyState() != READYSTATE_COMPLETE) return;

	AFXGetParentFrame(this)->SendMessage(WM_HTMLVIEW_DOCCOMPLETE, (WPARAM)this, (LPARAM)lpszURL);

	if(GetLocationURL() == _T("about:blank") && !m_strHtml.IsEmpty()){
		SetHTML();
		Refresh();
	}
}

BOOL CLollyHtmlView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{
	if (dispid == DISPID_AMBIENT_DLCONTROL && m_lBrowserOptions != 0)
	{
		pvar->vt = VT_I4;
		pvar->lVal = m_lBrowserOptions; 
		return TRUE;
	}

	return CHtmlView::OnAmbientProperty(pSite, dispid, pvar);
}

void CLollyHtmlView::NewWindow3( IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	SNewWindow3Params nwp;
	nwp.ppDisp = ppDisp;
	nwp.Cancel = Cancel;
	nwp.dwFlags = dwFlags;
	nwp.bstrUrlContext = bstrUrlContext;
	nwp.bstrUrl = bstrUrl;
	AFXGetParentFrame(this)->SendMessage(WM_HTMLVIEW_NEWWINDOW3, 0, (LPARAM)&nwp);
}

void CLollyHtmlView::SetHTML( LPCTSTR pszHtml )
{
	Navigate2(_T("about:blank"));
	m_strHtml = pszHtml;
}

void CLollyHtmlView::SetHTML()
{
	// Get document object
	CComQIPtr<IHTMLDocument2> doc = GetHtmlDocument();

	// Create string as one-element BSTR safe array for IHTMLDocument2::write.
	CComSafeArray<VARIANT> sar;
	sar.Create(1,0);
	sar[0] = CComBSTR(m_strHtml);

	// open doc and write
	LPDISPATCH lpdRet;
	doc->open(CComBSTR("text/html"),
		CComVariant(CComBSTR("_self")),
		CComVariant(CComBSTR("")),
		CComVariant((bool)1),
		&lpdRet);

	doc->write(sar);	// write contents to doc
	doc->close();		// close
	lpdRet->Release();

	m_strHtml.Empty();
}

void CLollyHtmlView::OnSetFocus( CWnd* pOldWnd )
{
	CWnd* pWnd = m_wndBrowser.ChildWindowFromPoint(CPoint(10, 10));
	CWnd* pWnd2 = pWnd->ChildWindowFromPoint(CPoint(10, 10));
	pWnd2->SetFocus();
}


void CLollyHtmlView::OnDraw(CDC* /*pDC*/)
{
}


void CLollyHtmlView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	SetScrollSizes ( MM_TEXT,CSize (0 , 0 ) );
}


void CLollyHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);

	CComQIPtr<IOleControl> pOleControl = m_pBrowserApp;
	pOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DLCONTROL);
}


void CLollyHtmlView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);    //注意此处一定要是CFormView

	if (::IsWindow(m_wndBrowser.m_hWnd)) 
	{ 
		CRect rect; 
		GetClientRect(rect); 
		::AdjustWindowRectEx(rect, GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL, 
			rect.left, 
			rect.top, 
			rect.Width(), 
			rect.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER); 
	}

}
