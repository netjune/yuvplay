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

#if !defined(AFX_POSWND_H__0B3E6C09_3132_4D3E_9F70_2B01484B30B3__INCLUDED_)
#define AFX_POSWND_H__0B3E6C09_3132_4D3E_9F70_2B01484B30B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PosWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PosWnd window

class PosWnd : public CWnd
{
// Construction
public:
	PosWnd();

// Attributes
public:
	int  m_gridX;
	int  m_gridY;
	int  m_show;
	HWND  m_hParent;
	
	CString m_buf;

	// 
	CSize m_sz;

// Operations
public:
	BOOL  Create(CWnd* pParentWnd, UINT nID,LPCTSTR lpszWindowName);

	int  setPos(CPoint& pt_grid, CPoint& pt_pixel);
	int  show(int show = 1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PosWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PosWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(PosWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSWND_H__0B3E6C09_3132_4D3E_9F70_2B01484B30B3__INCLUDED_)
