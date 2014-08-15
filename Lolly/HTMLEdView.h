#pragma once


// CHTMLEdView view

class CHTMLEdView : public CHtmlEditView
{
	DECLARE_DYNCREATE(CHTMLEdView)
	DECLARE_DHTMLEDITING_CMDMAP(CHTMLEdView);

protected:
	CHTMLEdView();           // protected constructor used by dynamic creation
	virtual ~CHTMLEdView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void SetAbsPos(UINT);
	void SetStaticPos(UINT);
	DECLARE_MESSAGE_MAP()
};


