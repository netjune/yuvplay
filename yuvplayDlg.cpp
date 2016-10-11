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

// yuvplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "yuvplayDlg.h"
#include "global.h"
#include "common.h"
#include <atlbase.h>
#include "DlgTranscode.h"
#include "Version.h"
#include "FormaterMan.h"
#include "FilterMan.h"
#include "DlgFormater.h"
#include "DlgFilters.h"
#include "DlgAString.h"

//aa
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include  "Message.h"
#include "yuvplaydlg.h"


#define ID_TIMER1		1



#define HELP_TEXT   "提示: 如果想把滤镜处理过的图像保存到文件, 可以使用\"格式转换器\""


/////////
static LPCTSTR helpText = "暂无";

/*history = "==========版本历史============\r\n\r\n"
"2008/02/01: 加入YUV比较功能\r\n"
		"2008/02/02: 增加快速版本\r\n";
*/
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LPCTSTR  help = helpText;
	CString str;

	str.Format("%s %s(%s)",APP_NAME, APP_VERSION, __DATE__);
	
	SetDlgItemText(IDC_STATIC1, str);
	SetDlgItemText(IDC_STATIC2, ABOUT_CONTACT);
	SetDlgItemText(IDC_STATIC3, ABOUT_OTHER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CYuvplayDlg dialog

CYuvplayDlg::CYuvplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYuvplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYuvplayDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fileName = "";
	m_width = 352;
	m_height = 288;
	m_bLoop = 0;
	/*
	int  m_showYUV;
	int  m_format;
	int  m_frameRate;
	int  m_bGridOn;
	int  m_gridXNum;
	int  m_gridYNum;
	int  m_bCompare;
*/

	m_bReady = 0;
	m_playing = 0;
	m_bCompare = 0;
	m_notifyId = 1;
	
	m_lpCmdline = NULL;

	m_pCmpDlg = NULL;

	
	m_gridColor = RGB(255,255,255);

	//
	m_pMenu = NULL;
	m_pRecentMenu = NULL;

	m_recent_size = 32;
	m_favor_size = 20;
	m_pRecentList = NULL;
	m_pFavorList = NULL;

	m_conf_file_flag = 1;

	m_ppFilters = NULL;
	m_ppFormaters = NULL;
	m_ppFilters2 = NULL;
}

CYuvplayDlg::~CYuvplayDlg(){

}

void CYuvplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYuvplayDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CYuvplayDlg, CDialog)
	//{{AFX_MSG_MAP(CYuvplayDlg)
	ON_MESSAGE(WM_COMPARE,OnMyMsg)
	ON_MESSAGE(WM_IMAGEWND, OnImageWnd)
	ON_MESSAGE(WM_FRONT, OnFront)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MORE, OnMore)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnOpenFile)
	ON_BN_CLICKED(IDC_CHECK_COMPARE, OnCompare)
	ON_BN_CLICKED(IDC_BUTTON_ADDSIZE, OnButtonAddsize)
	ON_BN_CLICKED(IDC_BUTTON_DELSIZE, OnButtonDelsize)
	ON_CBN_EDITCHANGE(IDC_COMBO_SIZE, OnChangeImageSize)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnSelchangeComboSize)
	ON_BN_CLICKED(IDC_CHECK_GRID, OnCheckGrid)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnPrevFrame)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNextFrame)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnPause)
	ON_CBN_EDITCHANGE(IDC_COMBO_FPS, OnChangeFps)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAY, OnReleasedcaptureSlider1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonGridColor)
	ON_WM_MOVE()
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnChangeFormat)
	ON_BN_CLICKED(IDC_BUTTON_TRANSCODE, OnButtonTranscode)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_BN_CLICKED(IDC_CHECK_EXT, OnCheckExt)
	ON_BN_CLICKED(IDC_CHECK_FILTER, OnCheckFilter)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_CFG, OnButtonFilterCfg)
	ON_BN_CLICKED(IDC_BUTTON_FORMAT_INFO, OnButtonFormatInfo)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_CHECK_CONF, OnCheckConf)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BOOKMARK, OnBnClickedButtonBookmark)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYuvplayDlg message handlers

LRESULT CYuvplayDlg::OnMyMsg(WPARAM wParam,LPARAM lParam){
	int id = wParam;

	if(id == 1){
		m_bCompare = 0;
		enableCheck(IDC_CHECK_COMPARE,0);
	}
	else if(id == 2012){
		openFile(m_fileName);
	}
	
	return 1;
}

LRESULT CYuvplayDlg::OnImageWnd(WPARAM wParam,LPARAM lParam){
	if(wParam == 0){
		HWND  hWnd = (HWND)lParam;

		if(hWnd == m_imageWnd.m_hWnd){
			if(m_bReady)
				OnStop();
			if(m_bReady)
				OnStop();
		}
	}
	else if(wParam == 1){
		int enable = isChecked(IDC_CHECK_GRID);
		enableCheck(IDC_CHECK_GRID, !enable);
		OnCheckGrid();
	}

	return 1;
}

LRESULT CYuvplayDlg::OnFront(WPARAM wParam,LPARAM lParam){
	WINDOWPLACEMENT place;
	GetWindowPlacement(&place);
	if(place.showCmd == SW_SHOWMINIMIZED)
		ShowWindow(SW_SHOWNORMAL);
	
	SetForegroundWindow();
	return 0;
}

void CYuvplayDlg::printMsg(LPCTSTR msg){
	if(msg != NULL)
		AfxMessageBox(msg);
}

int CYuvplayDlg::getEditInt(int id,int &val){
	CString buf;
	int x;
	CEdit *pEdit;

	pEdit = (CEdit*)GetDlgItem(id);
	if(pEdit == NULL)
		return -1;
	pEdit->GetWindowText(buf);

	if(myGetIntFromStr(buf,&val) < 0)
		return -1;
	return 1;
}

int CYuvplayDlg::setEditInt(int id,int val){
	CString buf;
	CEdit *pEdit;

	pEdit = (CEdit*)GetDlgItem(id);
	if(pEdit == NULL)
		return -1;
	buf.Format("%d",val);
	pEdit->SetWindowText(buf);

	return 1;
}

int CYuvplayDlg::getComBoxStr(int id,CString &str){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;

	int n = pBox->GetEditSel();
	int nIndex = pBox->GetCurSel();
	if(nIndex != CB_ERR){
		pBox->GetLBText(nIndex,str);
		return 1;
	}
	pBox->GetWindowText(str);
	if(str.IsEmpty())
		return -1;
	return 1;
}

int CYuvplayDlg::addComBoxStr(int id,LPCTSTR str){
	CComboBox *pBox;
	int nIndex;

	if(str == NULL)
		return -1;
	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;
/*
	nIndex = pBox->GetCurSel();
	if(nIndex == CB_ERR)
		return -2;
*/	
	nIndex = pBox->FindString(0,str);
	if(nIndex == CB_ERR ){
		nIndex = pBox->AddString(str);
		if(nIndex == CB_ERR)
			return -3;	// failed
	}

	pBox->SetCurSel(nIndex);
	return nIndex;
}

int CYuvplayDlg::addComBoxItem(int id,LPCTSTR str, unsigned int data){
	CComboBox *pBox;
	int index;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL ||str == NULL)
		return -1;
	index = pBox->AddString(str);
	if(index == CB_ERR)
		return -3;	// failed
	pBox->SetItemData(index, data);
	return index;
}
int CYuvplayDlg::delComBoxStr(int id, LPCTSTR str){
	CComboBox *pBox;
	int nIndex;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;

	if(str != NULL ){
		if(strlen(str) <= 0)
			return -2;
		nIndex = pBox->FindString(0,str);
	}
	else{
		nIndex = pBox->GetCurSel();
	}

	if(nIndex != CB_ERR){
		pBox->DeleteString(nIndex);
		nIndex--;
		if(nIndex < 0 && pBox->GetCount()){
			nIndex++;
		}
	}

	pBox->SetCurSel(nIndex);

	return 1;
}

int CYuvplayDlg::setComBoxSel(int id,int nIndex){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;

	pBox->SetCurSel(nIndex);
	return 1;
}

int CYuvplayDlg::setComBoxSel(int id,LPCTSTR str){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;

	int index;
	CString buf;

	index = 0;
	while(1){
		int index2;
		index2 = pBox->FindString(index,str);
		if(index2 == CB_ERR)
			break;
		else if(index2 < index)
			break;
		
		pBox->GetLBText(index2,buf);
		if(!buf.CompareNoCase(str)){
			pBox->SetCurSel(index2);
			return 1;
		}

		index++;
	}

	return 0;
}


int CYuvplayDlg::getComBoxSel(int id){
	CComboBox *pBox = (CComboBox*)GetDlgItem(id);
	if(pBox)
		return pBox->GetCurSel();
	return -1;
}

int CYuvplayDlg::setComBoxData(int id, int index, unsigned int data){
	CComboBox *pBox;
	
	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;
	int num = pBox->GetCount();
	if(index < 0 || index > num -1)
		return -2;
	pBox->SetItemData(index, data);
	return 1;
}
int CYuvplayDlg::getComBoxData(int id, int index, unsigned int *pData){
	CComboBox *pBox;
	
	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;
	int num = pBox->GetCount();
	if(index < 0 || index > num -1)
		return -2;
	if(pData)
		*pData = pBox->GetItemData(index);
	return 1;
}

CButton  *CYuvplayDlg::getButton(int id){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	return pB;
}

CComboBox *CYuvplayDlg::getComBox(int id){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	return pBox;
}

void  CYuvplayDlg::enableWnd(int id,int enable){
	CWnd  *pWnd = GetDlgItem(id);
	if(pWnd)
		pWnd->EnableWindow(enable);
}

