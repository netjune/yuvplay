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

// yuvplayDlg.h : header file
//

#if !defined(AFX_YUVPLAYDLG_H__D64E1F63_DA80_4810_8907_255984D1CFBD__INCLUDED_)
#define AFX_YUVPLAYDLG_H__D64E1F63_DA80_4810_8907_255984D1CFBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CYuvplayDlg dialog
#include  "CmpDlg.h"
#include  "YUVDoc.h"
#include  "ImageWnd.h"
#include  "MenuList.h"
#include  "Render.h"
#include "FormaterMan.h"
#include "FilterMan.h"



#define		FLAG_NONE	 0x0
#define		FLAG_HIDE	 0x1

#define		STATE_NOVIDEO		0x0
#define		STATE_VIDEOSTART	0x1
#define		STATE_VIDEOMID		0x2
#define		STATE_VIDEOEND		0x3
#define		STATE_PLAYING		0x4
#define		STATE_PAUSE			0x5
#define		STATE_ONEFRAME		0x6

#define		IDM_REGIST				0x0100
#define     IDM_CENTER_WINDOW       0x0110

class CYuvplayDlg : public CDialog
{
// Construction
public:
	CYuvplayDlg(CWnd* pParent = NULL);	// standard constructor
	~CYuvplayDlg();

// Dialog Data
	//{{AFX_DATA(CYuvplayDlg)
	enum { IDD = IDD_YUVPLAY_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYuvplayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

public:
	CString m_fileName;
	int  m_bLoop;

	int  m_width;
	int  m_height;
	int  m_field_id;
	int  m_format_id;

	int  m_showYUV;
	int  m_fps;


	int  m_bGridOn;
	int  m_gridXNum;
	int  m_gridYNum;
	COLORREF m_gridColor;

	int  m_bCompare;
	int  m_playing;
	
	int  m_bReady;		// 至少有一帧数据可以播

	int  m_notifyId;	 // imageWnd

	LPTSTR m_lpCmdline;

	// class
	CYUVDoc m_yuvDoc;
	CRender m_render;
	CFormaterItem **m_ppFormaters;
	CFilterItem **m_ppFilters;
	IImage_Filter **m_ppFilters2;

	CmpDlg  *m_pCmpDlg;
	ImageWnd  m_imageWnd;

	// msg
	CString m_buf;

	//
	CMenu *m_pMenu;
	CMenu *m_pRecentMenu;

	int  m_recent_size;
	int  m_favor_size;
	
	MenuList  *m_pRecentList;
	MenuList  *m_pFavorList;
	
	CString   m_exePath;
	
	//
	int m_conf_file_flag;
	
	// 
	CSize m_fullSize;
	CSize m_upSize;

	// cmp
	RawImage_Info m_formatLocal;
	
public:
	void printMsg(LPCTSTR msg);

	int getEditInt(int id,int &val);
	int setEditInt(int id,int val);
	int getComBoxStr(int id,CString &str);
	int addComBoxStr(int id,LPCTSTR str);
	int delComBoxStr(int id, LPCTSTR str);
	int setComBoxSel(int id,int nIndex);
	int setComBoxSel(int id,LPCTSTR str);
	int getComBoxSel(int id);

	// added on 2010/07/27
	int addComBoxItem(int id,LPCTSTR str, unsigned int data);
	int setComBoxData(int id, int index, unsigned int data);
	int getComBoxData(int id, int index, unsigned int *pData);

	// added 
	CButton* getButton(int id);
	CComboBox* getComBox(int id);
	void enableWnd(int id,int enable);
	void enableCheck(int id,int check);
	int  isChecked(int id);

	// added
	void setEditReadOnly(int id, int bReadOnly);
	void enableItem(int id,int enable);

	int getImageSize(int &width,int &height);
	int setImageSize(int widht,int height);
	
	// YUV
	void notifyView(int id = 0);
	int updateConf1(int level = 1);	
	int updateConf2(int level = 1);	
	int updateFinal();
	int displayFrame();

	int imageStart();
	int imageEnd(int back);
	int noImage();

	int startPlay();
	int endPlay();
	
	int  setState(int state);		// 仅负责控件使能与否
	int  closeFile();

	// compare
	int  updateCompare1(int level = 1);
	int  updateCompare2(int level = 1);
	void  notifyCompare(int id);

	// added
	int loadConfigFile();
	int saveConfigFile(int bForce = 0);
	void  initMenu();
	int openFile(LPCTSTR fileName, int bUseFile = 1, int bUpdateRencent = 1);


	//
	 void OnBkAdd();
	 void OnBkMan();
	 void OnRecent(int id);
	 void OnFavorite(int id);


	// Generated message map functions
	//{{AFX_MSG(CYuvplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMyMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnImageWnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnFront(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMore();
	afx_msg void OnOpenFile();
	afx_msg void OnCompare();
	afx_msg void OnButtonAddsize();
	afx_msg void OnButtonDelsize();
	afx_msg void OnChangeImageSize();
	afx_msg void OnSelchangeComboSize();
	afx_msg void OnCheckGrid();
	afx_msg void OnButtonApply();
	afx_msg void OnPrevFrame();
	afx_msg void OnNextFrame();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPause();
	afx_msg void OnChangeFps();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnCheckLoop();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonGridColor();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnChangeFormat();
	afx_msg void OnButtonTranscode();
	afx_msg void OnClose();
	afx_msg void OnButtonQuit();
	afx_msg void OnCheckExt();
	afx_msg void OnCheckFilter();
	afx_msg void OnButtonFilterCfg();
	afx_msg void OnButtonFormatInfo();
	afx_msg void OnCheckConf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonBookmark();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YUVPLAYDLG_H__D64E1F63_DA80_4810_8907_255984D1CFBD__INCLUDED_)
