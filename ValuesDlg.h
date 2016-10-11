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

#if !defined(AFX_VALUESDLG_H__7908D515_8A61_48E7_80F3_CDE1F22DA406__INCLUDED_)
#define AFX_VALUESDLG_H__7908D515_8A61_48E7_80F3_CDE1F22DA406__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ValuesDlg.h : header file
//

#define  ITEM_NUM_MAX	3

#define  VALUE_MODE_NONE    0x0
#define  VALUE_MODE_HEX      0x1
#define  VALUE_MODE_DEC      0x2

/////////////////////////////////////////////////////////////////////////////
// ValuesDlg dialog

class ValuesDlg : public CDialog
{
public:
	int m_adjustRect;
	
	CFont  m_font;
	COLORREF m_textColor;
	COLORREF m_bgColor;
	CBrush m_brush;

	//
	HWND  m_hParent;
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
	ValuesDlg(CWnd* pParent = NULL);   // standard constructor
	~ValuesDlg();

// Dialog Data
	//{{AFX_DATA(ValuesDlg)
	enum { IDD = IDD_DLG_VALUES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ValuesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ValuesDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioHex();
	afx_msg void OnRadioDec();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUESDLG_H__7908D515_8A61_48E7_80F3_CDE1F22DA406__INCLUDED_)