void  CYuvplayDlg::enableCheck(int id,int check){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	if(pB != NULL)
		pB->SetCheck(check ? BST_CHECKED : BST_UNCHECKED);
}

int  CYuvplayDlg::isChecked(int id){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	if(pB == NULL)
		return 0;
	return (pB->GetCheck() == BST_CHECKED);
}

/////////
int CYuvplayDlg::getImageSize(int &width,int &height){
	if(getEditInt(IDC_EDIT_WIDTH,width) < 0)
		return -1;
	if(getEditInt(IDC_EDIT_HEIGHT,height) < 0)
		return -1;
	return 1;
}

int CYuvplayDlg::setImageSize(int width,int height){
	if(width < 0 || height < 0)
		return -1;

	if(setEditInt(IDC_EDIT_WIDTH,width) < 0)
		return -2;
	if(setEditInt(IDC_EDIT_HEIGHT,height) < 0)
		return -3;

	return 1;
}

//
//  图像参数通信
//  level: 
//     0: 常规更新, 提示错误
//     1: 试探更新, 不提示错误
//
int CYuvplayDlg::updateConf1(int level){
	int val1,val2;
	CString err_msg;
	CString buf;
	CButton *pBtn;
	int n;
	CString  buf2;
	long pos;
	int index;

	if(m_playing > 0)
		return 0;

	m_bReady = 0;

	if(getEditInt(IDC_EDIT_WIDTH,val1) < 0){
		err_msg = "宽度值有错";
		goto leave;
	}
	if(getEditInt(IDC_EDIT_HEIGHT,val2) < 0){
		err_msg = "高度值有错";
		goto leave;
	}
	if(val1 <= 0 || val2 <= 0){
		err_msg = "尺寸设置有误";
		goto leave;
	}
	if(val1 & 0x1){
		err_msg = "暂不支持奇数宽度";
		goto leave;
	}
	if(val2 & 0x1){
		err_msg = "暂不支持奇数高度";
		goto leave;
	}

	if(	m_width != val1 || m_height != val2){
		m_width = val1;
		m_height = val2;
		m_notifyId = 1;
	}

	{
		// yuv格式
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		int index = pBox->GetCurSel();
		if(index < 0){
			err_msg = "无效yuv格式";
			goto leave;
		}
		val1 = pBox->GetItemData(index);
		val2 = m_yuvDoc.getFormaterID();
		if(val2 != val1){
			IYUV_Formater *formater = m_ppFormaters[index]->pItem;
			m_yuvDoc.setFormater(formater, val1);
		}
		m_format_id = val1;

		// 场参数
		pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FIELD);
		index = pBox->GetCurSel();
		if(index < 0){
			err_msg = "无效的场参数";
			goto leave;
		}
		if(index == 0)
			m_field_id = FIELD_ALL;
		else if(index == 1)
			m_field_id = FIELD_TOP_ONLY;
		else if(index == 2)
			m_field_id = FIELD_BOTTOM_ONLY;
		else if(index == 3)
			m_field_id = FIELD_TOP_BOTTOM_SEQ;
		else{
			err_msg = "不支持的场参数";
			goto leave;
		}

		YUV_Params params;
		params.width = m_width;
		params.height = m_height;
		params.field = m_field_id;
		if(m_yuvDoc.setParams(&params) < 0){
			err_msg.Format("当前格式插件不支持此yuv参数");
			printMsg(err_msg);
			goto leave;
		}
		m_field_id = index;

		RawImage_Info info;
		m_yuvDoc.getOutFormat(&info);
		
		// filters
		if(isChecked(IDC_CHECK_FILTER))
			m_render.setFilters(m_ppFilters2);
		else
			m_render.setFilters(NULL);

		if(m_render.setParams(&info) < 0){
			err_msg.Format("不支持的输出格式, id: %d(0x%X), %dx%d",
				info.raw_format, info.raw_format, info.width,info.height);
			printMsg(err_msg);
			goto leave;
		}
		
		if(m_render.update() < 0){
			printMsg(m_render.getErrMsg());
			goto leave;
		}
	
		m_render.getRawInfo(&info);
		m_formatLocal = info;

		if(m_pCmpDlg){
			if(m_bCompare && m_pCmpDlg->setCmpFormat(&info) <= 0){
				printMsg("两边格式不同, 将无法比较, 请调整格式");
			}
		}
	}

	return 1;

leave:
	if( level == 0){
		if(err_msg.IsEmpty())
			err_msg = m_yuvDoc.getErrMsg();
		if(!err_msg.IsEmpty())
			printMsg(err_msg);
	}

	return -1;
}

//
// update view
// level: 
//     0: 常规更新
//     1: 强制更新,即使参数不对也刷新,主要是刷新窗口
int CYuvplayDlg::updateConf2(int level){
	int change;
	CString err_msg;
	CString buf, buf2;
	int pos;

	if(getComBoxStr(IDC_COMBO_SHOWYUV,buf2) < 0){
		err_msg = "请选择显示分量";
		goto leave;
	}
	pos = buf2.Find('(',0);
	if(pos > 0){
		buf = buf2.Left(pos);
	}
	else{
		buf = buf2;
	}
	if(buf == "YUV")
		m_showYUV = YUV_SHOW_YUV;
	else if(buf == "Y")
		m_showYUV = YUV_SHOW_Y;
	else if(buf == "U")
		m_showYUV = YUV_SHOW_U;
	else if(buf == "V")
		m_showYUV = YUV_SHOW_V;
	else{
		err_msg = "请选择正确的显示分量";
		goto leave;
	}
	m_render.setShowYUV(m_showYUV);


	if(getComBoxStr(IDC_COMBO_FPS,buf) < 0){
		err_msg = "请选择帧率";
		goto leave;
	}
	if(myGetIntFromStr(buf,&m_fps) < 0 || m_fps <= 0){
		err_msg = "帧率参数错误";
		goto leave;
	}
	
	m_bLoop = isChecked(IDC_CHECK_LOOP);

	change = 0;
	if(!isChecked(IDC_CHECK_GRID)){
		if(m_bGridOn){
			change = 1;
			m_bGridOn = 0;
		}
	}
	else{
		CString buf;
		int xn,yn;
		if(m_bGridOn == 0){
			change = 1;
			m_bGridOn = 1;
		}
		if(getComBoxStr(IDC_COMBO_GRIDXN,buf) < 0){
			err_msg = "网格数目错误";
			goto leave;
		}
		if(myGetRangeUint(buf,'x',&xn,&yn,0) != 2){
			err_msg = "网格数目错误";
			goto leave;
		}
		if(m_gridXNum != xn || m_gridYNum != yn){
			m_gridXNum = xn;
			m_gridYNum = yn;
			change = 1;
		}
	}

	int ret;
	ret = m_imageWnd.setGridColor(m_gridColor);
	ret = m_imageWnd.enableGrid(m_bGridOn,m_gridXNum,m_gridYNum);
	if(ret < 0)
		goto leave;
	
	if(m_bCompare == 1)
		updateCompare2();

	return 1;

leave:
	if( level == 0 && !err_msg.IsEmpty()){
		printMsg(err_msg);
	}
	return -1;
}

int CYuvplayDlg::updateFinal(){
	int ret;
	
	if(m_fileName.IsEmpty())
		return 0;

	CString errMsg;

	m_yuvDoc.setFileName(m_fileName);
	ret = m_yuvDoc.update();  	// -1: 失败,0:无变化,1:更新
	if( ret  < 0){
		setState(STATE_NOVIDEO);		// 无图像
		errMsg = m_yuvDoc.getErrMsg();
		goto leave;
	}
	else if(ret == 0){  // 无变化,或无数据
		return 0;
	}
	else{		// 有变化,文件已经重启,可显示
		int n;
		m_bReady = 1;
		n = displayFrame();
		if( n >= 0){
			if(n > 0)
				setState(STATE_VIDEOSTART);
			m_imageWnd.setTitle(m_fileName);
			m_imageWnd.ShowWindow(SW_SHOW);
		}
	}
	
	
	// added. 2008/03/21
	if(m_yuvDoc.getFrameNum() == 1)
		setState(STATE_ONEFRAME);

	return 1;

leave:
	if(errMsg.IsEmpty() == 0)
		MessageBox(errMsg);
	return ret;
}


//
//  图像数据通信
//
int CYuvplayDlg::displayFrame(){
	if(m_bReady == 0)
		return 0;

	int framePos,frameNum;
	
	// 帧数目信息
	framePos = m_yuvDoc.getFrameIndex();
	frameNum = m_yuvDoc.getFrameNum();
	if(framePos < 0 || frameNum <= 0)
		return -1;
	
	CSliderCtrl *pCS = (CSliderCtrl*) GetDlgItem(IDC_SLIDER_PLAY);
	if(framePos == 1){
		pCS->SetRange(1,frameNum,1);
	}
	pCS->SetPos(framePos);
	m_buf.Format("%d/%d",framePos,frameNum);
	
	SetDlgItemText(IDC_STATIC_FRAMENUM, m_buf);
	
	unsigned char *pBuf;
	int bufSz;
	int ret;
	ret = m_render.getBuffer(&pBuf, &bufSz);
	if(ret < 0){
		m_buf.Format("OOPS: 0x1001, %d. ",ret);
		if(m_render.getErrMsg())
			m_buf += m_render.getErrMsg();
		printMsg(m_buf);
		return -2;
	}
	ret = m_yuvDoc.getFrame(pBuf, bufSz);
	if(ret < 0){
		m_buf.Format("OOPS: 0x1002, %d. ",ret);
		if(m_yuvDoc.getErrMsg())
			m_buf += m_yuvDoc.getErrMsg();
		printMsg(m_buf);
		return -2;
	}
	ret = m_render.putData(pBuf);
	if(ret < 0){
		m_buf.Format("OOPS: 0x1003, %d. ",ret);
		if(m_render.getErrMsg())
			m_buf += m_render.getErrMsg();
		printMsg(m_buf);
		return -2;
	}

	notifyView();
	/*
	if(framePos == frameNum && !m_bLoop){
		//imageEnd(0);
		return 0;
	}
	*/

	return 1;
}

