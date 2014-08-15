#pragma once

#include "PicBookView.h"

// CPicBookFrame

class CPicBookFrame : public CLollyFrame
{
	DECLARE_DYNCREATE(CPicBookFrame)

public:
	CPicBookFrame();
	virtual ~CPicBookFrame();

	void Init(const CString& strBookName, const CString& strFileName, int nNumPages, int nCurPage);

protected:
	CString m_strBookName, m_strFileName;
	int m_nPageNo, m_nPageCount;
	CMFCToolBarEditBoxButton2* m_pedtPageNo;
	CPicBookView* m_pView;

	UINT m_nIDLineWrite;

	virtual CString GetFrameText() const;

	void SetPageNo(int nPageNo);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(UINT nID);
	afx_msg void OnUpdateMovePrevious(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveNext(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLine(UINT nID);
	afx_msg void OnWrite();
	afx_msg void OnUpdateLineWrite(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnToolbarReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPageNo();
};
