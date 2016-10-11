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

#if !defined(AFX_DLGSIMPLECFG_H__9C7E6313_D6EE_4362_BAA4_1A2A9DF80A37__INCLUDED_)
#define AFX_DLGSIMPLECFG_H__9C7E6313_D6EE_4362_BAA4_1A2A9DF80A37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSimpleCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSimpleCfg dialog

class CDlgSimpleCfg : public CDialog
{
// Construction
public:
	CDlgSimpleCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSimpleCfg)
	enum { IDD = IDD_DLG_SIMPLE_CFG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	int m_val;
	unsigned int m_val2;
	CString m_str;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSimpleCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSimpleCfg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSIMPLECFG_H__9C7E6313_D6EE_4362_BAA4_1A2A9DF80A37__INCLUDED_)
