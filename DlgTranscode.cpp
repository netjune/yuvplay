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

// DlgTranscode.cpp : implementation file
//
//     YUV FOURCC:
//		"GREY":  Y8
//		"UYVY":
//		"YUYV"/"YUY2":
//
//		"YU16":
//		"YV12":
//		"I420"/"IYUV":
//		


#include "stdafx.h"
#include "yuvplay.h"
#include "DlgTranscode.h"
#include "common.h"
#include "Message.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgTranscode dialog


#define HELP_TEXT "注意: 若在主程序中打开了滤镜, 则数据保存之前会被滤镜处理一遍"


DlgTranscode::DlgTranscode(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTranscode::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgTranscode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pYuvDoc = NULL;
	m_pRender = NULL;
	m_ppFormaters = NULL;

	m_pArg = NULL;
	m_thread = NULL;

	m_font.CreatePointFont(200,"Tahoma");

}
DlgTranscode::~DlgTranscode(){
	if(m_pArg != NULL)
		delete m_pArg;

	m_font.DeleteObject();
}

void DlgTranscode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTranscode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgTranscode, CDialog)
	//{{AFX_MSG_MAP(DlgTranscode)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_TRANS, OnButtonTrans)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_MESSAGE(WM_TRANSCODE,OnMyThread)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnSelchangeComboFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
LRESULT DlgTranscode::OnMyThread(WPARAM wParam,LPARAM lParam){
	if(wParam  == 1){
		m_buf.Format("%d/%d",m_pArg->frameNow,m_pArg->frameNum);
		SetDlgItemText(IDC_STATIC_INFO,m_buf);
		return 1;
	}
	else if(wParam == 2){
		AfxMessageBox("转换完毕");
		SetDlgItemText(IDC_BUTTON_TRANS,"开始转换");
		m_thread = NULL;
	}
	else{
		if(m_pArg->errMsg.IsEmpty() == 0)
			AfxMessageBox(m_pArg->errMsg);
		SetDlgItemText(IDC_BUTTON_TRANS,"开始转换");
		m_thread = NULL;
	}

	return 1;
}


/////////////////////////////////////////////////////////////////////////////
// DlgTranscode message handlers
BOOL DlgTranscode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pBox;

	if(m_ppFormaters){
		pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		for(int i = 0;;i++){
			if(m_ppFormaters[i] == NULL)
				break;
			const char *name = m_ppFormaters[i]->pItem->getName();
			int index = pBox->AddString(name);
			if(index >= 0)
				pBox->SetItemData(index, i);
		}
	}

	int frameNum = m_pYuvDoc->getFrameNum();

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	pBox->SetCurSel(0);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_START);
	pBox->SetCurSel(0);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FRAMENUM);
	m_buf.Format("%d",frameNum);
	pBox->SetWindowText(m_buf);
	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SKIP_FRAME);
	pBox->SetCurSel(0);

	CWnd *pW = GetDlgItem(IDC_STATIC_INFO);
	m_buf.Format("0/%d",frameNum);
	pW->SetFont(&m_font);
	pW->SetWindowText(m_buf);
	
	m_pYuvDoc->getFileName(m_fileName);
	int pos1,pos2;
	pos1 = m_fileName.ReverseFind('\\');
	pos2 = m_fileName.ReverseFind('.');
	CString str;;
	CString ext;
	if(pos1 < 0 || pos2 < pos1){
		str = m_fileName;
		ext = ".yuv";
	}
	else{
		str = m_fileName.Left(pos2);
		ext = m_fileName.Right(m_fileName.GetLength() - pos2);
	}
	str += "_new";
	str += ext;
	m_fileName = str;
	SetDlgItemText(IDC_EDIT_FILE, str);

	GotoDlgCtrl(GetDlgItem(IDC_COMBO_FORMAT));
	
	if(m_pRender){
		RawImage_Info info;
		m_pRender->getRawInfo(&info);
		CString str;

		str.Format("%s, %dx%d",getRawFormatName(info.raw_format), info.width, info.height);
		SetDlgItemText(IDC_STATIC_SRC_INFO, str);
	}

	SetDlgItemText(IDC_STATIC_HELP, HELP_TEXT);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgTranscode::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFile(FALSE);
	CString buf;
	char *filefilter = "all files(*.*)""\0""*""\0"
					   "dat file(*.dat)""\0""*.dat""\0"
					   "avi file(*.avi)""\0""*.avi""\0"
					   "yuv file(*.yuv)""\0""*.yuv""\0\0";
	
	dlgFile.m_ofn.lpstrFile = buf.GetBuffer(MAX_PATH);
	dlgFile.m_ofn.nMaxFile = MAX_PATH;
	dlgFile.m_ofn.lpstrTitle = "保存到文件";
	dlgFile.m_ofn.nFilterIndex = 4;
	dlgFile.m_ofn.lpstrFilter = (LPCTSTR)filefilter;
	dlgFile.m_ofn.lpstrInitialDir = m_fileName;
	
	GetDlgItemText(IDC_COMBO_FORMAT,m_buf);
	int pos = m_buf.Find('(');
	if(pos > 0){
		CString buf2;
		buf2 = m_buf.Left(pos);
		m_buf = buf2;
	}
	if(m_buf == "AVI")
		dlgFile.m_ofn.nFilterIndex = 3;
	else
		dlgFile.m_ofn.nFilterIndex = 4;

	INT_PTR nRet = dlgFile.DoModal();
	if(nRet == 1){
		m_fileName = dlgFile.GetPathName();

		SetDlgItemText(IDC_EDIT_FILE, m_fileName);
	}
}


