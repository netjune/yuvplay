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

#if !defined(AFX_DLGFORMATER_H__68E0DC8B_C418_4D2C_AD38_99F96DC771F2__INCLUDED_)
#define AFX_DLGFORMATER_H__68E0DC8B_C418_4D2C_AD38_99F96DC771F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgFormater.h : header file
//

#include "FormaterMan.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFormater dialog

class CDlgFormater : public CDialog
{
// Construction
public:
	CDlgFormater(CWnd* pParent = NULL);   // standard constructor

public:
	CFormaterItem *m_pFormater;

// Dialog Data
	//{{AFX_DATA(CDlgFormater)
	enum { IDD = IDD_DIALOG_FORMATER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormater)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormater)
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

#endif // !defined(AFX_DLGFORMATER_H__68E0DC8B_C418_4D2C_AD38_99F96DC771F2__INCLUDED_)