void CYuvplayDlg::notifyView(int id){
	::PostMessage(m_imageWnd.m_hWnd,WM_VIEW,m_notifyId,0);
	if(m_notifyId>0){
		m_notifyId = 0;
	}
}

// do setup at image start
int CYuvplayDlg::imageStart(){
	setState(STATE_VIDEOSTART);

	return 1;
}

// do clear at image end
//
//   设置为停止状态
//
int CYuvplayDlg::imageEnd(int back){
	m_playing = 0;
	setState(STATE_VIDEOEND);

	return 1;
}

int CYuvplayDlg::noImage(){
	setState(STATE_NOVIDEO);

	return 1;
}

int CYuvplayDlg::startPlay(){
	if(m_bReady == 0)
		return -1;
	if(m_fps <= 0){
		printMsg("错误的帧率");
		return -2;
	}
	if(m_yuvDoc.getFrameNum() - m_yuvDoc.getFrameIndex() <= 1)
		return 0;

	if(m_playing <= 0){
		int n;
		n = 1000/m_fps - 1;
		if(n <= 0)
			n = 39;
		SetTimer(ID_TIMER1,n,NULL);
	}
	else if(m_playing != 2){		// pause
		m_playing = 0;
		return -2;
	}

	m_playing = 1;

	setState(STATE_PLAYING);
	
	return 1;
}

// 
//  停止播放专用
//
int CYuvplayDlg::endPlay(){
	m_playing = 0;
	KillTimer(ID_TIMER1);

	return 1;
}

void CYuvplayDlg::setEditReadOnly(int id, int bReadOnly){
	CEdit *pEdit = (CEdit*)GetDlgItem(id);
	if(pEdit)
		pEdit->SetReadOnly(bReadOnly);
}

int  CYuvplayDlg::setState(int state){
	int enable;

	if(state == STATE_NOVIDEO){
		enable = TRUE;
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enable = FALSE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_PAUSE,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_BUTTON_PREV,enable);
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,1);
	}
	else if(state == STATE_VIDEOSTART){
		enable = TRUE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		enable = FALSE;
		enableWnd(IDC_BUTTON_PAUSE,enable);
		enableWnd(IDC_BUTTON_PREV,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,0);
	}
	else if(state == STATE_VIDEOMID){
		enable = TRUE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_BUTTON_PREV,enable);	
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		enable = FALSE;
		enableWnd(IDC_BUTTON_PAUSE,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,0);
	}
	else if(state == STATE_VIDEOEND){
		enable = TRUE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_BUTTON_PREV,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		enable = FALSE;
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_PAUSE,enable);
		enableWnd(IDC_BUTTON_NEXT,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,0);
	}
	else if(state == STATE_PLAYING){
		enable = TRUE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_PAUSE,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		//enableWnd(IDC_CHECK_COMPARE,m_bCompare == enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enable = FALSE;
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_PREV,enable);
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,1);
	}
	else if(state == STATE_PAUSE){
		enable = TRUE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_BUTTON_PREV,enable);	
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enable = FALSE;
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_PAUSE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,0);
	}
	else if(state == STATE_ONEFRAME){
		enable = TRUE;
		enableWnd(IDC_BUTTON_OPEN,enable);
		enableWnd(IDC_BUTTON_STOP,enable);
		enableWnd(IDC_CHECK_COMPARE,enable);
		enableWnd(IDC_BUTTON_SAVE,enable);
		enableWnd(IDC_BUTTON_APPLY,enable);
		enableWnd(IDC_BUTTON_TRANSCODE,enable);
		enable = FALSE;
		enableWnd(IDC_SLIDER_PLAY,enable);
		enableWnd(IDC_BUTTON_PLAY,enable);
		enableWnd(IDC_BUTTON_PREV,enable);	
		enableWnd(IDC_BUTTON_NEXT,enable);
		enableWnd(IDC_BUTTON_PAUSE,enable);
		setEditReadOnly(IDC_STATIC_FRAMENUM,0);
	}

	return 1;
}

int  CYuvplayDlg::closeFile(){
	setState(STATE_NOVIDEO);
	
	m_imageWnd.clear();

	m_yuvDoc.destroy();

	CSliderCtrl *pCS = (CSliderCtrl*) GetDlgItem(IDC_SLIDER_PLAY);
	pCS->SetRange(0,0,1);
	
	SetDlgItemText(IDC_STATIC_FRAMENUM,"0/0");

	return 1;
}

// compare
int  CYuvplayDlg::updateCompare1(int level){
	if(m_pCmpDlg->m_width != m_width 
		|| m_pCmpDlg->m_height != m_height){
		m_pCmpDlg->m_width = m_width;
		m_pCmpDlg->m_height = m_height;
		m_pCmpDlg->m_notifyId = 1;
		m_pCmpDlg->m_notifyId2 = 1;
	}
	
	return m_pCmpDlg->m_notifyId || m_pCmpDlg->m_notifyId2;
}

int  CYuvplayDlg::updateCompare2(int level){
	if(m_pCmpDlg->m_showYUV != m_showYUV){
		m_pCmpDlg->m_showYUV = m_showYUV;
		m_pCmpDlg->m_notifyId = 1;
	}
	m_pCmpDlg->m_bLoop = m_bLoop;

	m_pCmpDlg->m_bGridOn = m_bGridOn;
	m_pCmpDlg->m_gridXNum = m_gridXNum;
	m_pCmpDlg->m_gridYNum = m_gridYNum;
	m_pCmpDlg->m_gridColor = m_gridColor;
	
	if(level > 0)
		m_pCmpDlg->updateConf2();

	return m_pCmpDlg->m_notifyId;
}

void  CYuvplayDlg::notifyCompare(int id){
	::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,id,0);
}

int CYuvplayDlg::openFile(LPCTSTR fileName, int bUseFile, int bUpdateRencent){
	if(bUseFile){
		m_fileName = fileName;
	}
	else{
		CFileDialog dlgFile(TRUE);
		CString buffer;
		char *filefilter = "all files(*.*)""\0""*""\0"
			"dat file(*.dat)""\0""*.dat""\0"
			"yuv file(*.yuv)""\0""*.yuv""\0\0";

		dlgFile.m_ofn.lpstrFile = buffer.GetBuffer(MAX_PATH*2);
		dlgFile.m_ofn.nMaxFile = MAX_PATH*2;
		dlgFile.m_ofn.lpstrTitle = "打开文件...";
		dlgFile.m_ofn.nFilterIndex = 3;
		dlgFile.m_ofn.lpstrFilter = (LPCTSTR)filefilter;
		dlgFile.m_ofn.lpstrInitialDir = fileName;

		INT_PTR nRet = dlgFile.DoModal();
		if(nRet != IDOK){	
			return 0;   // 取消
		}

		m_fileName = dlgFile.GetPathName();
	}

	if(m_bReady){
		m_bReady = 0;
		closeFile();
	}

	int ret;
	ret = updateConf1(0);
	if(ret < 0)	goto leave;
	ret = updateConf2(0);
	if(ret < 0)	goto leave;
	ret = updateFinal();
	if(ret < 0)	goto leave;
	if(bUpdateRencent){
		int id;

		if(m_pRecentList->getItemId((LPCTSTR)m_fileName,id) > 0){
			m_pRecentMenu->DeleteMenu(id, MF_BYCOMMAND);
		}
		else{
			if(m_pRecentList->isFull()){
				int num = m_pRecentMenu->GetMenuItemCount();

				id = m_pRecentMenu->GetMenuItemID(num-1);

				m_pRecentList->deleteItem(id);
				m_pRecentMenu->DeleteMenu(id, MF_BYCOMMAND);
			}

			m_pRecentList->appendItem(m_fileName,id);
		}

		m_pRecentMenu->InsertMenu(2,MF_STRING|MF_BYPOSITION,id, (LPCTSTR)m_fileName);
	}

	return 1;

leave:
	return ret;
}


///////////////////////////////////////////////////////////////////////////
///
//
//
void CYuvplayDlg::OnDropFiles(HDROP hDropInfo)
{
	CString buffer;
	CEdit *pEdit;
	int fileNum;
	int ret;

	if(m_playing > 0)
		return;

	fileNum = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);

	ret = DragQueryFile(hDropInfo,0,buffer.GetBuffer(MAX_PATH*2),MAX_PATH*2);
	buffer.ReleaseBuffer();

	::DragFinish(hDropInfo);

	openFile(buffer);
}

