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

#if !defined(AFX_DLGTRANSCODE_H__1C04F7ED_849F_4E79_9377_CF876FF17081__INCLUDED_)
#define AFX_DLGTRANSCODE_H__1C04F7ED_849F_4E79_9377_CF876FF17081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTranscode.h : header file
//

#include "ThreadTranscode.h"
#include "yuvDoc.h"
#include "FormaterMan.h"
#include "Render.h"

/////////////////////////////////////////////////////////////////////////////
// DlgTranscode dialog

class DlgTranscode : public CDialog
{
// Construction
public:
	DlgTranscode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgTranscode)
	enum { IDD = IDD_DLG_TRANSCODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgTranscode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	~DlgTranscode();

	CYUVDoc *m_pYuvDoc;
	CRender *m_pRender;
	CFormaterItem **m_ppFormaters;

	TranscodeArg *m_pArg;
	CWinThread *m_thread;
	CString m_buf;
	CString m_fileName;

	//
	CFont m_font;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgTranscode)
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonTrans();
	afx_msg void OnButtonQuit();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMyThread(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRANSCODE_H__1C04F7ED_849F_4E79_9377_CF876FF17081__INCLUDED_)
