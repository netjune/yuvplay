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

#if !defined(AFX_DLGFILTER_H__C78428EB_4313_4C83_88F3_0ADB3842704C__INCLUDED_)
#define AFX_DLGFILTER_H__C78428EB_4313_4C83_88F3_0ADB3842704C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFilter.h : header file
//
#include "FilterMan.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgFilter dialog

class CDlgFilter : public CDialog
{
// Construction
public:
	CDlgFilter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFilter)
	enum { IDD = IDD_DLG_FILTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CFilterItem *m_pFilter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFilter)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonCfg1();
	afx_msg void OnButtonCfg2();
	afx_msg void OnButtonQuery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILTER_H__C78428EB_4313_4C83_88F3_0ADB3842704C__INCLUDED_)
