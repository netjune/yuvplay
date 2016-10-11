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

// DlgSimpleCfg.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgSimpleCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSimpleCfg dialog


CDlgSimpleCfg::CDlgSimpleCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSimpleCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSimpleCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_val = 0;
	m_val2 = 0;
}


void CDlgSimpleCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSimpleCfg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSimpleCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgSimpleCfg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSimpleCfg message handlers

BOOL CDlgSimpleCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemInt(IDC_EDIT_VAL, m_val);
	SetDlgItemInt(IDC_EDIT_VAL2, m_val2);
	if(!m_str.IsEmpty())
		SetDlgItemText(IDC_EDIT_STR, m_str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSimpleCfg::OnOK() 
{
	// TODO: Add extra validation here
	int ret;
	GetDlgItemText(IDC_EDIT_VAL, m_str.GetBuffer(513),512);
	m_str.ReleaseBuffer();
	if(!m_str.IsEmpty()){
		if(m_str[0] == '0' && (m_str[1] == 'x' || m_str[1] == 'X')){
			const char *p = m_str;
			p += 2;
			ret = sscanf(p, "%X",&m_val);
		}
		else{
			m_val = GetDlgItemInt(IDC_EDIT_VAL,&ret,0);
		}
		if(ret != 1){
			AfxMessageBox("整形参数有错");
			return;
		}
	}
	else{
		m_val = 0;
	}

	GetDlgItemText(IDC_EDIT_VAL2, m_str.GetBuffer(513),512);
	m_str.ReleaseBuffer();
	if(!m_str.IsEmpty()){
		if(m_str[0] == '0' && (m_str[1] == 'x' || m_str[1] == 'X')){
			const char *p = m_str;
			p += 2;
			ret = sscanf(p, "%X",&m_val2);
		}
		else{
			m_val2 = GetDlgItemInt(IDC_EDIT_VAL2,&ret,1);
		}
		if(ret != 1){
			AfxMessageBox("整形参数有错");
			return;
		}
	}
	else{
		m_val2 = 0;
	}

	GetDlgItemText(IDC_EDIT_STR, m_str.GetBuffer(513),512);
	m_str.ReleaseBuffer();

	CDialog::OnOK();
}

void CDlgSimpleCfg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
