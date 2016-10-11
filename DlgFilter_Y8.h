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

#if !defined(AFX_DLGFILTER_Y8_H__D00604C9_3723_429B_87E3_2B8B09AA5EAA__INCLUDED_)
#define AFX_DLGFILTER_Y8_H__D00604C9_3723_429B_87E3_2B8B09AA5EAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFilter_Y8.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFilter_Y8 dialog

class CDlgFilter_Y8 : public CDialog
{
// Construction
public:
	CDlgFilter_Y8(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFilter_Y8)
	enum { IDD = IDD_DLG_FILT_Y8 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	int m_bEmu;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFilter_Y8)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFilter_Y8)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILTER_Y8_H__D00604C9_3723_429B_87E3_2B8B09AA5EAA__INCLUDED_)
