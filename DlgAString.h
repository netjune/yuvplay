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

#if !defined(AFX_DLGASTRING_H__449FC94D_F12C_4317_B256_E5F0F177E6E2__INCLUDED_)
#define AFX_DLGASTRING_H__449FC94D_F12C_4317_B256_E5F0F177E6E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAString.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAString dialog

class CDlgAString : public CDialog
{
public:
	CString m_str;

// Construction
public:
	CDlgAString(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAString)
	enum { IDD = IDD_DLG_ASTRING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAString)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAString)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGASTRING_H__449FC94D_F12C_4317_B256_E5F0F177E6E2__INCLUDED_)
