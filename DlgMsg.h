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

#if !defined(AFX_DLGMSG_H__48FA07E9_79FD_4840_86B8_8AA69723AFAD__INCLUDED_)
#define AFX_DLGMSG_H__48FA07E9_79FD_4840_86B8_8AA69723AFAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog

class CDlgMsg : public CDialog
{
// Construction
public:
	CDlgMsg(CWnd* pParent = NULL);   // standard constructor
	~CDlgMsg();

// Dialog Data
	//{{AFX_DATA(CDlgMsg)
	enum { IDD = IDD_DLG_MSG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	const char *m_msg;
	
	CBrush m_brush;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMsg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSG_H__48FA07E9_79FD_4840_86B8_8AA69723AFAD__INCLUDED_)
