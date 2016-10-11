//
// 工程: yuvplay
//
// 描述:
// 播放和分析原始YUV文件, 格式转换, 文件比较等
//
// 作者:
// zhanghj(netjunegg@gmail.com)
//
// 开发历史:
// 2007~2012: 业余项目
//
// 版权协议: GNU GPL v2
//
//

#if !defined(AFX_IMAGEWND_H__DA54F22F_B21C_4BE4_9D7A_DA2E129E0DBE__INCLUDED_)
#define AFX_IMAGEWND_H__DA54F22F_B21C_4BE4_9D7A_DA2E129E0DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageWnd.h : header file
//

#include "ValuesDlg.h"
#include "YuvDoc.h"
#include "PosWnd.h"
#include "DlgPixel.h"
#include "Render.h"
#include "DlgValues2.h"


class CMyRect{
public:
	int left;
	int top;
	int width;
	int height;

	CMyRect(){
		left = top = width = height = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// ImageWnd window

class ImageWnd : public CWnd
{
// Construction
public:
	ImageWnd();

// Attributes
public:
	int m_rszFlag;

	CRender *m_pRender;
	
	int m_width;
	int m_height;
	BITMAPINFOHEADER *m_pBmpInfo;
	unsigned char *m_pBmpBits;

	int  m_bGridOn;
	int  m_gridXNum;
	int  m_gridYNum;
	COLORREF m_gridColor;
	unsigned int m_myLife1, m_myLife2;

	ValuesDlg  *m_pValuesDlg;
	HWND   m_hParent;

	PosWnd  *m_pPosWnd;
	CPoint  m_pt;

	// added
	CPoint  m_oldPt;

	// added on 2008/10/30
	int  m_topWnd;
	
	CString  m_title;
	

	// debug
	int  m_dbg;

	HBITMAP m_hBitmap;
	HDC   m_hMemDC;

	CMyRect m_rect1, m_rect2, m_rect3;
	int m_paintFlag;

	CDlgValues2 *m_pValues2Dlg;

	CPoint m_lockPoint;
	int m_bGridFlag;


// Operations
public:
	BOOL  Create(CWnd* pParentWnd, UINT nID,LPCTSTR lpszWindowName);

	void drawGrid(CDC *pDc,CRect &rect);

	int enableGrid(int enable,int xNum = -1,int yNum = -1);
	
	int getGridPos(CPoint &point,int bGrid);

	int setClientSize(int cx,int cy, int center = 0);
	void printMsg(LPCTSTR msg);
	
	void  preDisplayImage(int rectOnly = 0);
	void  displayImage(CDC *pDc= NULL);
	void  adjustRect(int level = 0);

	void clear();
	void destroy();
	
	int  setGridColor(COLORREF color);

	// added
	int  updateValues(CPoint point, int flag = 0);
	int  updateValues2(CPoint pt, int flag = 0);

	void setTitle(LPCTSTR title);
	
	int enableValues2(int enable);

	int updateLayerout(int cx, int cy);
	int transferRect(const CMyRect &rect1, const CMyRect &rect2, const CMyRect &rect3, CMyRect &rect4);
	int fillTheRest(CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImageWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ImageWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(ImageWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMyMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMsgValueWnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnMsgValue2Wnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEWND_H__DA54F22F_B21C_4BE4_9D7A_DA2E129E0DBE__INCLUDED_)
