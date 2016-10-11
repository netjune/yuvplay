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

// DlgMsg.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog


CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_msg = NULL;

	m_brush.CreateSolidBrush(RGB(255,255,255));
}
CDlgMsg::~CDlgMsg(){
	m_brush.DeleteObject();
}

void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialog)
	//{{AFX_MSG_MAP(CDlgMsg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg message handlers

BOOL CDlgMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_msg && m_msg[0] != 0)
		SetDlgItemText(IDC_EDIT_MSG, m_msg);
	else{
		SetDlgItemText(IDC_EDIT_MSG, "NULL");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	CWnd *pWnd2 = GetDlgItem(IDC_EDIT_MSG);
	if(pWnd->m_hWnd == pWnd2->m_hWnd){
		//
		//pDC->SetBkColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(m_textColor);
		return (HBRUSH)m_brush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
