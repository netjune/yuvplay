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

#if !defined(AFX_DLGVALUES2_H__73B49F2A_DCB6_4156_BE4E_39D9997A6A49__INCLUDED_)
#define AFX_DLGVALUES2_H__73B49F2A_DCB6_4156_BE4E_39D9997A6A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgValues2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgValues2 dialog

class CDlgValues2 : public CDialog
{
public:
	int m_adjustRect;
	
	CFont  m_font;
	COLORREF m_textColor;
	COLORREF m_bgColor;
	CBrush m_brush;
	
	//
	HWND  m_hParent;

	int m_x, m_y;
	int m_width, m_height;
	int m_real_w,m_real_h;
	
	int m_valueMode;
	int m_YUV;

public:
	int enableWnd(int id,int enable);
	int enableCheck(int id,int check);
	int  isChecked(int id);
	
	int  getStringSize(LPCTSTR str,int length,int &width,int &height);
	int  getStringLine(CString& str,int line, int &start,int &length);
	
	// interface
	int  clearItem();
	int  addItem(CString &text,int nLine,int numPixels);
	
	//
	void destroy();

// Construction
public:
	CDlgValues2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgValues2)
	enum { IDD = IDD_DLG_VALUES2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgValues2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgValues2)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioDec();
	afx_msg void OnRadioHex();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSet();
	afx_msg void OnButtonYuv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVALUES2_H__73B49F2A_DCB6_4156_BE4E_39D9997A6A49__INCLUDED_)
