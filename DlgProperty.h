//
// ����: yuvplay
//
// ����:
// ���źͷ���ԭʼYUV�ļ�, ��ʽת��, �ļ��Ƚϵ�
//
// ����:
// zhanghj(netjunegg@gmail.com)
//
// ������ʷ:
// 2007~2012: ҵ����Ŀ
//
// ��ȨЭ��: GNU GPL v2
//
//

#if !defined(AFX_DLGPROPERTY_H__FD48B0BA_82D1_40D0_9DA8_44C0B587ECD7__INCLUDED_)
#define AFX_DLGPROPERTY_H__FD48B0BA_82D1_40D0_9DA8_44C0B587ECD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGProperty.h : header file
//

#include "Plugin.h"


/////////////////////////////////////////////////////////////////////////////
// DLGProperty dialog

class DLGProperty : public CDialog
{
// Construction
public:
	DLGProperty(CWnd* pParent = NULL);   // standard constructor

public:
	const char *m_name;
	const Module_Info *m_pInfo;
	const char *m_path;


// Dialog Data
	//{{AFX_DATA(DLGProperty)
	enum { IDD = IDD_DLG_PROPERTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLGProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLGProperty)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTY_H__FD48B0BA_82D1_40D0_9DA8_44C0B587ECD7__INCLUDED_)
