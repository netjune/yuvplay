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

// CDlgFilterGain.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgFilterGain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFilterGain dialog


CDlgFilterGain::CDlgFilterGain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilterGain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilterGain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_gain1 = 1;
	m_gain2 = 1;
	m_gain3 = 1;
}


void CDlgFilterGain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilterGain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFilterGain, CDialog)
	//{{AFX_MSG_MAP(CDlgFilterGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFilterGain message handlers
BOOL CDlgFilterGain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CString buf;
	buf.Format("%f",m_gain1);
	SetDlgItemText(IDC_EDIT_VAL1, buf);
	buf.Format("%f",m_gain2);
	SetDlgItemText(IDC_EDIT_VAL2, buf);
	buf.Format("%f",m_gain3);
	SetDlgItemText(IDC_EDIT_VAL3, buf);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFilterGain::OnOK() 
{
	// TODO: Add extra validation here
	CString buf;
	float f = 0;
	GetDlgItemText(IDC_EDIT_VAL1, buf.GetBuffer(64), 64);
	buf.ReleaseBuffer();
	if(sscanf(buf, "%f", &f) != 1 || f < 0){
		AfxMessageBox("gain1 有误");
		return;
	}
	m_gain1 = f;
	GetDlgItemText(IDC_EDIT_VAL2, buf.GetBuffer(64), 64);
	buf.ReleaseBuffer();
	if(sscanf(buf, "%f", &f) != 1 || f < 0){
		AfxMessageBox("gain2 有误");
		return;
	}
	m_gain2 = f;
	GetDlgItemText(IDC_EDIT_VAL3, buf.GetBuffer(64), 64);
	buf.ReleaseBuffer();
	if(sscanf(buf, "%f", &f) != 1 || f < 0){
		AfxMessageBox("gain3 有误");
		return;
	}
	m_gain3 = f;
	CDialog::OnOK();
}

void CDlgFilterGain::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

