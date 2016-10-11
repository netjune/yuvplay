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

#if !defined(AFX_DLGPIXEL_H__945A546F_ADC9_47F6_9FAA_DFC4AE7F1D5A__INCLUDED_)
#define AFX_DLGPIXEL_H__945A546F_ADC9_47F6_9FAA_DFC4AE7F1D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPixel.h : header file
//


#define  WM_PIXEL_VAL           (WM_USER + 102)


struct PixelVal{
	int format;   // yuv, rgb
	int x;
	int y;
	unsigned char Y[3][3];
	unsigned char U[3][3];
	unsigned char V[3][3];
};



/////////////////////////////////////////////////////////////////////////////
// CDlgPixel dialog

class CDlgPixel : public CDialog
{
// Construction
public:
	CDlgPixel(CWnd* pParent = NULL);   // standard constructor
	~CDlgPixel();

// Dialog Data
	//{{AFX_DATA(CDlgPixel)
	enum { IDD = IDD_DIALOG_PIXEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPixel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CString m_str;
	int m_bHexMode;
	
	PixelVal m_pixVal;
	CBrush *m_pBrushes[9];
	CFont m_font;

	int update();
	void destroy();
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPixel)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMsgPixelVal(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCheckHex();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPIXEL_H__945A546F_ADC9_47F6_9FAA_DFC4AE7F1D5A__INCLUDED_)