BOOL CYuvplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		//strAboutMenu.LoadString(IDS_ABOUTBOX);
		strAboutMenu = "关于+帮助";
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING,IDM_REGIST,"设置文件关联");
		pSysMenu->AppendMenu(MF_STRING,IDM_CENTER_WINDOW,"窗口居中");
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	
	SetDlgItemText(IDC_STATIC_HELP, HELP_TEXT);

	CString buf1;
	CString path;
	
	GetModuleFileName(NULL, buf1.GetBuffer(MAX_PATH), MAX_PATH);
	buf1.ReleaseBuffer();
	int pos = buf1.ReverseFind('\\');
	path = buf1.Left(pos+1);
	gbl_formater_man.init(path);
	m_ppFormaters = gbl_formater_man.getList();
	if(m_ppFormaters){
		for(int i = 0;;i++){
			if(m_ppFormaters[i] == NULL)
				break;
			const char *name = m_ppFormaters[i]->pItem->getName();
			int index = addComBoxItem(IDC_COMBO_FORMAT, name, i);
		}
	}
	else {
		printMsg("没有找到任何格式插件, 无法浏览任何yuv文件");
	}

	CRect rect;
	CRect rect2;

	GetWindowRect(&rect);
	m_fullSize.cx = rect.Width();
	m_fullSize.cy = rect.Height();
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_EXT);
	pBtn->GetWindowRect(&rect2);
	m_upSize.cx = rect.Width();
	m_upSize.cy = rect2.bottom - rect.top + 3;

	m_imageWnd.Create(this,1,"image window");
	m_imageWnd.ShowWindow(SW_HIDE);
	m_imageWnd.setGridColor(m_gridColor);
	m_imageWnd.m_pRender = &m_render;
	m_imageWnd.m_hParent = m_hWnd;

	setComBoxSel(IDC_COMBO_SIZE,1);
	OnSelchangeComboSize();
	setComBoxSel(IDC_COMBO_SHOWYUV,0);
	setComBoxSel(IDC_COMBO_FORMAT,0);
	setComBoxSel(IDC_COMBO_FPS,5);
	setComBoxSel(IDC_COMBO_GRIDXN,3);
	//addComBoxStr(IDC_COMBO_FPS,"25");
	setComBoxSel(IDC_COMBO_FIELD, 0);

	enableWnd(IDC_BUTTON_FILTER_CFG, 0);

	// disable something
	enableWnd(IDC_COMBO_GRIDXN,0);
	//enableWnd(IDC_CHECK_COMPARE,0);
	//OnMore();

	
	noImage();
	
	SetDlgItemText(IDC_STATIC_FRAMENUM, "0/0");
	setEditReadOnly(IDC_STATIC_FRAMENUM,1);
	enableCheck(IDC_CHECK_EXT, 0);
	OnCheckExt();

	// set window position
	// 降低窗口的默认高度
	int width, height;
	int cx_screen = GetSystemMetrics(SM_CXSCREEN);
	int cy_screen = GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(&rect);
	width = rect.Width();
	height = rect.Height();
	CWnd *pW = GetDlgItem(IDC_STATIC_FRAME);
	pW->GetWindowRect(&rect2);
	int height2 = rect2.top - rect.top;
	rect.left = (cx_screen - width)/2;
	rect.right = rect.left + width;
	rect.top = (cy_screen - height2)/2;
	rect.bottom = rect.top + height;
	MoveWindow(&rect);

	// 
	loadConfigFile();
	initMenu();

	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_OPEN));
	//enableWnd(IDC_STATIC_FRAMENUM, 0);
	//enableWnd(IDC_STATIC_FRAMENUM, 1);

	// 命令行
	if(m_lpCmdline){
		char* fileName = m_lpCmdline;
		if(*fileName == '\"' || *fileName == '\''){
			fileName++;
			int n = strlen(fileName);

			if(fileName[n-1] == '\"' || fileName[n-1] == '\'')
				fileName[n-1] = 0;
		}

		m_fileName = fileName;
		if(!m_fileName.IsEmpty()){
			PostMessage(WM_COMPARE, 2012, 0);
		}
	}
	
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CYuvplayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if((nID & 0xFFF0) == IDM_REGIST)
	{
		CRegKey reg;
		CString buf;
		CString buf2;
		::GetModuleFileName(NULL,buf.GetBuffer(MAX_PATH*2),MAX_PATH*2);
		buf.ReleaseBuffer();

		reg.Create(HKEY_CLASSES_ROOT,"yuvfile");
		buf2 = buf + "";
		reg.SetKeyValue("DefaultIcon",buf2);
		reg.Create(reg.m_hKey,"shell");
		reg.Create(reg.m_hKey,"edit");
		buf2 = buf + " %1";
		reg.SetKeyValue("command",buf2);

		reg.Create(HKEY_CLASSES_ROOT,".yuv");
		reg.Close();
		reg.Open(HKEY_CLASSES_ROOT,NULL);
		reg.SetKeyValue(".yuv","yuvfile");

		SHChangeNotify(SHCNE_ASSOCCHANGED,   SHCNF_IDLIST,   0,   0);

		printMsg("关联至.yuv文件");
		
	}
	else if((nID & 0xFFF0) == IDM_CENTER_WINDOW){
		WINDOWPLACEMENT place;
		GetWindowPlacement(&place);
		if(place.showCmd == SW_SHOWMINIMIZED)
			ShowWindow(SW_SHOWNORMAL);

		CRect rect;
		int width, height;
		int cx_screen = GetSystemMetrics(SM_CXSCREEN);
		int cy_screen = GetSystemMetrics(SM_CYSCREEN);
		GetWindowRect(&rect);
		width = rect.Width();
		height = rect.Height();
		CWnd *pW = GetDlgItem(IDC_STATIC_FRAME);
		CRect rect2;
		pW->GetWindowRect(&rect2);
		int height2 = rect2.top - rect.top;
		rect.left = (cx_screen - width)/2;
		rect.right = rect.left + width;
		rect.top = (cy_screen - height2)/2;
		rect.bottom = rect.top + height;
		MoveWindow(&rect);

		SetForegroundWindow();
	}
	else{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CYuvplayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CYuvplayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CYuvplayDlg::OnMore() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	CRect rect2;
	CButton *pBtn;

	GetWindowRect(&rect);
	pBtn = (CButton*)GetDlgItem(IDC_BUTTON_MORE);
	pBtn->GetWindowRect(&rect2);

	if(rect.bottom < rect2.bottom + 20){
		rect.bottom = rect.top + m_upSize.cy;
		MoveWindow(&rect);
		CWnd *pWndCtl = GetWindow(GW_CHILD);
		while (pWndCtl != NULL){ 
			pWndCtl->GetWindowRect (rect2);
			if(rect2.bottom < rect.bottom)
				pWndCtl->ShowWindow(SW_SHOW);
			pWndCtl = pWndCtl->GetWindow (GW_HWNDNEXT);
		}
		pBtn->SetWindowText("||");
		OnCheckExt();
	}
	else{
		rect.bottom = rect2.bottom + 8;
		MoveWindow(&rect);
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd){
			pWnd->GetWindowRect(&rect2);
			if(rect2.top > rect.bottom)
				pWnd->ShowWindow(SW_HIDE);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
		pBtn->SetWindowText(">>");
	}

	//
	GetWindowRect(&rect);
	CWnd *pWnd = FindWindow("Shell_TrayWnd",NULL);
	if(pWnd == NULL)
		return;

	pWnd->GetWindowRect(&rect2);
	
	int n = rect.bottom - rect2.top;
	if(n > 0 && rect.top > n){
		rect.top -= n;
		rect.bottom -= n;
		MoveWindow(&rect);
	}
}

void CYuvplayDlg::OnOpenFile() 
{
	// TODO: Add your control notification handler code here
	openFile(m_fileName,0);
}

void CYuvplayDlg::OnCompare() 
{
	// TODO: Add your control notification handler code here
	int  flag = 0;

	if(m_pCmpDlg == NULL){
		m_pCmpDlg = new CmpDlg(this);
		m_pCmpDlg->m_hParent = m_hWnd;
		m_pCmpDlg->m_pRender = &m_render;
		//m_pCmpDlg->setLoop(m_bLoop);
				
		m_pCmpDlg->setStartPath(m_fileName);

		m_pCmpDlg->m_width = m_width;
		m_pCmpDlg->m_height = m_height;
		m_pCmpDlg->m_format_id = m_format_id;
		m_pCmpDlg->m_field_id = m_field_id;
		m_pCmpDlg->m_showYUV = m_showYUV;

		m_pCmpDlg->Create(IDD_DLG_COMPARE,this);
		
		m_pCmpDlg->setCmpFormat(&m_formatLocal);

		flag = 1;
	}

	if(isChecked(IDC_CHECK_COMPARE)  && m_bReady){
		updateCompare1(0);
		m_bCompare = 1;

		CRect rect1;
		CRect rect2;
		CRect rect3;

		GetWindowRect(&rect1);
		m_pCmpDlg->GetWindowRect(&rect2);

		rect3 = rect1;
		rect3.left = rect3.right;
		rect3.right = rect3.left + rect2.Width();
		rect3.bottom = rect3.top + rect2.Height();

		if(flag == 1){
			int cxScreen = GetSystemMetrics(SM_CXFULLSCREEN);
			
			if(rect3.right > cxScreen){
				int cx,offset = 0;
				cx = rect1.Width() + rect3.Width();
				if(cx < cxScreen){
					offset = rect3.right - cxScreen;
				}
				else if(cx > cxScreen && rect1.left != 0){
					offset = rect1.left;
				}
				
				if(offset != 0){
					rect1.left -= offset;
					rect1.right -= offset;
					rect3.left -= offset;
					rect3.right -= offset;

					MoveWindow(&rect1);
				}
			}
		}

		m_pCmpDlg->MoveWindow(&rect3,1);
		m_pCmpDlg->show();
	}
	else{
		m_pCmpDlg->clear();
		m_pCmpDlg->ShowWindow(SW_HIDE);
		m_bCompare = 0;
	}

}

void CYuvplayDlg::OnButtonAddsize() 
{
	// TODO: Add your control notification handler code here
	CString buf;
	int width,height;

	if(getImageSize(width,height) < 0){
		buf = "图像尺寸有误";
		printMsg(buf);
		return;
	}

	buf.Format("%dx%d",width,height);


	{
		CComboBox *pBox;
		int nIndex;
		pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
		nIndex = pBox->FindString(0,buf);
		if(nIndex >= 0){
			MessageBox("分辨率已存在");
			return;
		}
	}

	CDlgAString dlg;
	if(dlg.DoModal() == IDOK){
		if(!dlg.m_str.IsEmpty()){
			buf += "(";
			buf += dlg.m_str;
			buf += ")";
		}
	}

	addComBoxStr(IDC_COMBO_SIZE,buf);
}

