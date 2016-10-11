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

#if !defined(AFX_DLGFILTER_H__A5A01E74_3B28_402D_A600_86DB8EE677BF__INCLUDED_)
#define AFX_DLGFILTER_H__A5A01E74_3B28_402D_A600_86DB8EE677BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgFilters.h : header file
//

#include "FilterMan.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgFilters dialog

class CDlgFilters : public CDialog
{
// Construction
public:
	CDlgFilters(CWnd* pParent = NULL);   // standard constructor

public:
	CFilterItem **m_ppFilters;
	IImage_Filter **m_ppFilters2;

	int initColumn();
	int addItem(const char *name, int id, int checked);
	
	CFilterItem ** getFilters();

	int seeItem(int index);

// Dialog Data
	//{{AFX_DATA(CDlgFilters)
	enum { IDD = IDD_DLG_FILTERS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFilters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFilters)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonProperty();
	afx_msg void OnButtonCfg();
	afx_msg void OnDblclkListFilters(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkListFilters(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILTER_H__A5A01E74_3B28_402D_A600_86DB8EE677BF__INCLUDED_)
