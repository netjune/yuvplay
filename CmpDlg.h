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

#if !defined(AFX_CMPDLG_H__4000703C_97AA_4769_9D11_0180549277F1__INCLUDED_)
#define AFX_CMPDLG_H__4000703C_97AA_4769_9D11_0180549277F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmpDlg.h : header file
//

#include "ImageWnd.h"
#include "FormaterMan.h"
#include "FilterMan.h"


#define		STATE_NOVIDEO		0x0
#define		STATE_VIDEOSTART	0x1
#define		STATE_VIDEOMID		0x2
#define		STATE_VIDEOEND		0x3
#define		STATE_PLAYING		0x4
#define		STATE_PAUSE			0x5

#define		CMP_NEXT_FRAME		0x1
#define		CMP_PREV_FRAME		0x2
#define		CMP_STOP				0x3
#define		CMP_SEEK				0x4
#define     CMP_UPDATE          0x5


#define     CMP_MODE_BI				0x1
#define		CMP_MODE_DIFF			0x2

/////////////////////////////////////////////////////////////////////////////
// CmpDlg dialog

class CmpDlg : public CDialog
{
// Construction
public:
	CmpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CmpDlg)
	enum { IDD = IDD_DLG_COMPARE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmpDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

public:
	CString m_fileName;
	int  m_width,m_height;
	int  m_showYUV;
	int  m_format_id;
	int  m_field_id;

	int  m_frameRate;
	int  m_bLoop;
	
	int  m_bGridOn;
	int  m_gridXNum;
	int  m_gridYNum;
	COLORREF m_gridColor;

	HWND  m_hParent;
	int  m_lockSync;

	CYUVDoc  m_yuvDoc;
	CRender  m_render;
	CRender  m_render2;
	ImageWnd  m_imageWnd;
	ImageWnd  m_imageWnd2;
	int  m_notifyId;	 // imageWnd use
	int  m_notifyId2;

	CFormaterItem **m_ppFormaters;
	CFilterItem **m_ppFilters;
	IImage_Filter **m_ppFilters2;

	int  m_bPlaying;
	int  m_bReady;		// 至少有一帧数据可以播

	int  m_bEnd;

	int  m_threshold;

	int  m_cmpMode;
	int m_cmpComp;

	CString m_path;
	CString m_buf;

	// compare
	int m_bFormatSynced;
	RawImage_Info m_formatPeer;
	RawImage_Info m_formatLocal;
	CRender *m_pRender;
	
	
public:
	int getEditInt(int id,int &val);
	int setEditInt(int id,int val);
	int getComBoxStr(int id,CString &str);
	int addComBoxStr(int id,LPCTSTR str);
	int delComBoxStr(int id, LPCTSTR str);
	int setComBoxSel(int id,int nIndex);
	int setComBoxSel(int id,LPCTSTR str);
	int getComBoxSel(int id);
	
	// added 
	CButton* getButton(int id);
	CComboBox* getComBox(int id);
	void enableWnd(int id,int enable);
	void enableCheck(int id,int check);
	int  isChecked(int id);

	// added on 2010/07/27
	int addComBoxItem(int id,LPCTSTR str, unsigned int data);
	int setComBoxData(int id, int index, unsigned int data);
	int getComBoxData(int id, int index, unsigned int *pData);


	// YUV
	void notifyView(int id = 0);
	int updateConf1(int level = 1);		// for yuvDoc
	int updateConf2(int level = 1);		// for View
	int setState(int state);
	int displayFrame();
	int updateDiff();
	void clear(int level = 0);
	int show();
	void printMsg(LPCTSTR msg);

	int imageEnd(int pos);

	//
	int setLoop(int bLoop);
	
	//
	int setStartPath(LPCTSTR path);

	//
	void  destroy();

	// compare 
	int setCmpFormat(RawImage_Info *pInfo);


	// Generated message map functions
	//{{AFX_MSG(CmpDlg)
	afx_msg void OnOpenFile();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	afx_msg LRESULT OnMyMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnImageWnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCheckLockSync();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonStop();
	afx_msg void OnButtonSave();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonOk();
	afx_msg void OnRadioBi();
	afx_msg void OnRadioDiff();
	afx_msg void OnButtonSave2();
	afx_msg void OnButtonFormatInfo();
	afx_msg void OnCheckFilter();
	afx_msg void OnButtonFilterCfg();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboCmpComp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMPDLG_H__4000703C_97AA_4769_9D11_0180549277F1__INCLUDED_)
