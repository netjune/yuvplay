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

// DlgFilter_Y8.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgFilter_Y8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFilter_Y8 dialog


CDlgFilter_Y8::CDlgFilter_Y8(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilter_Y8::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilter_Y8)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFilter_Y8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilter_Y8)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFilter_Y8, CDialog)
	//{{AFX_MSG_MAP(CDlgFilter_Y8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFilter_Y8 message handlers

void CDlgFilter_Y8::OnOK() 
{
	// TODO: Add extra validation here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_Y8_EMU);
	m_bEmu = (pBtn->GetCheck() == BST_CHECKED);
	CDialog::OnOK();
}

BOOL CDlgFilter_Y8::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_Y8_EMU);
	pBtn->SetCheck(m_bEmu ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
