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

// ValuesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "ValuesDlg.h"
#include "Message.h"
#include "DlgValues2Set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ValuesDlg dialog


ValuesDlg::ValuesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ValuesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ValuesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_adjustRect = 1;

	m_font.CreatePointFont(90,"Courier New");
	
	srand(time(0)+rand());
	unsigned char a[3];
	int b;
	int i;
	b = rand()%3;
	if(b == 0){
		for(i = 0;i < 3;i++){
			a[i] = rand()%256;
		}
	}
	else if(b == 1){
		for(i = 0;i < 3;i++){
			a[i] = 255;
		}
	}
	else{
		for(i = 0;i < 3;i++){
			a[i] = 0;
		}
	}
	b = a[0] + a[1] + a[2];
	b /= 3;
	unsigned char c[3];
	if(b > 128){
		for(i = 0;i < 3;i++){
			if(a[i] < 196) a[i] = 196;
			c[i] = rand()%100;
		}
	}
	else{
		for(i = 0;i < 3;i++){
			if(a[i] > 64) a[i] = 64;
			c[i] = rand()%100 + 155;
		}
	}
	m_bgColor = RGB(a[0],a[1],a[2]);
	m_brush.CreateSolidBrush(m_bgColor);
	//m_textColor = RGB(255-a[0],255-a[1],255-a[2]);
	m_textColor = RGB(c[0], c[1],c[2]);

	m_hParent = NULL;

	m_valueMode = VALUE_MODE_HEX;
	m_YUV = 7;
}

ValuesDlg::~ValuesDlg(){
	m_font.DeleteObject();

}

void ValuesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ValuesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ValuesDlg, CDialog)
	//{{AFX_MSG_MAP(ValuesDlg)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_HEX, OnRadioHex)
	ON_BN_CLICKED(IDC_RADIO_DEC, OnRadioDec)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
int  ValuesDlg::enableWnd(int id, int enable){
	CWnd *pW = GetDlgItem(id);
	
	if(pW == NULL)
		return -1;
	
	pW->EnableWindow(enable);
	return 1;
}
int  ValuesDlg::enableCheck(int id,int check){
	CButton *pB;
	
	pB = (CButton*)GetDlgItem(id);
	if(pB != NULL){
		pB->SetCheck(check ? BST_CHECKED : BST_UNCHECKED);
		return 1;
	}return -1;
}

int  ValuesDlg::isChecked(int id){
	CButton *pB;
	
	pB = (CButton*)GetDlgItem(id);
	if(pB == NULL)
		return 0;
	return (pB->GetCheck() == BST_CHECKED);
}
////

int  ValuesDlg::getStringSize(LPCTSTR str,int length,int &width,int &height){
	SIZE size;
	CDC *pDc;

	pDc = GetDC();
	if(pDc == NULL)
		goto leave;

	HFONT  hFont;
	hFont = (HFONT)pDc->SelectObject((HFONT)m_font);
	if(GetTextExtentPoint32(pDc->m_hDC,str,length,&size) == 0)
		goto leave;

	width = size.cx;
	height = size.cy;

	return 1;
leave:
	if(pDc != NULL){
		pDc->SelectObject(hFont);
		ReleaseDC(pDc);
	}
	return -1;
}

// 返回指定行的起点和长度
//  不存在的行都返回负值
// "\r\n"
int  ValuesDlg::getStringLine(CString& str,int lineNO, int &start,int &length){
	if(str.IsEmpty() || lineNO < 0)
		return -1;
	
	int lineStart,lineEnd;
	start = 0;
	length = 0;
	lineStart = 0;
	while(1){
		lineEnd = str.Find("\r\n",lineStart);
		if(lineEnd < 0){
			if(lineNO == 0){
				lineEnd = str.GetLength();
				break;
			}
			else
				return -2;
		}
		if(lineNO == 0)
			break;
		lineNO--;
		lineStart =  lineEnd + 1;
	}

	start = lineStart;
	length = lineEnd - lineStart;

	return 1;
}

int  ValuesDlg::clearItem(){
	CRect  rect;

	//GetWindowRect(&rect);
	//rect.right = 
	//MoveWindow(0,0,0,0,0);
	ShowWindow(SW_HIDE);


	return 1;
}

