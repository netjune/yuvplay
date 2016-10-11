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

// CDlgFormater.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgFormater.h"
#include "DlgSimpleCfg.h"
#include "DlgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormater dialog


CDlgFormater::CDlgFormater(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormater::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormater)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pFormater = NULL;
}


void CDlgFormater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormater)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormater, CDialog)
	//{{AFX_MSG_MAP(CDlgFormater)
	ON_BN_CLICKED(IDC_BUTTON_CFG1, OnButtonCfg1)
	ON_BN_CLICKED(IDC_BUTTON_CFG2, OnButtonCfg2)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormater message handlers

BOOL CDlgFormater::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pFormater){
		SetDlgItemText(IDC_EDIT_NAME, m_pFormater->pItem->getName());
		Module_Info *pInfo = new Module_Info;
		memset(pInfo, 0, sizeof(Module_Info));
		if(pInfo == NULL){
			AfxMessageBox("申请内存失败");
			return TRUE;
		}
		m_pFormater->pItem->getModuleInfo(pInfo);
		SetDlgItemText(IDC_EDIT_VERSION, pInfo->version);
		SetDlgItemText(IDC_EDIT_AUTHOR, pInfo->author_name);
		SetDlgItemText(IDC_EDIT_COMMENT, pInfo->comment);
		SetDlgItemText(IDC_EDIT_PATH, m_pFormater->path);
		delete pInfo;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFormater::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDlgFormater::OnButtonCfg1() 
{
	// TODO: Add your control notification handler code here
	CDlgSimpleCfg dlg;
	if(dlg.DoModal() == IDOK){
		const char *str;
		if(dlg.m_str.IsEmpty())
			str = NULL;
		else
			str = dlg.m_str;
		m_pFormater->pItem->simpleConfig(dlg.m_val, dlg.m_val2, str);
	}
}

void CDlgFormater::OnButtonCfg2() 
{
	// TODO: Add your control notification handler code here
	if(m_pFormater->pItem->selfConfig() == 0){
		AfxMessageBox("本插件不支持此配置");
	}
}

void CDlgFormater::OnButtonQuery() 
{
	// TODO: Add your control notification handler code here
	char *buf = (char*)malloc(1025);
	if(buf == NULL){
		AfxMessageBox("申请内存失败");
		return;
	}
	memset(buf, 0,1025);
	m_pFormater->pItem->queryRuntimeInfo(buf, 1024);
	if(buf[0] != 0){
		CDlgMsg dlg;
		dlg.m_msg = buf;
		dlg.DoModal();
	}
	else{
		AfxMessageBox("没有信息");
	}
	
	free(buf);
}
