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

// DlgPixel.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgPixel.h"
#include "Render.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPixel dialog


CDlgPixel::CDlgPixel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPixel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPixel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bHexMode = 0;
	
	for(int i = 0;i < 9;i++)
		m_pBrushes[i] = new CBrush;
}

CDlgPixel::~CDlgPixel(){
	for(int i = 0;i < 9;i++){
		if(m_pBrushes[i]){
			m_pBrushes[i]->DeleteObject();
			delete m_pBrushes[i];
		}
	}

	m_font.DeleteObject();
}


void CDlgPixel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPixel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPixel, CDialog)
	//{{AFX_MSG_MAP(CDlgPixel)
	ON_MESSAGE(WM_PIXEL_VAL, OnMsgPixelVal)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_HEX, OnCheckHex)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void  CDlgPixel::destroy(){
	SendMessage(WM_QUIT, 0, 0);
	DestroyWindow();
}

int CDlgPixel::update(){
	OnMsgPixelVal(0,0);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// CDlgPixel message handlers
LRESULT CDlgPixel::OnMsgPixelVal(WPARAM wParam,LPARAM lParam){
	PixelVal *pv = &m_pixVal;
	
	m_str.Format("(%d,%d)", pv->x,pv->y);
	SetWindowText(m_str);

	/*
	if(m_bHexMode){
		m_str.Format(
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n"
			"\r\n"
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n"
			"\r\n"
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n"
			"%2X   %2X   %2X\r\n",
			pv->Y[0][0], pv->Y[0][1], pv->Y[0][2], 
			pv->U[0][0], pv->U[0][1], pv->U[0][2],
			pv->V[0][0], pv->V[0][1], pv->V[0][2],
			pv->Y[1][0], pv->Y[1][1], pv->Y[1][2], 
			pv->U[1][0], pv->U[1][1], pv->U[1][2],
			pv->V[1][0], pv->V[1][1], pv->V[1][2],
			pv->Y[2][0], pv->Y[2][1], pv->Y[2][2], 
			pv->U[2][0], pv->U[2][1], pv->U[2][2],
			pv->V[2][0], pv->V[2][1], pv->V[2][2]);
	}
	else{
		m_str.Format(
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n"
			"\r\n"
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n"
			"\r\n"
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n"
			"%3d   %3d   %3d\r\n",
			pv->Y[0][0], pv->Y[0][1], pv->Y[0][2], 
			pv->U[0][0], pv->U[0][1], pv->U[0][2],
			pv->V[0][0], pv->V[0][1], pv->V[0][2],
			pv->Y[1][0], pv->Y[1][1], pv->Y[1][2], 
			pv->U[1][0], pv->U[1][1], pv->U[1][2],
			pv->V[1][0], pv->V[1][1], pv->V[1][2],
			pv->Y[2][0], pv->Y[2][1], pv->Y[2][2], 
			pv->U[2][0], pv->U[2][1], pv->U[2][2],
			pv->V[2][0], pv->V[2][1], pv->V[2][2]);
	}
*/

	int i,j;
	int id = IDC_STATIC_YUV0;
	for(j = 0;j < 3;j++){
		for(i = 0;i < 3;i++){
			if(m_bHexMode == 0){
				m_str.Format("%3d\r\n%3d\r\n%3d",
					m_pixVal.Y[j][i],m_pixVal.U[j][i],m_pixVal.V[j][i]);
			}
			else{
				m_str.Format("%2X\r\n%2X\r\n%2X",
					m_pixVal.Y[j][i],m_pixVal.U[j][i],m_pixVal.V[j][i]);
			}
			SetDlgItemText(id++, m_str);
		}
	}
	
	
	return 1;
}

BOOL CDlgPixel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFont *pF = GetDlgItem(IDC_STATIC_YUV0)->GetFont();
	LOGFONT lf;
	pF->GetLogFont(&lf);

	lf.lfWeight = 700;
	m_font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_YUV4)->SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPixel::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags&MK_LBUTTON)
	{
		POINT pt;
		pt.x=point.x;pt.y=point.y;
		ClientToScreen(&pt);
		SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPixel::OnCheckHex() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_HEX);
	m_bHexMode = (pBtn->GetCheck() == BST_CHECKED);
	OnMsgPixelVal(0,0);
}



HBRUSH CDlgPixel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int id = pWnd->GetDlgCtrlID();
	if(id >= IDC_STATIC_YUV0 && id <= IDC_STATIC_YUV8){
		register unsigned char y,u,v;
		register unsigned char r,g,b;

		id -= IDC_STATIC_YUV0;
		y = m_pixVal.Y[id/3][id%3];
		u = m_pixVal.U[id/3][id%3];
		v = m_pixVal.V[id/3][id%3];
		
		if(m_pixVal.format == 0)  // yuv
			yuv2rgb(y,u,v,&r,&g,&b);
		else if(m_pixVal.format == 2){  // grey8
			r = y;
			g = y;
			b = y;
		}
		else if(m_pixVal.format == 3){
			if(y || u || v)
				r = 255;
			else
				r = 0;
			g = b = r;
		}
		else{  // rgb
			r = y;
			g = u;
			b = v;
		}
		
		if(r+g+b < 355){
			pDC->SetTextColor(RGB(255,255,255));
		}
		else{
			pDC->SetTextColor(RGB(0,0,0));
		}
		pDC->SetBkMode(TRANSPARENT);
		m_pBrushes[id]->DeleteObject();
		m_pBrushes[id]->CreateSolidBrush(RGB(r,g,b));
		return (HBRUSH)*m_pBrushes[id];
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
