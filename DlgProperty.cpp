//
// ����: yuvplay
//
// ����:
// ���źͷ���ԭʼYUV�ļ�, ��ʽת��, �ļ��Ƚϵ�
//
// ����:
// zhanghj(netjunegg@gmail.com)
//
// ������ʷ:
// 2007~2012: ҵ����Ŀ
//
// ��ȨЭ��: GNU GPL v2
//
//

// DLGProperty.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DLGProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLGProperty dialog


DLGProperty::DLGProperty(CWnd* pParent /*=NULL*/)
	: CDialog(DLGProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLGProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_name = NULL;
	m_pInfo = NULL;
	m_path = NULL;
}


void DLGProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLGProperty)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGProperty, CDialog)
	//{{AFX_MSG_MAP(DLGProperty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLGProperty message handlers

BOOL DLGProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_NAME, m_name);
	SetDlgItemText(IDC_EDIT_VERSION, m_pInfo->version);
	SetDlgItemText(IDC_EDIT_AUTHOR, m_pInfo->author_name);
	SetDlgItemText(IDC_EDIT_COMMENT, m_pInfo->comment);
	SetDlgItemText(IDC_EDIT_PATH, m_path);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