void CYuvplayDlg::OnButtonDelsize() 
{
	// TODO: Add your control notification handler code here
	delComBoxStr(IDC_COMBO_SIZE,0);
}

void CYuvplayDlg::OnChangeImageSize() 
{
	// TODO: Add your control notification handler code here

}

void CYuvplayDlg::OnSelchangeComboSize() 
{
	// TODO: Add your control notification handler code here
	int width,height;
	CString buf;
	CString buf2;
	long pos;
	int nIndex;

	if(getComBoxStr(IDC_COMBO_SIZE,buf2) < 0)
		return;
	pos = buf2.Find('(',0);
	if(pos > 0){
		buf = buf2.Left(pos);
	}
	else{
		buf = buf2;
	}

	if(myGetRangeUint(buf,'x',&width,&height,0) < 2)
		return;

	setEditInt(IDC_EDIT_WIDTH,width);
	setEditInt(IDC_EDIT_HEIGHT,height);	
}

void CYuvplayDlg::OnCheckGrid() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_GRID);
	int ret = 1;

	if(m_bReady)
		ret = updateConf2(0);
	if(ret < 0)
		enableCheck(IDC_CHECK_GRID, 0);
	enableWnd(IDC_COMBO_GRIDXN, pBtn->GetCheck() == BST_CHECKED);

}

void CYuvplayDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(m_playing == 1)		// is playing
		return;
	
	if(m_playing == 2){	// paused
		if(updateConf2() > 0)
			displayFrame();
		return;
	}

	int ret;
	ret = updateConf1();
	if(ret >= 0)
		ret = updateConf2();

	if(ret > 0 && m_fileName.IsEmpty() == 0)
		ret = updateFinal();
}

void CYuvplayDlg::OnPrevFrame() 
{
	// TODO: Add your control notification handler code here
	int n;
	int num = m_yuvDoc.getFrameNum();
	
	if(m_playing == 1)
		return;
	
	n = m_yuvDoc.getFrameIndex();
	if(n <= 1)
		return;
	
	if(n == num && m_playing <= 0){
		setState(STATE_VIDEOMID);
	}
	
	n = m_yuvDoc.seekFrameBackward(1);
	if(n > 0)
		displayFrame();
	else{
		setState(STATE_VIDEOSTART);
	}
	
	if(m_bCompare == 1)
		notifyCompare(CMP_PREV_FRAME);
	
	// added. 2008/03/21
	if(m_yuvDoc.getFrameIndex() == 1 && m_playing <= 0)
		setState(STATE_VIDEOSTART);
}

void CYuvplayDlg::OnNextFrame() 
{
	// TODO: Add your control notification handler code here
	int seek_flag = 1;
//	if(m_playing > 0){
		if(m_yuvDoc.getFrameIndex() == m_yuvDoc.getFrameNum()){
			if(!m_bLoop){
				imageEnd(0);
				return;
			}
			m_yuvDoc.restart();
			seek_flag = 0;
		}

//	}

	if(m_yuvDoc.getFrameIndex() == 1 && m_playing <= 0){
		setState(STATE_VIDEOMID);
	}

	if(m_yuvDoc.seekFrameForward(seek_flag) > 0){
		displayFrame();

		if(m_bCompare == 1)
			notifyCompare(CMP_NEXT_FRAME);
	}

	// added. 此处不用考虑是否在播放中,前面已经检查过了
	if(m_yuvDoc.getFrameIndex() == m_yuvDoc.getFrameNum() && !m_bLoop)
		imageEnd(0);

}

void CYuvplayDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
	if(!m_bReady){
		return;
	}

	startPlay();
}

// 停止显示第一帧
void CYuvplayDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	if(m_playing == 1)
		endPlay();

	m_playing = 0;
	if(!m_bReady)
		return;

	if(m_pCmpDlg != NULL)	// m_bCompare ->
		::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,CMP_STOP,0);

	if( m_yuvDoc.getFrameIndex() == 1 ){
		closeFile();
		m_bReady = 0;

		if(m_pCmpDlg != NULL){	// m_bCompare ->	// added
			m_pCmpDlg->clear(2);
			m_bCompare = 0;
		}

		return;
	}

	// 显示第一帧
	if(m_yuvDoc.seekFrame(0,SEEK_SET) >= 0){
		if(m_yuvDoc.getFrameIndex() == 1){
			setState(STATE_VIDEOSTART);
		}

		displayFrame();
	}
}


void CYuvplayDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_playing == 1)
		OnNextFrame();

	CDialog::OnTimer(nIDEvent);
}

void CYuvplayDlg::OnPause() 
{
	// TODO: Add your control notification handler code here
	if(m_playing == 1){
		m_playing = 2;
		setState(STATE_PAUSE);
	}
}

void CYuvplayDlg::OnChangeFps() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FPS);
	if(pBox == NULL)
		return;
}

void CYuvplayDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int pos;

	
	*pResult = 0;
}

void CYuvplayDlg::OnCheckLoop() 
{
	// TODO: Add your control notification handler code here
	m_bLoop = isChecked(IDC_CHECK_LOOP);
	if(m_pCmpDlg != NULL)
		m_pCmpDlg->setLoop(m_bLoop);
}

void CYuvplayDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags&MK_LBUTTON)
	{
		POINT pt;
		pt.x=point.x;pt.y=point.y;
		ClientToScreen(&pt);
		SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
		if(m_pCmpDlg){
			;//::SendMessage(m_pCmpDlg->m_hWnd,WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CYuvplayDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_playing > 0)
		return;

	CPoint pt2;
	CRect rect2;
	CButton *pBtn;

	GetWindowRect(&rect2);
	pBtn = (CButton*)GetDlgItem(IDC_BUTTON_MORE);
	pBtn->GetWindowRect(&rect2);
	pt2.y = rect2.bottom;
	ScreenToClient(&pt2);

	if(point.y > pt2.y)
		OnOpenFile();

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CYuvplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	static int flag = 0;
	int n;
	int nNow;
	CSliderCtrl *pCS;

	if(m_bReady == 0)
		goto leave;

	
	if(flag != 0){
		flag=  0;
		goto leave;
	}

	flag++;
	pCS = (CSliderCtrl*) GetDlgItem(IDC_SLIDER_PLAY);
	n = pCS->GetPos();
	nNow = m_yuvDoc.getFrameIndex();
	if(n == nNow)
		goto leave;
	
	if(n > nNow){
		if(nNow == 1)
			setState(STATE_VIDEOMID);
		
		
		n--;
		m_yuvDoc.seekFrame(n,SEEK_SET);
		if(m_bCompare)
			::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,CMP_SEEK,n);
		OnNextFrame();
	}
	else{
		if(nNow == m_yuvDoc.getFrameNum())
			setState(STATE_VIDEOMID);

		n++;
		m_yuvDoc.seekFrame(n,SEEK_SET);
		if(m_bCompare)
			::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,CMP_SEEK,n);
		OnPrevFrame();
	}

leave:
	;
}

void CYuvplayDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if(m_bReady <= 0){
		printMsg("无图像");
		return;
	}

	CFileDialog dlgFile(FALSE);
	CString buffer;
	CString errMsg;
	char *filefilter = "all files(*.*)""\0""*""\0"
					   "bmp file(*.bmp)""\0""*.bmp""\0"
					   "rgb file(*.rgb)""\0""*.rgb""\0"
					   "yuv file(*.yuv)""\0""*.yuv""\0\0";

	dlgFile.m_ofn.lpstrFile = buffer.GetBuffer(MAX_PATH);
	dlgFile.m_ofn.nMaxFile = MAX_PATH;
	dlgFile.m_ofn.lpstrTitle = "保存本帧到文件...";
	dlgFile.m_ofn.nFilterIndex = 2;
	dlgFile.m_ofn.lpstrFilter = (LPCTSTR)filefilter;

	INT_PTR nRet = dlgFile.DoModal();
	if(nRet != IDOK)
		return;

	CString  fileName;
	CString  ext;
	int type;

	fileName = dlgFile.GetPathName();

	//ext = getExtFileName(fileName,0);
	ext = dlgFile.GetFileExt();
	if(ext.IsEmpty()){
		if(dlgFile.m_ofn.nFilterIndex == 2){
			ext = "bmp";
			fileName += ".bmp";
		}
		else if(dlgFile.m_ofn.nFilterIndex == 3){
			ext = "rgb";
			fileName += ".rgb";
		}
		else{
			ext = "yuv";
			fileName += ".yuv";
		}
	}
	
	CStdioFile outFile(fileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if(outFile.m_hFile == CFile::hFileNull){
		errMsg = "打开文件失败";
		goto leave;
	}

	if(ext.CompareNoCase("bmp") == 0 || ext.CompareNoCase("rgb") == 0){
		BITMAPINFOHEADER *pBmpInfo;
		unsigned char *pBmpBits;
		int len1,len2;
		BITMAPFILEHEADER bfh;

		m_render.getImageBmp(&pBmpInfo, &pBmpBits);
		if(pBmpInfo == NULL || pBmpBits == NULL){
			errMsg = "获取图像数据失败";
			goto leave;
		}
		
		len1 = sizeof(*pBmpInfo);
		len2 = pBmpInfo->biSizeImage;

		if(ext.CompareNoCase("bmp") == 0){
			memset(&bfh,0,sizeof(bfh));
			bfh.bfType = 'B' + ('M'<<8);
			bfh.bfSize = sizeof(bfh) + len1 + len2;
			bfh.bfOffBits = sizeof(bfh) + len1;

			outFile.Write(&bfh,sizeof(bfh));
			outFile.Write(pBmpInfo,len1);
		}

		outFile.Write(pBmpBits,len2);
	}
	else{
		const unsigned char *pYuv;
		int len;

		pYuv = m_render.getRaw(&len); 
		if(pYuv == NULL){
			errMsg = "获取图像数据失败";
			goto leave;
		}

		outFile.Write(pYuv,len);
	}
	outFile.Close();

leave:
	if(!errMsg.IsEmpty()){
		AfxMessageBox(errMsg);
	}
}

void CYuvplayDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CYuvplayDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN){
		int nChar = pMsg->wParam;
		if(nChar == VK_RETURN){
			CWnd *pWnd = GetDlgItem(IDC_STATIC_FRAMENUM);
			if(pWnd == GetFocus()){
				int n, nNow;
				
				if(m_bReady == 0 || m_playing == 1)
					return TRUE;
				
				GetDlgItemText(IDC_STATIC_FRAMENUM, m_buf);

				if(sscanf((LPCTSTR)m_buf,"%d",&n) != 1){
					printMsg("输入数值错误, 请直接输入要跳到的帧号, 如: 100");
					return TRUE;
				}
				
				if(n <= 0)  n = 1;
				if(n > m_yuvDoc.getFrameNum())  n = m_yuvDoc.getFrameNum();

				nNow = m_yuvDoc.getFrameIndex();
				if(n == nNow){
					//displayFrame();
					return 1;
				}
				
				if(n > nNow){
					if(nNow == 1)
						setState(STATE_VIDEOMID);
					m_yuvDoc.seekFrameForward(n - nNow - 1);
					n = m_yuvDoc.getFrameIndex();
					if(m_bCompare)
						::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,CMP_SEEK,n);

					OnNextFrame();
				}
				else{
					if(nNow == m_yuvDoc.getFrameNum())
						setState(STATE_VIDEOMID);
					m_yuvDoc.seekFrameBackward(nNow - n - 1);
					if(m_bCompare)
						::PostMessage(m_pCmpDlg->m_hWnd,WM_COMPARE,CMP_SEEK,-n);
					OnPrevFrame();
				}

				return TRUE;
			}
		}
		/*
		else if(nChar == VK_RIGHT){
			if(m_bReady){
				OnNextFrame();
				return TRUE;
			}
		}
		else if(nChar == VK_LEFT){
			if(m_bReady){
				OnPrevFrame();
				return TRUE;
			}
		}*/
		
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CYuvplayDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CYuvplayDlg::OnButtonGridColor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = RGB(128,128,128);
	
	if(dlg.DoModal() == IDOK){
		m_gridColor = dlg.GetColor();
		//m_imageWnd.setGridColor(m_gridColor);
		updateConf2();
	}	
}

void CYuvplayDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if(m_bCompare){
		CRect rect1;
		CRect rect2;
		GetWindowRect(&rect1);
		m_pCmpDlg->GetWindowRect(&rect2);

		rect1.left += rect1.Width();
		rect1.right = rect1.left + rect2.Width();
		rect1.bottom = rect1.top + rect2.Height();

		m_pCmpDlg->MoveWindow(&rect1,1);
	}
}



void CYuvplayDlg::OnChangeFormat() 
{
	// TODO: Add your control notification handler code here
	/*
	CString  buf;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	int index = pBox->GetCurSel();
	
	pBox->GetWindowText(buf);
	buf.TrimLeft(' ');
	if(buf[0] == '-')
		pBox->SetCurSel(index+1);

	pBox->GetWindowText(buf);

	buf.TrimLeft(' ');
	int pos = buf.Find('(',0);
	if(pos >= 0)
		buf = buf.Left(pos);

	if((!buf.CompareNoCase("Y")) || (!buf.CompareNoCase("Y8")) ){
		//setComBoxSel(IDC_COMBO_SHOWYUV,"Y");
		enableWnd(IDC_COMBO_SHOWYUV,0);
	}
	else{
		enableWnd(IDC_COMBO_SHOWYUV,1);
	}
*/
}

void CYuvplayDlg::OnButtonTranscode() 
{
	// TODO: Add your control notification handler code here
	DlgTranscode dlg;
	dlg.m_ppFormaters = m_ppFormaters;
	dlg.m_pYuvDoc = &m_yuvDoc;
	dlg.m_pRender = &m_render;
	dlg.DoModal();
}

void CYuvplayDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_playing){
		endPlay();
	}

	if(m_imageWnd.m_hWnd != NULL)
		m_imageWnd.destroy();
	
	if(m_pCmpDlg != NULL)
		m_pCmpDlg->destroy();

	//if(m_bReady)
	m_yuvDoc.destroy();

	saveConfigFile();
	
	//////////
	if(m_pMenu){
		m_pMenu->DestroyMenu();
		delete m_pMenu;
	}

	if(m_pRecentMenu){
		m_pRecentMenu->DestroyMenu();
		delete m_pRecentMenu;
	}

	if(m_pFavorList){
		m_pFavorList->destroy();
		delete m_pFavorList;
	}
	if(m_pRecentList){
		m_pRecentList->destroy();
		delete m_pRecentList;
	}
	
	gbl_formater_man.destroyList(m_ppFormaters);
	gbl_formater_man.destroy();
	gbl_fielter_nam.destroyList(m_ppFilters);
	gbl_fielter_nam.destroy();
	//////////////

	// WINVER 0x500
	//AnimateWindow(GetSafeHwnd(),100,AW_HIDE|AW_CENTER);

	CDialog::OnClose();
}

void CYuvplayDlg::OnBnClickedButtonBookmark()
{
	// TODO: 
	CPoint pt;	
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_BUTTON_BOOKMARK);
	pWnd->GetWindowRect(&rect);

	pt.x = rect.left;
	pt.y = rect.bottom;
	//ClientToScreen(&pt);
	//GetCursorPos(&pt);

	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
}

