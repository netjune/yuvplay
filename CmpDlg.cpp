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

// CmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "CmpDlg.h"
#include "Message.h"
#include "common.h"
#include "DlgFormater.h"
#include "DlgFilters.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CmpDlg dialog


#define HELP_TEXT "提示: 可以比较不同格式文件, 可以比较同一文件或不同文件的两场, 可以对同一文件应用滤镜和未应用滤镜的效果进行比较"



CmpDlg::CmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CmpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lockSync = 1;		// 默认
	m_bPlaying = 0;

	m_bReady = 0;
	m_bEnd = 0;

	m_threshold = 0;

	m_imageWnd2.m_dbg = 1;

	m_ppFilters = NULL;
	m_ppFormaters = NULL;
	m_ppFilters2 = NULL;

	m_bFormatSynced = 0;
	m_formatPeer.raw_format = RAW_FORMAT_UNKNOWN;
	m_formatLocal.raw_format = RAW_FORMAT_UNKNOWN;
	m_pRender = NULL;

	m_cmpComp = 0;

	m_bGridOn = 0;
	m_gridXNum = 0;
	m_gridYNum = 0;
}


void CmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CmpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CmpDlg, CDialog)
	//{{AFX_MSG_MAP(CmpDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnOpenFile)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_MESSAGE(WM_COMPARE,OnMyMsg)
	ON_MESSAGE(WM_IMAGEWND, OnImageWnd)
	ON_BN_CLICKED(IDC_CHECK_LOCKSYNC, OnCheckLockSync)
	ON_WM_MOVE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_RADIO_BI, OnRadioBi)
	ON_BN_CLICKED(IDC_RADIO_DIFF, OnRadioDiff)
	ON_BN_CLICKED(IDC_BUTTON_SAVE2, OnButtonSave2)
	ON_BN_CLICKED(IDC_BUTTON_FORMAT_INFO, OnButtonFormatInfo)
	ON_BN_CLICKED(IDC_CHECK_FILTER, OnCheckFilter)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_CFG, OnButtonFilterCfg)
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_COMBO_CMP_COMP, OnSelchangeComboCmpComp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmpDlg message handlers
void CmpDlg::printMsg(LPCTSTR msg){
	if(msg != NULL)
		AfxMessageBox(msg);
}

LRESULT CmpDlg::OnMyMsg(WPARAM wParam,LPARAM lParam){
	if(m_bFormatSynced == 0)
		return 0;

	int id = wParam;
	if(id == CMP_NEXT_FRAME){
		if(m_lockSync)
			OnButtonNext();
		else
			updateDiff();
	}
	else if(id == CMP_PREV_FRAME){
		if(m_lockSync)
			OnButtonPrev();
		else
			updateDiff();

	}
	else if(id == CMP_STOP){
		if(m_lockSync)
			OnButtonStop();
		else
			updateDiff();
	}
	else if(id == CMP_SEEK){
		int n = lParam;
		
		if(n < 0){
			n = -n;
			if(n > m_yuvDoc.getFrameNum())
				n = m_yuvDoc.getFrameNum() - 1;
		}
		else{
			if(n > m_yuvDoc.getFrameNum())
				n = m_yuvDoc.getFrameNum() - 2;
		}
		
		//m_yuvDoc.seekFrame(n,SEEK_SET);
	}
	else if(id == CMP_UPDATE){
		displayFrame();
	}
	else{
		;
	}
	
	return 1;
}

LRESULT CmpDlg::OnImageWnd(WPARAM wParam,LPARAM lParam){
	HWND  hWnd = (HWND)wParam;
	
	if(hWnd == m_imageWnd.m_hWnd){
		if(m_bReady)
			OnButtonStop();
		if(m_bReady)
			OnButtonStop();
	}
	
	return 1;
}

/////////////////////