int  ValuesDlg::addItem(CString &text,int nLineT,int numPixels){
	CStatic  *pS;
	CEdit  *pE;
	CRect  rect;
	CRect  rect2;
	LPCTSTR p;
	int lineStart,lineLen;
	int width,height;

	if(text.IsEmpty())
		return -1;

	pE = (CEdit*)GetDlgItem(IDC_EDIT_VALUES);

	if(nLineT < 3){
		pE->SetWindowText(text);
		return -2;
	}
	
	//
	CString  buf;
	getStringLine(text,0,lineStart,lineLen);
	buf = text.Mid(lineStart,lineLen);
	SetDlgItemText(IDC_EDIT_GRID_POS,buf);
	getStringLine(text,1,lineStart,lineLen);
	buf = text.Mid(lineStart,lineLen);
	SetDlgItemText(IDC_EDIT_START_POINT,buf);
	getStringLine(text,2,lineStart,lineLen);
	buf = text.Mid(lineStart);
	SetDlgItemText(IDC_EDIT_VALUES,buf);

	if(!m_adjustRect)
		return 1;
	
	m_adjustRect = 0;
	if(nLineT >= 3){
		getStringLine(text,3,lineStart,lineLen);
	}
	else
		getStringLine(text,0,lineStart,lineLen);
	p = text;
	p += lineStart;

	if(getStringSize(p,lineLen,width,height) < 0)
		return -2;
	width *= 1.1;
	height *= nLineT;
	//height *= 1.06;
	
	if(width < 270)
		width = 270;

	// edit window
	pE->GetWindowRect(&rect2);
	rect2.right = rect2.left + width;
	rect2.bottom = rect2.top + height;

	// expand dlg
	GetWindowRect(&rect);
	rect.right = rect2.right + 4;
	rect.bottom = rect.top + rect2.Height() + rect2.top - rect.top + 4;

	int cxScreen,cyScreen;			
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	
	if(rect.Width() > cxScreen/2){
		int n = rect.Width() - cxScreen/2;
		rect.right -= n;
		rect2.right -= n;
	}
	if(rect.Height() > cyScreen - TASKBAR_HEIGHT){
		int n = rect.Height() - (cyScreen - TASKBAR_HEIGHT);
		rect.bottom -= n;
		rect2.bottom -= n;
	}
	if(rect.right > cxScreen){
		int n = rect.right - cxScreen;
		rect.right -= n;
		rect.left -= n;
		rect2.right -= n;
		rect2.left -= n;
	}
	
	MoveWindow(&rect,1);
	ShowWindow(SW_SHOW);
	
	ScreenToClient(&rect2);
	pE->MoveWindow(&rect2,1);

	return 1;
}

void  ValuesDlg::destroy(){
	SendMessage(WM_QUIT, 0, 0);
	DestroyWindow();
}

///////////////////////////////////////////////////////////////
// ValuesDlg message handlers

void ValuesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect1;
	CRect rect2;
	CEdit *pE;

	GetWindowRect(&rect1);
	
	pE = (CEdit*)GetDlgItem(IDC_EDIT_VALUES);
	if(pE == NULL)
		return;
	pE->GetWindowRect(&rect2);
	rect2.right = rect1.right;
	rect2.bottom = rect1.bottom;
	ScreenToClient(&rect2);
	pE->MoveWindow(&rect2,1);
}

void ValuesDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	clearItem();

	//CDialog::OnClose();
}

HBRUSH ValuesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);;

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_VALUES){
		//pDC->SetBkColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_textColor);

		return (HBRUSH)m_brush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL ValuesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit *pE = (CEdit*)GetDlgItem(IDC_EDIT_VALUES);
	pE->SetFont(&m_font);

	if(m_valueMode == VALUE_MODE_HEX){
		enableCheck(IDC_RADIO_HEX, 1);
		enableCheck(IDC_RADIO_DEC, 0);
	}
	else{
		enableCheck(IDC_RADIO_HEX, 0);
		enableCheck(IDC_RADIO_DEC, 1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ValuesDlg::OnRadioHex() 
{
	// TODO: Add your control notification handler code here
	if(m_valueMode == VALUE_MODE_HEX)
		return;
	m_valueMode = VALUE_MODE_HEX;
	m_adjustRect = 1;
	::PostMessage(m_hParent,WM_VALUEDLG,0,0);
}

void ValuesDlg::OnRadioDec() 
{
	// TODO: Add your control notification handler code here
	if(m_valueMode == VALUE_MODE_DEC)
		return;
	m_valueMode = VALUE_MODE_DEC;
	m_adjustRect = 1;
	::PostMessage(m_hParent,WM_VALUEDLG,0,0);
}

void ValuesDlg::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	CDlgValues2Set dlg(this);
	dlg.m_YUV = m_YUV;
	if(dlg.DoModal() == IDOK){
		int n = m_YUV;
		m_YUV = dlg.m_YUV;
		if(n != m_YUV){
			m_adjustRect = 1;
			::PostMessage(m_hParent,WM_VALUEDLG,1,0);
		}
	}
}