int CYuvplayDlg::loadConfigFile(){
	m_exePath = ".";
	
	::GetModuleFileName(NULL,m_buf.GetBuffer(MAX_PATH),MAX_PATH);
	m_buf.ReleaseBuffer();

	int pos = m_buf.ReverseFind('\\');
	m_exePath = m_buf.Left(pos+1);
	
	// load resulution presets
	CString  fileName = m_exePath + CONF_FILE;
	FILE *fp = NULL;
	char lineBuf[LINESZ];
	int n;
	char *p, *p1;
	int id;
	int flag;
	int num;

	fp = fopen(fileName, "r");
	if(fp == NULL)
		return 0;

	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, APP_STRING,strlen(APP_STRING)))
			break;
	}
	
	if(p != NULL){
		num = 0;
		while(1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			lineBuf[LINESZ-1] = 0;
			
			if(strncmp(p,"vcode",5) == 0){
				continue;
			}
			
			if(strncmp(p,"save_conf",9)){
				if(*p == '\n')
					continue;
				break;
			}

			p = strchr(lineBuf,'=');
			if(p == NULL)
				break;

			flag = 0;
			while(*++p){
				if(*p != ' ' && *p != '\t'){
					flag = 1;				
					break;
				}
			}
			
			if(flag == 0)
				continue;
			
			if(sscanf(p,"%d",&n) != 1)
				continue;
			if(n > 0){
				m_conf_file_flag = 1;
			}
			else{
				m_conf_file_flag =0;
			}
			enableCheck(IDC_CHECK_CONF, m_conf_file_flag);
		}
	}

	// look for "Resolution presets"
	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, RESOLUTIONS_STRING,strlen(RESOLUTIONS_STRING)))
			break;
	}

	if(p != NULL){
		int width, height;
		
		num = 0;
		while(1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			lineBuf[LINESZ-1] = 0;
			
			if(strncmp(p,"res",3)){
				if(*p == '\n')
					continue;
				break;
			}

			n = strlen(p);
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符

			p = strchr(lineBuf,'=');
			if(p == NULL)
				continue;
			
			flag = 0;
			while(*++p){
				if(*p != ' ' && *p != '\t'){
					flag = 1;				
					break;
				}
			}

			if(flag == 0)
				continue;

			if(sscanf(p,"%d",&width) != 1)
				continue;

			p1 = strchr(p,'x');
			if(p1 == NULL)
				p1 = strchr(p,'*');
			if(p1 == NULL)
				continue;
			p1++;
			if(sscanf(p1,"%d",&height) != 1)
				continue;

			if(width <= 0 || height <= 0)
				continue;

			if(num == 0){ // delete init resolutions
				CComboBox *pBox;
				int nItem;

				pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
				nItem = pBox->GetCount();
				while(nItem-- > 0){
					pBox->DeleteString(0);
				}
			}
			
			p1 = strchr(p1,'(');
			if(p1)
				m_buf.Format("%dx%d%s",width,height,p1);
			else
				m_buf.Format("%dx%d",width,height);
			addComBoxStr(IDC_COMBO_SIZE,m_buf);
			
			num++;
		}
	}

	// last state
	fseek(fp,0,SEEK_SET);
	// look for "Last state"
	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, LAST_STATE_STRING,strlen(LAST_STATE_STRING)))
			break;
	}
	
	if(p){
		int val;
		int more_flag = 0;

		num = 0;
		while(1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			lineBuf[LINESZ-1] = 0;

			n = strlen(p);
			if(n < 2){
				if(feof(fp))
					break;
				else
					continue;
			}
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符

			if(strncmp(p,"width",5) == 0){ // width
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val <= 0)
					continue;

				setEditInt(IDC_EDIT_WIDTH,val);
			}
			else if(strncmp(p,"height",6) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val <= 0)
					continue;

				setEditInt(IDC_EDIT_HEIGHT,val);
			}
			else if(strncmp(p,"resolution",10) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val < 0)
					continue;

				setComBoxSel(IDC_COMBO_SIZE,val);
			}
			else if(strncmp(p,"format",6) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val < 0)
					continue;

				setComBoxSel(IDC_COMBO_FORMAT,val);
			}
			else if(strncmp(p,"show",4) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val < 0)
					continue;

				setComBoxSel(IDC_COMBO_SHOWYUV, val);
			}
			else if(strncmp(p,"fps",3) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val <= 0)
					continue;
				m_buf.Format("%d",val);
				addComBoxStr(IDC_COMBO_FPS, m_buf);
			}
			else if(strncmp(p,"field",5) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;
				
				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val < 0)
					continue;
				
				m_buf.Format("%d",val);
				setComBoxSel(IDC_COMBO_FIELD, val);
			}
			else if(strncmp(p,"loop",4) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val <= 0)
					continue;

				enableCheck(IDC_CHECK_LOOP, val);
			}
			else if(strncmp(p,"more",4) == 0){
				p = strchr(lineBuf, '=');
				if(p == NULL)
					continue;

				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;
				
				if(sscanf(p,"%d",&val) != 1)
					continue;

				if(val){
					OnMore();
				}
			}
			else if(strncmp(p, "ext_pannel", 10) == 0){
				p = strchr(lineBuf, '=');
				if(p == NULL)
					continue;

				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;
				
				if(sscanf(p,"%d",&val) != 1)
					continue;

				if(val){
					CRect rect;
					GetWindowRect(&rect);
					if(rect.Height() >= m_upSize.cy){
						enableCheck(IDC_CHECK_EXT,1);
						OnCheckExt();
					}
				}
			}
			else if(strncmp(p,"grid",4) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;
				
				if(sscanf(p,"%d",&val) != 1)
					continue;
				if(val < 0)
					continue;
				
				setComBoxSel(IDC_COMBO_GRIDXN,val);
			}
			else if(strncmp(p,"pos",3) == 0){
				p = strchr(lineBuf, '=');
				if(p == NULL)
					continue;

				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;

				int x, y;
				if(*p == '(') p++;
				if(sscanf(p,"%d,%d",&x,&y) != 2)
					continue;
				
				CRect rect;
				GetWindowRect(&rect);
				int cx_screen = GetSystemMetrics(SM_CXSCREEN);
				int cy_screen = GetSystemMetrics(SM_CYSCREEN);

				if(-x >= rect.Width() - 10)
					x = -(rect.Width() - 10);
				if(x >= cx_screen - 10)
					x = cx_screen - 10;
				if(-y >= rect.Height() - 10)
					y = -(rect.Height() - 10);
				if(y >= cy_screen - 80)
					y = cy_screen - 80;

				val = rect.Width();
				rect.left = x;
				rect.right = x + val;
				val = rect.Height();
				rect.top = y;
				rect.bottom = y +val;
				MoveWindow(&rect);
			}
			else{
				break;
			}

		}		
	}

	// filter
	fseek(fp,0,SEEK_SET);
	// look for "filter"
	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, FILTERS_STRING,strlen(FILTERS_STRING)))
			break;
	}
	
	if(p){
		int val;
		int filter_flag = 0;
		
		while(1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			lineBuf[LINESZ-1] = 0;
			
			n = strlen(p);
			if(n < 2){
				if(feof(fp))
					break;
				else
					continue;
			}
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符
			
			if(strncmp(p, "on", 2) == 0){
				enableCheck(IDC_CHECK_FILTER, 1);
				OnCheckFilter();
				filter_flag = 1;
				break;
			}
			else if(strncmp(p, "off", 3) == 0){
				break;
			}
		}

		while(filter_flag == 1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			lineBuf[LINESZ-1] = 0;
			
			n = strlen(p);
			if(n < 2){
				if(feof(fp))
					break;
				else
					continue;
			}
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符
			
			if(strncmp(p, "filter", 6) == 0){
				p = strchr(lineBuf,'=');
				if(p == NULL)
					continue;
				
				flag = 0;
				while(*++p){
					if(*p != ' ' && *p != '\t'){
						flag = 1;				
						break;
					}
				}				
				if(flag == 0)
					continue;
				
				p++;
				char *p2 = p + strlen(p);
				while(p2 - p > 0){
					if(*p2 == '\n' || *p2 == '\t' || *p2 == ' ' || *p2 == 0)
						p2--;
					break;
				}
				if(*p2 != '"') continue;
				if(p2 - p == 0) continue;
				*p2 = 0;
				
				int i = 0;
				while(m_ppFilters[i]){
					if(strncmp(p, m_ppFilters[i]->pItem->getName(), p2-p) == 0){
						m_ppFilters[i]->flag = 1;
						m_ppFilters2[0] = m_ppFilters[i]->pItem;
						m_ppFilters2[1] = NULL;
						break;  // only one now!!! 因为不保存顺序
					}
					i++;
				}
			}
			else
				break;
		}

	}

	if(fp)
		fclose(fp);

	return 1;
}

int CYuvplayDlg::saveConfigFile(int bForce){
	if(bForce == 0 && m_conf_file_flag == 0)
		return 0;

	FILE *fp = NULL;
	CString fileName = m_exePath + CONF_FILE;
	
	int cnt;
	int num;
	int id;
	CString str;
	int n;
	int i;
	int val;
	
	int ini_level = 100;
	
	if(ini_level <= 0)
		return 0;

	fp = fopen(fileName,"w");
	if(fp == NULL){
		//m_buf.Format("打开配置文件\"%s\"失败",CONF_FILE);
		goto SAVE_BOOKMARK;
	}
	
	fprintf(fp,"%s\n",APP_STRING);
	fprintf(fp,"vcode=%d\n",APP_VER_CODE);
	fprintf(fp,"save_conf=%d\n\n",m_conf_file_flag);

	// last state
	fprintf(fp,"%s\n",LAST_STATE_STRING);
	
	if(getEditInt(IDC_EDIT_WIDTH,val) < 0 || val <= 0)
		val = 352;
	fprintf(fp,"width=%d\n",val);
	if(getEditInt(IDC_EDIT_HEIGHT,val) < 0 || val <= 0)
		val = 288;
	fprintf(fp,"height=%d\n",val);

	CComboBox *pBox;
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
	val = pBox->GetCurSel();
	if(val < 0)
		val = 0;
	fprintf(fp,"resolution=%d\n",val);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	val = pBox->GetCurSel();
	if(val < 0)
		val = 0;
	fprintf(fp,"format=%d\n",val);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SHOWYUV);
	val = pBox->GetCurSel();
	if(val < 0)
		val = 0;
	fprintf(fp,"show=%d\n",val);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FIELD);
	val = pBox->GetCurSel();
	if(val < 0)
		val = 0;
	fprintf(fp,"field=%d\n",val);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FPS);
	pBox->GetWindowText(m_buf);
	if(m_buf.IsEmpty() || sscanf(m_buf,"%d",&val) != 1 || val <= 0)
		val = 25;
	fprintf(fp,"fps=%d\n",val);
	
	val = isChecked(IDC_CHECK_LOOP);
	fprintf(fp,"loop=%d\n",val);

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_GRIDXN);
	val = pBox->GetCurSel();
	if(val < 0)
		val = 0;
	fprintf(fp,"grid=%d\n",val);
	
	{
	CRect rect1, rect2;
	GetWindowRect(&rect1);
	CWnd *pWnd = GetDlgItem(IDC_BUTTON_MORE);
	pWnd->GetWindowRect(&rect2);
	val = !(rect1.bottom > rect2.bottom + 20);
	}
	fprintf(fp, "more=%d\n",val);
	fprintf(fp, "ext_pannel=%d\n",isChecked(IDC_CHECK_EXT));

	{
		CRect rect;
		GetWindowRect(&rect);
		fprintf(fp, "pos=(%d,%d)\n",rect.left, rect.top);
	}
	
	if(ini_level < 2)
		goto leave;

	// resolution presets
	fprintf(fp,"\n%s\n",RESOLUTIONS_STRING);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
	num = pBox->GetCount();
	for(i = 0;i < num;i++){
		pBox->GetLBText(i,m_buf);
		fprintf(fp,"resolution%d=%s\n",i+1,m_buf);
	}
	
	fprintf(fp,"\n%s\n", FILTERS_STRING);
	if(isChecked(IDC_CHECK_FILTER)){
		fprintf(fp,"on\n");
		i = 0;
		while(m_ppFilters[i]){
			if(m_ppFilters[i]->flag)
				fprintf(fp,"filter%d=\"%s\"\n",i+1, m_ppFilters[i]->pItem->getName());
			i++;
		}
	}
	else
		fprintf(fp,"off\n");


	if(ini_level < 3)
		goto leave;

	if(fp){
		fclose(fp);
		fp = NULL;
	}

SAVE_BOOKMARK:
	// 书签
	fileName = m_exePath + BOOKMARK_FILE;
	if(strcmp(BOOKMARK_FILE,CONF_FILE) == 0)
		fp = fopen(fileName,"a");
	else
		fp = fopen(fileName,"w");
	if(fp == NULL){
		//m_buf.Format("打开书签文件\"%s\"失败",BOOKMARK_FILE);
		return -2;
	}

	// bookmark
	fprintf(fp,"\n%s\n",FAVOR_STRING);
	num = m_pMenu->GetMenuItemCount();

	cnt = 1;
	for(i = 4;i < num;i++){
		id = m_pMenu->GetMenuItemID(i);
		if(m_pFavorList->getItemName(id,str) > 0){
			fprintf(fp,"dir%d=%s\n",cnt++,(LPCTSTR)str);
		}
	}
	
	if(ini_level < 4)
		goto leave;

	// recent files
	fprintf(fp,"\n%s\n",RECENT_STRING);
	num = m_pRecentMenu->GetMenuItemCount();
	
	cnt = 1;
	for(i = 2;i < num;i++){
		id = m_pRecentMenu->GetMenuItemID(i);
		if(m_pRecentList->getItemName(id,str) > 0){
			fprintf(fp,"file%d=%s\n",cnt++,(LPCTSTR)str);
		}
	}
	
leave:
	if(fp)
		fclose(fp);

	return 1;
}