int CmpDlg::getEditInt(int id,int &val){
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

int CmpDlg::setEditInt(int id,int val){
	CString buf;
	CEdit *pEdit;

	pEdit = (CEdit*)GetDlgItem(id);
	if(pEdit == NULL)
		return -1;
	buf.Format("%d",val);
	pEdit->SetWindowText(buf);

	return 1;
}

int CmpDlg::getComBoxStr(int id,CString &str){
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

int CmpDlg::addComBoxStr(int id,LPCTSTR str){
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
	return 1;
}

int CmpDlg::delComBoxStr(int id, LPCTSTR str){
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

int CmpDlg::setComBoxSel(int id,int nIndex){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	if(pBox == NULL)
		return -1;

	pBox->SetCurSel(nIndex);
	return 1;
}

int CmpDlg::setComBoxSel(int id,LPCTSTR str){
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


int CmpDlg::getComBoxSel(int id){
	CComboBox *pBox = (CComboBox*)GetDlgItem(id);
	if(pBox)
		return pBox->GetCurSel();
	return -1;
}

int CmpDlg::addComBoxItem(int id,LPCTSTR str, unsigned int data){
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
int CmpDlg::setComBoxData(int id, int index, unsigned int data){
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
int CmpDlg::getComBoxData(int id, int index, unsigned int *pData){
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


CButton  *CmpDlg::getButton(int id){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	return pB;
}

CComboBox *CmpDlg::getComBox(int id){
	CComboBox *pBox;

	pBox = (CComboBox*)GetDlgItem(id);
	return pBox;
}

void  CmpDlg::enableWnd(int id,int enable){
	CWnd  *pWnd = GetDlgItem(id);
	if(pWnd)
		pWnd->EnableWindow(enable);
}

void  CmpDlg::enableCheck(int id,int check){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	if(pB != NULL)
		pB->SetCheck(check ? BST_CHECKED : BST_UNCHECKED);
}

int  CmpDlg::isChecked(int id){
	CButton *pB;

	pB = (CButton*)GetDlgItem(id);
	if(pB == NULL)
		return 0;
	return (pB->GetCheck() == BST_CHECKED);
}


////
//
//  图像参数通信
//  level: 
//     0: 常规更新
//     1: 试探更新
//
int CmpDlg::updateConf1(int level){
	int val1,val2;
	CString err_msg;
	CString buf;
	CButton *pBtn;
	int n;
	CString  buf2;
	long pos;
	int index;

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
		m_yuvDoc.setParams(&params);
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
			return 0;
		}

		m_render.setShowYUV(m_showYUV);
		
		if(m_render.update() < 0){
			printMsg(m_render.getErrMsg());
			return 0;
		}

		// can't fail
		m_render.getRawInfo(&m_formatLocal);
		if(m_formatLocal.raw_format != m_formatPeer.raw_format
			|| m_formatLocal.width != m_formatPeer.width
			|| m_formatLocal.height != m_formatPeer.height){
			printMsg("两边格式不同, 无法比较");
			m_bFormatSynced = 0;
			clear(1);
			goto leave;
		}

		m_bFormatSynced = 1;
		info = m_formatLocal;
		info.raw_format = RAW_FORMAT_GREY8;
		m_render2.setParams(&info);
		m_render2.update();
	}
	
	m_yuvDoc.setFileName(m_fileName);
	n = m_yuvDoc.update();  	// -1: 失败,0:无变化,1:更新
	if( n  < 0){
		setState(STATE_NOVIDEO);		// 无图像
		goto leave;
	}
	else if(n == 0){  //
		return 0;
	}
	else{		//
		int n;
		m_bReady = 1;
		n = displayFrame();
		if( n >= 0){
			if(n > 0)
				setState(STATE_VIDEOSTART);
			m_imageWnd.setTitle(m_fileName);
			m_imageWnd.ShowWindow(SW_SHOW);
			m_imageWnd2.ShowWindow(SW_SHOW);
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

int CmpDlg::updateConf2(int level){
	if(m_imageWnd.m_gridColor != m_gridColor){
		m_imageWnd.m_gridColor = m_gridColor;
	}
	m_imageWnd.enableGrid(m_bGridOn,m_gridXNum,m_gridYNum);
	m_imageWnd2.enableGrid(m_bGridOn,m_gridXNum,m_gridYNum);
	m_render.setShowYUV(m_showYUV);
	
	if(m_notifyId && m_bReady)
		displayFrame();

	return 1;
}

int  CmpDlg::setState(int state){

	return 1;
}

void  CmpDlg::notifyView(int id){
	::PostMessage(m_imageWnd.m_hWnd,WM_VIEW,m_notifyId,0);
	if(m_notifyId>0){
		m_notifyId = 0;
	}
}

int CmpDlg::displayFrame(){
	if(m_bReady == 0)
		return 0;

	int framePos,frameNum;
	int ret;
	
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
	m_render.getBuffer(&pBuf, &bufSz);
	m_yuvDoc.getFrame(pBuf, bufSz);
	m_render.putData(pBuf);

	notifyView();

	updateDiff();

	return 1;
}

int  CmpDlg::updateDiff(){
	if(m_bReady <= 0)
		return 0;

	CString buf;
	int n;
	const unsigned char *p1, *p2;
	unsigned char *p3;
	int len1, len2;
	int bufsz;

	len1 = GetDlgItemInt(IDC_EDIT_THRESHOLD,&n,1);
	if(n == 0){
		MessageBox("错误的阈值");
		return -1;
	}

	m_threshold = len1;

	p1 = m_render.getRaw(&len1);
	p2 = m_pRender->getRaw(&len2);


	m_render2.getBuffer(&p3, &bufsz);
	if(m_cmpMode == CMP_MODE_BI){
		if(m_cmpComp == 0)
			n = m_render2.yuvDiff_bi_all(&m_render, p1, p2, m_threshold);
		else
			n = m_render2.yuvDiff_bi_comp(&m_render, p1, p2, m_threshold, m_cmpComp-1);
	}
	else{
		if(m_cmpComp == 0)
			n = m_render2.yuvDiff_diff_all(&m_render, p1, p2);
		else
			n = m_render2.yuvDiff_diff_comp(&m_render, p1, p2, m_cmpComp-1);
	}
	m_render2.putData(p3);
	
	if( n > 0){
		RawImage_Info info;
		m_render2.getRawInfo(&info);
		double db = info.width*info.height;
		float f = n/db*100;
		buf.Format("差异像素数: %d 个(占%.2f%%)",n,f);
	}
	else{
		buf = "相同: ^_^";
	}
	m_imageWnd2.setTitle(buf);

	::PostMessage(m_imageWnd2.m_hWnd,WM_VIEW,m_notifyId2,0);
	if(m_notifyId2>0){
		m_notifyId2 = 0;
	}

	return 1;
}

// if level == 0, only hide
void  CmpDlg::clear(int level){
	m_imageWnd.clear();
	m_imageWnd2.clear();
	//m_fileName.Empty();
	
	if(level >= 1){
		setState(STATE_NOVIDEO);
		
		m_yuvDoc.destroy();

		CSliderCtrl  *pSB;
		pSB = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_PLAY);
		pSB->SetRange(0,0,1);
		CStatic *pS;
		pS = (CStatic *)GetDlgItem(IDC_STATIC_FRAMENUM);
		pS->SetWindowText("0/0");

		m_bReady = 0;
	}

	if(level >= 2){
		ShowWindow(SW_HIDE);
		::PostMessage(m_hParent,WM_COMPARE,1,0);
	}
}

int  CmpDlg::show(){
	ShowWindow(SW_SHOW);
	if(m_bReady){
		m_imageWnd.ShowWindow(SW_SHOW);
		m_imageWnd2.ShowWindow(SW_SHOW);
	}

	return 1;
}

int  CmpDlg::imageEnd(int pos){

	return 1;
}

int CmpDlg::setLoop(int bLoop){
	m_bLoop = bLoop;
	return 1;
}

int CmpDlg::setStartPath(LPCTSTR path){
	m_path = path;
	
	return 1;
}


void  CmpDlg::destroy(){
	if(m_imageWnd.m_hWnd != NULL)
		m_imageWnd.destroy();

	if(m_imageWnd2.m_hWnd != NULL)
		m_imageWnd2.destroy();

	//SendMessage(WM_QUIT, 0, 0);
	CDialog::OnClose();
}

int CmpDlg::setCmpFormat(RawImage_Info *pInfo){
	if(pInfo == NULL)
		return -1;

	// assume pInfo is valid, and don't check it
	m_formatPeer = *pInfo;

	if(m_formatLocal.raw_format == RAW_FORMAT_UNKNOWN){ 
		// local format has not been updated, we will do later check
		return 1;
	}
	
	if(m_formatLocal.raw_format == pInfo->raw_format
		&& m_formatLocal.width == pInfo->width
		&& m_formatLocal.height == pInfo->height)
		return 1;
	
	if(m_bReady)
		clear(1);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
void CmpDlg::OnDropFiles(HDROP hDropInfo)
{
	CString fns;
	CString buffer;
	int fileNum;
	int i;

	if(m_bPlaying > 0)
		return;

	fileNum = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	if(1 && fileNum > 0)
		fileNum = 1;
	for(i = 0;i < fileNum;i++){
		int n;
		n = DragQueryFile(hDropInfo,i,buffer.GetBuffer(MAX_PATH),MAX_PATH);
		buffer.ReleaseBuffer();
		if(i > 0)
			fns += "," + buffer;
		else
			fns = buffer;
	}
	::DragFinish(hDropInfo);

	if(m_bReady){
		m_bReady = 0;
		clear(1);
	}
	
	m_fileName = fns;
	updateConf1(0);
}

void CmpDlg::OnOpenFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFile(TRUE);
	CString buffer;
	char *filefilter = "all files(*.*)""\0""*""\0"
					   "dat file(*.dat)""\0""*.dat""\0"
					   "yuv file(*.yuv)""\0""*.yuv""\0\0";

	dlgFile.m_ofn.lpstrFile = buffer.GetBuffer(MAX_PATH);
	dlgFile.m_ofn.nMaxFile = MAX_PATH;
	dlgFile.m_ofn.lpstrTitle = "打开比较文件";
	dlgFile.m_ofn.nFilterIndex = 3;
	dlgFile.m_ofn.lpstrFilter = (LPCTSTR)filefilter;
	dlgFile.m_ofn.lpstrInitialDir = m_path;

	INT_PTR nRet = dlgFile.DoModal();
	if(nRet == 1){
		int n;
		CString fileName = dlgFile.GetPathName();

		if(m_bReady){
			m_bReady = 0;
			clear(1);
		}

		m_fileName = fileName;
		n = updateConf1(0);		// 强制更新
	}
} 

void CmpDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	clear(2);

	//CDialog::OnClose();
}

BOOL CmpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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


	// TODO: Add extra initialization here
	m_imageWnd.Create(this,1,"YUV viewer 2");
	m_imageWnd.ShowWindow(SW_HIDE);
	m_imageWnd.setGridColor(m_gridColor);
	m_imageWnd.m_pRender = &m_render;
	m_imageWnd.m_hParent = m_hWnd;
	
	m_imageWnd2.Create(this,2,"YUV diff window");
	m_imageWnd2.ShowWindow(SW_HIDE);
	m_imageWnd2.setGridColor(m_gridColor);
	m_imageWnd2.m_pRender = &m_render2;
	m_imageWnd2.m_hParent = m_hWnd;
	m_imageWnd2.m_gridColor = RGB(32,255,32);

	int cxScreen,cyScreen;
	CRect rect;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	m_imageWnd.GetWindowRect(&rect);
	rect.left += cxScreen/3;
	rect.right += cxScreen/3;
	m_imageWnd.MoveWindow(&rect,0);
	m_imageWnd2.GetWindowRect(&rect);
	rect.left += (cxScreen/3)*2;
	rect.right += (cxScreen/3)*2;
	m_imageWnd2.MoveWindow(&rect,0);

	enableCheck(IDC_CHECK_LOCKSYNC,1);

	SetDlgItemText(IDC_EDIT_THRESHOLD,"0");

	OnRadioDiff();
	
	SetDlgItemInt(IDC_EDIT_WIDTH, m_width);
	SetDlgItemInt(IDC_EDIT_HEIGHT, m_height);
	setComBoxSel(IDC_COMBO_FORMAT, m_format_id);
	setComBoxSel(IDC_COMBO_FIELD, m_field_id);
	enableWnd(IDC_BUTTON_FILTER_CFG, 0);
	//enableWnd(IDC_COMBO_CMP_COMP, 0);
	setComBoxSel(IDC_COMBO_CMP_COMP, m_cmpComp);

	SetDlgItemText(IDC_STATIC_HELP,  HELP_TEXT);

	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_OPEN));

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CmpDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	if(m_bReady <= 0)
		return;

	int n;
	int num = m_yuvDoc.getFrameNum();
	
	n = m_yuvDoc.getFrameIndex();
	if(n <= 1)
		return;
	
	if(n == num){
		setState(STATE_VIDEOMID);
	}
	
	n = m_yuvDoc.seekFrameBackward(1);
	if(n > 0)
		displayFrame();
	else{
		setState(STATE_VIDEOSTART);
	}
}

void CmpDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(m_bReady <= 0)
		return;

	if(m_bPlaying <= 0){
		if(m_yuvDoc.getFrameIndex() == 1){
			setState(STATE_VIDEOMID);
		}
	}

	if(m_yuvDoc.seekFrameForward(1) > 0)
		displayFrame();
	else{
		updateDiff();
	}
}

void CmpDlg::OnCheckLockSync() 
{
	// TODO: Add your control notification handler code here
	CButton  *pBtn = (CButton*)GetDlgItem(IDC_CHECK_LOCKSYNC);
	if(pBtn->GetCheck() == BST_CHECKED)
		m_lockSync = 1;
	else
		m_lockSync = 0;
}


void CmpDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

void CmpDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int n;
	int nNow;
	CSliderCtrl *pSC;

	if(m_bReady == 0)
		goto leave;

	pSC = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_PLAY);
	n = pSC->GetPos();
	nNow = m_yuvDoc.getFrameIndex();
	if(n == nNow)
		goto leave;
	if(n > nNow){
		if(nNow == 1)
			setState(STATE_VIDEOMID);
		//m_yuvDoc.seekFrame(n-1,SEEK_SET);
		OnButtonNext();
	}
	else{
		if(nNow == m_yuvDoc.getFrameNum())
			setState(STATE_VIDEOMID);
		//m_yuvDoc.seekFrame(n+1,SEEK_SET);
		OnButtonPrev();
	}

leave:	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CmpDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	m_bPlaying = 0;
	if(!m_bReady)
		return;

	if(m_yuvDoc.getFrameIndex() == 1 ){
		clear(1);	
		m_bReady = 0;
		return;
	}

	// 显示第一帧
	if(m_yuvDoc.restart() >= 0){
		int n = m_yuvDoc.getFrameIndex();
		if(n == 1){
			setState(STATE_VIDEOSTART);
		}
		else if(n < 0){
			m_bReady = 0;
			return;
		}
			
		displayFrame();
	}
}

void CmpDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if(m_bReady == 0 || m_render.getRaw(0) == NULL){
		printMsg("没有图像");
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
	dlgFile.m_ofn.lpstrTitle = "保存比较图像";
	dlgFile.m_ofn.nFilterIndex = 4;
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
		len1 = sizeof(*pBmpInfo);
		len2 = pBmpInfo->biSizeImage;
		if(pBmpInfo == NULL || pBmpBits == NULL){
			errMsg = "获取图像数据失败";
			goto leave;
		}
		
		if(ext.CompareNoCase("bmp") == 0){
			memset(&bfh,0,sizeof(bfh));
			bfh.bfType = 'B' + ('M'<<8);
			bfh.bfSize = sizeof(bfh) + len1 + len2;
			bfh.bfOffBits = sizeof(bfh) + len1;

			outFile.Write(&bfh,sizeof(bfh));
		}

		outFile.Write(pBmpInfo,len1);
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

void CmpDlg::OnButtonSave2() 
{
	// TODO: Add your control notification handler code here
	if(m_bReady == 0 || m_render2.getRaw(0) == NULL){
		printMsg("没有图像");
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
	dlgFile.m_ofn.lpstrTitle = "保存差别图像";
	dlgFile.m_ofn.nFilterIndex = 4;
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
		
		m_render2.getImageBmp(&pBmpInfo, &pBmpBits);
		len1 = sizeof(*pBmpInfo);
		len2 = pBmpInfo->biSizeImage;
		if(pBmpInfo == NULL || pBmpBits == NULL){
			errMsg = "获取图像数据失败";
			goto leave;
		}
		
		if(ext.CompareNoCase("bmp") == 0){
			memset(&bfh,0,sizeof(bfh));
			bfh.bfType = 'B' + ('M'<<8);
			bfh.bfSize = sizeof(bfh) + len1 + len2;
			bfh.bfOffBits = sizeof(bfh) + len1;
			
			outFile.Write(&bfh,sizeof(bfh));
		}
		
		outFile.Write(pBmpInfo,len1);
		outFile.Write(pBmpBits,len2);
	}
	else{
		const unsigned char *pYuv;
		int len;
		
		pYuv = m_render2.getRaw(&len);
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


void CmpDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	updateConf1();

	CEdit *pE = (CEdit*)GetDlgItem(IDC_EDIT_THRESHOLD);
	CString  buf;
	int val;

	pE->GetWindowText(buf.GetBuffer(32),32);
	buf.ReleaseBuffer();
	if(buf.IsEmpty())
		val = 0;
	else
		myGetIntFromStr(buf,&val);

	if(val != m_threshold){
		m_threshold = val;
		//updateDiff();
	}
}

void CmpDlg::OnRadioBi() 
{
	// TODO: Add your control notification handler code here
	int en = isChecked(IDC_RADIO_BI);
	m_cmpMode = en ? CMP_MODE_BI : CMP_MODE_DIFF;

	enableCheck(IDC_RADIO_DIFF,!en);
	
	enableWnd(IDC_EDIT_THRESHOLD,en);

	updateDiff();
}

void CmpDlg::OnRadioDiff() 
{
	// TODO: Add your control notification handler code here
	int en = isChecked(IDC_RADIO_DIFF);
	m_cmpMode = (!en) ? CMP_MODE_BI : CMP_MODE_DIFF;
	
	enableCheck(IDC_RADIO_BI,!en);
	enableWnd(IDC_EDIT_THRESHOLD,!en);

	updateDiff();
}


void CmpDlg::OnButtonFormatInfo() 
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

void CmpDlg::OnCheckFilter() 
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

void CmpDlg::OnButtonFilterCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgFilters dlg;
	dlg.m_ppFilters = m_ppFilters;
	dlg.m_ppFilters2 = m_ppFilters2;
	
	if(dlg.DoModal() == IDOK){
		;
	}
}


void CmpDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnOpenFile();
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CmpDlg::OnSelchangeComboCmpComp() 
{
	// TODO: Add your control notification handler code here
	m_cmpComp = getComBoxSel(IDC_COMBO_CMP_COMP);
	updateDiff();
}

BOOL CmpDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN){
		int nChar = pMsg->wParam;
		if(nChar == VK_RETURN){
			updateDiff();
			return 1;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
