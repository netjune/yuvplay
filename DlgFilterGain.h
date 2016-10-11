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

#if !defined(AFX_DLGFILTERGAIN_H__4F4A5B48_E514_4EF9_863C_0F59794E1F00__INCLUDED_)
#define AFX_DLGFILTERGAIN_H__4F4A5B48_E514_4EF9_863C_0F59794E1F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgFilterGain.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFilterGain dialog

class CDlgFilterGain : public CDialog
{
// Construction
public:
	CDlgFilterGain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFilterGain)
	enum { IDD = IDD_DLG_FILTER_GAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFilterGain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	float m_gain1;
	float m_gain2;
	float m_gain3;
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFilterGain)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILTERGAIN_H__4F4A5B48_E514_4EF9_863C_0F59794E1F00__INCLUDED_)
