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

#if !defined(AFX_DLGVALUES2SET_H__32688011_4D00_4D65_9559_3F6DDD1DD62C__INCLUDED_)
#define AFX_DLGVALUES2SET_H__32688011_4D00_4D65_9559_3F6DDD1DD62C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgValues2Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgValues2Set dialog

class CDlgValues2Set : public CDialog
{
public:
	int m_YUV;

	int enableWnd(int id,int enable);
	int enableCheck(int id,int check);
	int  isChecked(int id);


// Construction
public:
	CDlgValues2Set(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgValues2Set)
	enum { IDD = IDD_DLG_VALUES2_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgValues2Set)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgValues2Set)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVALUES2SET_H__32688011_4D00_4D65_9559_3F6DDD1DD62C__INCLUDED_)
