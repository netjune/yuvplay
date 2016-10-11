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

// DlgValues2Set.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgValues2Set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgValues2Set dialog


CDlgValues2Set::CDlgValues2Set(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgValues2Set::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValues2Set)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_YUV = 0;
}


void CDlgValues2Set::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgValues2Set)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgValues2Set, CDialog)
	//{{AFX_MSG_MAP(CDlgValues2Set)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int  CDlgValues2Set::enableWnd(int id, int enable){
	CWnd *pW = GetDlgItem(id);
	
	if(pW == NULL)
		return -1;
	
	pW->EnableWindow(enable);
	return 1;
}
int  CDlgValues2Set::enableCheck(int id,int check){
	CButton *pB;
	
	pB = (CButton*)GetDlgItem(id);
	if(pB != NULL){
		pB->SetCheck(check ? BST_CHECKED : BST_UNCHECKED);
		return 1;
	}return -1;
}

int  CDlgValues2Set::isChecked(int id){
	CButton *pB;
	
	pB = (CButton*)GetDlgItem(id);
	if(pB == NULL)
		return 0;
	return (pB->GetCheck() == BST_CHECKED);
}
////
/////////////////////////////////////////////////////////////////////////////
// CDlgValues2Set message handlers

BOOL CDlgValues2Set::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	enableCheck(IDC_CHECK_Y,m_YUV&1);
	enableCheck(IDC_CHECK_U,m_YUV&2);
	enableCheck(IDC_CHECK_V,m_YUV&4);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgValues2Set::OnOK() 
{
	// TODO: Add extra validation here
	if(isChecked(IDC_CHECK_Y))
		m_YUV |= 1;
	else
		m_YUV &= ~1;
	if(isChecked(IDC_CHECK_U))
		m_YUV |= 2;
	else
		m_YUV &= ~2;
	if(isChecked(IDC_CHECK_V))
		m_YUV |= 4;
	else
		m_YUV &= ~4;

	CDialog::OnOK();
}