void DlgTranscode::OnButtonTrans() 
{
	// TODO: Add your control notification handler code here
	if(m_thread != NULL){
		m_pArg->cmd = 0;
		return;
	}

	if(m_pYuvDoc == NULL || (!m_pYuvDoc->isValid())){
		AfxMessageBox("没有转换源");
		return;
	}
	
	if(m_pYuvDoc == NULL || m_pRender == NULL || m_ppFormaters == NULL){
		AfxMessageBox("OOPS, 0x10010");
		return;
	}

	CString buf;
	CString fileName;
	CString errMsg;
	int startFrame = 0;
	int frameNum = 0;
	int skipNum = 0;
	int n;
	int format_id;
	CComboBox *pBox;

	GetDlgItemText(IDC_EDIT_FILE,fileName);
	if(fileName.IsEmpty()){
		errMsg = "请选择文件名";
		goto leave;
	}

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	format_id = pBox->GetCurSel();
	if(format_id < 0){
		errMsg = "请选择正确的格式";
		goto leave;
	}


	GetDlgItemText(IDC_COMBO_START,buf);
	if(!buf.IsEmpty()){
		if(myGetIntFromStr(buf,&startFrame) < 0 || startFrame < 0){
			errMsg = "起始帧值错误";
			goto leave;
		}
	}

	GetDlgItemText(IDC_COMBO_FRAMENUM,buf);
	if(!buf.IsEmpty()){
		if(myGetIntFromStr(buf,&frameNum) < 0 || frameNum < 0){
			errMsg = "帧数目值有误";
			goto leave;
		}
	}
	GetDlgItemText(IDC_COMBO_SKIP_FRAME,buf);
	if(!buf.IsEmpty()){
		if(myGetIntFromStr(buf,&skipNum) < 0 || skipNum < 0){
			errMsg = "跳帧值有误";
			goto leave;
		}
	}
	
	n = m_pYuvDoc->getFrameNum();

	if(startFrame > n){
		errMsg = "起始帧超过帧总数";
		goto leave;
	}
	n -= startFrame;
	if(n <= 0){
		errMsg = "实际转换帧数小于1,无法转换";
		goto leave;
	}

	if(skipNum > 0){
		n = (n - 1)/skipNum + 1;
	}
	if(frameNum > n || frameNum == 0)
		frameNum = n;
	
	if(m_pArg == NULL)
		m_pArg = new TranscodeArg;

	m_pArg->cmd = 1;
	m_pArg->fileName = fileName;
	m_pArg->frameNum = frameNum;
	m_pArg->skipNum = skipNum;
	m_pArg->startFrame = startFrame;
	m_pArg->m_hParent = m_hWnd;
	m_pArg->pDoc = m_pYuvDoc;
	m_pArg->pRender = m_pRender;
	m_pArg->pFormater = NULL;

	if(format_id == 0){
		m_thread = AfxBeginThread(thread_transcode_raw,m_pArg);
		if(m_thread == NULL){
			errMsg = "创建线程失败";
			goto leave;
		}
	}
	else if(format_id == 1){
		m_thread = AfxBeginThread(thread_transcode_avi,m_pArg);
		if(m_thread == NULL){
			errMsg = "创建线程失败";
			goto leave;
		}
	}
	else{
		int index = pBox->GetItemData(format_id);
		m_pArg->pFormater = m_ppFormaters[index]->pItem;
		m_thread = AfxBeginThread(thread_transcode,m_pArg);
		if(m_thread == NULL){
			errMsg = "创建线程失败";
			goto leave;
		}
	}

	SetDlgItemText(IDC_BUTTON_TRANS,"取消");
	
	return;

leave:
	if(!errMsg.IsEmpty())
		AfxMessageBox(errMsg);
}

void DlgTranscode::OnButtonQuit() 
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE,0,0);
}


void DlgTranscode::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_thread != NULL){
		m_pArg->cmd = 0;
		while(m_pArg->state != 0)
			Sleep(10);
	}

	CDialog::OnClose();
}

HBRUSH DlgTranscode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void DlgTranscode::OnSelchangeComboFormat() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CString str2;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	int index = pBox->GetCurSel();
	pBox->GetLBText(index,str);

	GetDlgItemText(IDC_EDIT_FILE, str2);
	str2.ReleaseBuffer();
	if(str2.IsEmpty())
		str2 = m_fileName;

	CString str3;
	if(strncmp((const char*)str, "AVI", 3) == 0){
		str3 = str2.Right(4);
		if(str3.Compare(".avi") == 0)
			return;

		int pos =  str2.ReverseFind('.');
		int pos2 = str2.ReverseFind('\\');
		if(pos < 0 || pos < pos2)
			str3 = str2;
		else{
			str3 = str2.Left(pos);
		}
		//str3 += "_";
		//str3 += str;
		str3 += ".avi";
	}
	else{
		int pos =  str2.ReverseFind('.');
		int pos2 = str2.ReverseFind('\\');
		CString ext;

		if(pos < 0 || pos < pos2){
			str3 = str2;
			ext = ".yuv";
		}
		else{
			str3 = str2.Left(pos);
			ext = str2.Right(str2.GetLength() - pos);
			if(ext.Compare(".avi") != 0)
				return;
			ext = ".yuv";
		}
		
		//str3 += "_";
		//str3 += str;
		str3 += ext;
	}

	SetDlgItemText(IDC_EDIT_FILE, str3);
}