void  CYuvplayDlg::initMenu(){
	m_pRecentMenu = new CMenu;
	m_pMenu = new CMenu;

	// recent file list menu
	m_pRecentMenu->CreatePopupMenu();
	m_pRecentMenu->AppendMenu(MF_STRING, IDM_RECENT_NEW, "打开新文件");
	m_pRecentMenu->AppendMenu(MF_SEPARATOR);

	// main menu
	m_pMenu->CreatePopupMenu();
	m_pMenu->AppendMenu(MF_POPUP,(int)m_pRecentMenu->m_hMenu,"最近打开文件");
	m_pMenu->AppendMenu(MF_STRING, IDM_BK_ADD, "添加到收藏夹");
	m_pMenu->AppendMenu(MF_STRING, IDM_BK_MAN, "管理收藏夹");
	m_pMenu->AppendMenu(MF_SEPARATOR);


	m_pRecentList = new MenuList;
	m_pRecentList->create(m_recent_size,IDM_RECENT_OFFSET);

	m_pFavorList = new MenuList;
	m_pFavorList->create(m_favor_size, IDM_FAVOR_OFFSET);


	//
	FILE *fp = NULL;
	char lineBuf[LINESZ];
	int n;
	char *p;
	int id;
	int flag;
	CString fileName = m_exePath + BOOKMARK_FILE;

	fp = fopen(fileName,"r");
	if(fp == NULL)
		return;

	// recent files
	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, RECENT_STRING,strlen(RECENT_STRING)))
			break;
	}
	
	if(p != NULL){
		while(1){
			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;
			
			if(strncmp(p,"file",4)){
				if(*p == '\n')
					continue;
				break;
			}

			n = strlen(p);
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符

			p = strchr(lineBuf,'=');
			if(p == NULL)
				continue;
			
			flag = 0;
			while(*++p){
				if(*p != ' ' && *p != '\t'){
					flag = 1;				
					break;
				}
			}

			if(flag == 0)
				continue;

			if(m_pRecentList->getItemId(p,id) > 0)
				continue;

			m_pRecentList->appendItem(p,id);

			m_pRecentMenu->AppendMenu(MF_STRING, id, p);
			
		}
	}

	// bookmark
	fseek(fp,0,SEEK_SET);
	while(1){
		p = fgets(lineBuf, LINESZ, fp);
		if(p == NULL)
			break;
		if(!strncmp(lineBuf, FAVOR_STRING,strlen(FAVOR_STRING)))
			break;
	}

	if(p != NULL){
		while(1){
			char *p1;

			p = fgets(lineBuf, LINESZ, fp);
			if(p == NULL)
				break;

			if(strncmp(p,"dir",3)){
				if(*p == '\n')
					continue;
				break;
			}

			n = strlen(p);
			if(p[n-1] != '\n'){
				if(!feof(fp))
					continue;
			}
			else
				p[n-1] = 0;  // 去掉行尾换行符
			
			p1 = p + n-2;
			while(1){
				if(*p1 != '\\' && *p1 != '/')
					break;
				*p1-- = 0;
			}

			p = strchr(lineBuf,'=');
			if(p == NULL)
				continue;

			flag = 0;
			while(*++p){
				if(*p != ' ' && *p != '\t'){
					flag = 1;				
					break;
				}
			}

			if(flag == 0)
				continue;

			if(m_pFavorList->getItemId(p,id) > 0)
				continue;

			m_pFavorList->appendItem(p,id);

			m_pMenu->AppendMenu(MF_STRING, id, p);

		}
	}


	if(fp != NULL){
		fclose(fp);
	}
}

void CYuvplayDlg::OnBkAdd() 
{
	// TODO: Add your command handler code here
	if(m_bReady == 0){
		printMsg("请先打开文件, 然后可以将文件所在目录添加到收藏夹中");
		return;
	}

	int pos = m_fileName.ReverseFind('\\');
	CString str = m_fileName.Left(pos);
	
	int id;

	if(m_pFavorList->getItemId(str,id) > 0){
		m_buf.Format("目录\"%s\"已经在收藏夹中",(LPCTSTR)str);
		printMsg(m_buf);
		return;
	}

	if(m_pFavorList->isFull()){
		printMsg("收藏夹已满");
		return;
	}
	
	m_pFavorList->appendItem(str,id);

	m_pMenu->AppendMenu(MF_STRING, id, (LPCTSTR)str);
	
	m_buf.Format("目录\"%s\"已添加到收藏夹",(LPCTSTR)str);
	printMsg(m_buf);
}

void CYuvplayDlg::OnBkMan()
{
	// TODO: Add your command handler code here
	m_buf.Format("管理收藏夹, 暂未实现\r\n"
		"可手动修改程序所在目录下的文件\"%s\"(文本文件)",
		BOOKMARK_FILE);
	printMsg(m_buf); 
}

void CYuvplayDlg::OnRecent( int id) 
{
	// TODO: Add your command handler code here
	if(m_bReady){
		m_bReady = 0;
		closeFile();
	}

	CString  str;
	m_pRecentList->getItemName(id, str);
	int ret = openFile(str, 1, 0);
	if(ret > 0){
		m_pRecentMenu->DeleteMenu(id, MF_BYCOMMAND);
		m_pRecentMenu->InsertMenu(2,MF_STRING|MF_BYPOSITION,id, (LPCTSTR)str);
	}
	else if(ret < 0){
		m_pRecentMenu->DeleteMenu(id, MF_BYCOMMAND);
		m_pRecentList->deleteItem(id);
	}
}

void CYuvplayDlg::OnFavorite( int id) 
{
	// TODO: Add your command handler code here
	CString  str;
	m_pFavorList->getItemName(id,str);
	openFile(str,0);
}

BOOL CYuvplayDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCode == 0){
		int n;

		if(nID == IDM_BK_ADD){
			OnBkAdd();
			return 1;
		}
		else if(nID == IDM_BK_MAN){
			OnBkMan();
			return 1;
		}
		else if(nID >= IDM_RECENT_OFFSET && nID < IDM_RECENT_OFFSET + m_recent_size){
			OnRecent(nID);
			return 1;
		}
		else if(nID >= IDM_FAVOR_OFFSET && nID < IDM_FAVOR_OFFSET + m_favor_size){
			OnFavorite(nID);
			return 1;
		}
		else if(nID == IDM_RECENT_NEW){
			PostMessage(WM_COMMAND,((WPARAM)BN_CLICKED)<<8|(WPARAM)IDC_BUTTON_OPEN,0L);
			return 1;
		}
	}
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CYuvplayDlg::OnButtonQuit() 
{
	// TODO: Add your control notification handler code here
	m_conf_file_flag = 0;

	DestroyWindow();
	PostQuitMessage(0);
}

void CYuvplayDlg::OnCheckExt() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_EXT);
	CRect rect;
	CRect rect2;

	GetWindowRect(&rect);

	if(pBtn->GetCheck() == BST_CHECKED){
		rect.bottom = rect.top + m_fullSize.cy;
		MoveWindow(&rect);
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd){
			pWnd->GetWindowRect(&rect2);
			if(rect2.bottom < rect.bottom)
				pWnd->ShowWindow(SW_SHOW);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
	}
	else{
		pBtn->GetWindowRect(&rect2);
		rect.bottom = rect2.bottom + 6;
		MoveWindow(&rect);
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd){
			pWnd->GetWindowRect(&rect2);
			if(rect2.top > rect.bottom)
				pWnd->ShowWindow(SW_HIDE);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
	}
}

void CYuvplayDlg::OnButtonFormatInfo() 
{
	// TODO: Add your control notification handler code here
	int index = getComBoxSel(IDC_COMBO_FORMAT);
	if(index < 0){
		int num;
		if(m_ppFormaters){
			for(num = 0;;num++){
				if(m_ppFormaters[num] == NULL)
					break;
			}
		}
		if(m_ppFormaters == NULL || num <= 0){
			printMsg("没有可用的格式插件, 将无法浏览yuv文件");
			return;
		}
		else{
			CString msg;
			msg.Format("共%d个可用插件",num);
			printMsg(msg);
		}
	}
	else{
		unsigned int data;
		getComBoxData(IDC_COMBO_FORMAT, index, &data);
		
		CDlgFormater dlg;
		dlg.m_pFormater = m_ppFormaters[data];
		dlg.DoModal();
	}
}

void CYuvplayDlg::OnCheckFilter() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = getButton(IDC_CHECK_FILTER);
	enableWnd(IDC_BUTTON_FILTER_CFG, pBtn->GetCheck() == BST_CHECKED);

	if(m_ppFilters == NULL){
		CString buf1;
		CString path;
		
		GetModuleFileName(NULL, buf1.GetBuffer(MAX_PATH), MAX_PATH);
		buf1.ReleaseBuffer();
		int pos = buf1.ReverseFind('\\');
		path = buf1.Left(pos+1);
		gbl_fielter_nam.init(path);
		m_ppFilters = gbl_fielter_nam.getList();

		int i;
		for(i = 0;;i++){
			if(m_ppFilters[i] == NULL)
				break;
		}
		i++;

		m_ppFilters2 = (IImage_Filter**)malloc(i*sizeof(IImage_Filter*));
		m_ppFilters2[0] = NULL;
	}
}

void CYuvplayDlg::OnButtonFilterCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgFilters dlg;
	dlg.m_ppFilters = m_ppFilters;
	dlg.m_ppFilters2 = m_ppFilters2;

	if(dlg.DoModal() == IDOK){
		;
	}
	
	//MessageBox("即将实现\r\n\r\n     用来管理和配置图像滤镜     ");
}


void CYuvplayDlg::OnCheckConf() 
{
	// TODO: Add your control notification handler code here
	if(isChecked(IDC_CHECK_CONF))
		m_conf_file_flag = 1;
	else{
		m_conf_file_flag = 0;
		saveConfigFile(1);
	}
}
