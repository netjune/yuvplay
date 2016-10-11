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

// ImageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "ImageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Message.h"


#define  IDM_ONTOP              0x0300
#define  IDM_ORIGIN_SIZE        0x0400
#define  IDM_SHOW_TITLE         0x0500
#define  IDM_SWITCH_SHOWMODE    0x0600
#define  IDM_HELP               0x0700

#define IMAGE_MOVE_STEP_HORZ   20
#define IMAGE_MOVE_STEP_VERT   20


#define HELP_MSG   "快捷键:\r\n------\r\n" \
	"f: 切换显示模式(适应屏幕或原始大小)\r\n" \
	"wasd: 上下左右移动,相当于方向键\r\n" \
	"o或0: 适应屏幕模式下,调整窗口大小,使图像尽量恢复到原始分辨率\r\n" \
	"x: 窗口调整到最大以显示最大图像\r\n" \
	"g: 网格\r\n" \
	"q: 主窗口提前\r\n" \
	"\r\n鼠标:\r\n------\r\n" \
	"点鼠标右键, 可将主窗口切换到前台\r\n"  \
	"鼠标接近下边缘的时候,或按住shift键的时候,鼠标滚轮用于左右滚动,其他情况用于上下滚动  \r\n"  \
	"另: 按住ctrl键滚动速度加倍\r\n" \
	"\r\n其他\r\n------\r\n"  \
	"主窗口的帧序号输入框, 数据显示窗口的起点坐标输入框和像素数目输入框, 三者输入后按回车生效   \r\n"  \
	"在主窗口空白区域(靠中下部)双击鼠标左键, 显示打开文件对话框\r\n"  \
	"\r\n意见和问题反馈: netjunegg@gmail.com\r\n"


/////////////////////////////////////////////////////////////////////////////
// ImageWnd

ImageWnd::ImageWnd()
{
	m_pBmpInfo = NULL;
	m_pBmpBits = NULL;
	
	m_bGridOn = 0;
	m_gridColor = RGB(255,255,255);
	m_gridXNum = 1;
	m_gridYNum = 1;
	
	m_pValuesDlg = NULL;
	m_pRender = NULL;
	
	m_myLife1 = m_myLife2 = 0;
	
	m_pPosWnd = NULL;

	m_rszFlag = 0;

	m_hBitmap = NULL;
	m_hMemDC = NULL;

	m_paintFlag = 0;

	m_pValues2Dlg = NULL;

	m_bGridFlag = 0;

	m_oldPt.x = 0;
	m_oldPt.y = 0;
}

ImageWnd::~ImageWnd()
{
	if(m_pValuesDlg)
		delete m_pValuesDlg;
	if(m_pPosWnd){
		delete m_pPosWnd;
	}
	if(m_pValues2Dlg)
		delete m_pValues2Dlg;
	if(m_hMemDC)
		::ReleaseDC(m_hWnd, m_hMemDC);
	if(m_hBitmap)
		::DeleteObject(m_hBitmap);
}


BEGIN_MESSAGE_MAP(ImageWnd, CWnd)
//{{AFX_MSG_MAP(ImageWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_VIEW,OnMyMsg)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOMMAND()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_VALUEDLG,OnMsgValueWnd)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_MESSAGE(WM_VALUE2DLG,OnMsgValue2Wnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ImageWnd message handlers
void ImageWnd::printMsg(LPCTSTR msg){
	if(msg && strlen(msg) > 0)
		AfxMessageBox(msg);
}

//   0: 刷新图像
//   1: 重算大小
LRESULT ImageWnd::OnMyMsg(WPARAM wParam,LPARAM lParam){
	preDisplayImage();
	Invalidate(0);

	if(m_pValuesDlg){
		if(m_pValuesDlg->IsWindowVisible())
			updateValues(m_oldPt);
	}
	if(m_pValues2Dlg){
		if(m_pValues2Dlg->IsWindowVisible()){
			CPoint pt(m_pValues2Dlg->m_x, m_pValues2Dlg->m_y);
			updateValues2(pt, 1);
		}
	}
	
	return 1;
}

LRESULT ImageWnd::OnMsgValueWnd(WPARAM wParam,LPARAM lParam){
	updateValues(m_oldPt);

	return 1;
}

LRESULT ImageWnd::OnMsgValue2Wnd(WPARAM wParam,LPARAM lParam){
	if(wParam == 0){
		// mode change
	}
	else if(wParam == 1){
		// yuv change
	}
	
	CPoint pt(m_pValues2Dlg->m_x, m_pValues2Dlg->m_y);
	updateValues2(pt, 1);
	
	return 1;
}


void ImageWnd::drawGrid(CDC *pDc,CRect &rect){
	if(rect.IsRectEmpty())
		return;
	if(m_gridXNum > m_rect1.width || m_gridYNum > m_rect1.height)
		return;
	CPen pen;
	CPen *oldPen;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = m_gridColor;
	pen.CreatePen(PS_SOLID |PS_GEOMETRIC|PS_ENDCAP_ROUND, 1, &logBrush);
	oldPen = pDc->SelectObject(&pen);
	
	int x,y,cx,cy,step,extra;
	x = rect.left;
	y = rect.top;
	cx = rect.Width();
	cy = rect.Height();
	
	if(m_gridXNum > 0){
		y = rect.top;
		step = m_gridXNum;
		extra = 0;
		x += step;		// added on 2008/02/03
		while(x < cx){
			pDc->MoveTo(x,y);
			pDc->LineTo(x,y+cy);
			x += step;
			if(extra-- > 0)
				x += 1;
		}
	}
	if(m_gridYNum > 0){
		x = rect.left;
		step = m_gridYNum;
		extra = 0;
		y += step;
		while(y < cy){
			pDc->MoveTo(x,y);
			pDc->LineTo(x+cx,y);
			y += step;
			if(extra-- > 0)
				y += 1;
		}
	}
	
	pDc->SelectObject(oldPen);
	pen.DeleteObject();
}

int ImageWnd::enableGrid(int enable,int xNum,int yNum){
	if(enable == 0){
		m_bGridOn = 0;
		m_bGridFlag = 0;
		if(m_pValuesDlg)
			m_pValuesDlg->ShowWindow(SW_HIDE);
		if(m_pPosWnd)
			m_pPosWnd->show(0);
	}
	else{
		if(m_rszFlag == 0 && m_pBmpInfo &&
			(m_rect1.width != m_pBmpInfo->biWidth 
			|| m_rect1.height != m_pBmpInfo->biHeight) ){
			printMsg("请先调整到原始大小, 或者切换为原始大小模式");
			return -2;
		}
		
		if(m_pValues2Dlg)
			m_pValues2Dlg->ShowWindow(SW_HIDE);

		if(xNum > 0)
			m_gridXNum = xNum;
		if(yNum > 0)
			m_gridYNum = yNum;
		m_bGridOn = 1;
		m_bGridFlag = 1;

		if(m_pValuesDlg){
			m_pValuesDlg->m_adjustRect = 1;
			m_pValuesDlg->SetWindowText(m_title);
		}

	}
	
	if(m_pBmpInfo && m_pBmpBits){
		preDisplayImage();
		Invalidate(0);
	}
	return 1;
}

int ImageWnd::getGridPos(CPoint &point,int bGrid){
	int x,y;
	int step;
	int left;
	CRect rect;
	
	x = point.x;
	y = point.y;
	if(m_rszFlag == 1){
		x += m_rect3.left;
		y += m_rect3.top;
	}
	
	x /= m_gridXNum;
	y /= m_gridYNum;
	if(!bGrid){
		x *= m_gridXNum;
		y *= m_gridYNum;
	}
	
	point.x = x;
	point.y = y;
	
	return 1;
}

int ImageWnd::setClientSize(int cx,int cy, int center){
	CRect wRect;
	CRect cRect;
	
	GetWindowRect(&wRect);
	
	GetClientRect(&cRect);
	wRect.right += cx - cRect.Width();
	wRect.bottom += cy - cRect.Height();
	
	int cxScreen,cyScreen;			
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	if(wRect.Width() > cxScreen)
		wRect.right = wRect.left + cxScreen;
	if(wRect.Height() > cyScreen - TASKBAR_HEIGHT)
		wRect.bottom = wRect.top + cyScreen - TASKBAR_HEIGHT;
	SetWindowPos(&wndTop,wRect.left,wRect.top,wRect.Width(),wRect.Height(),SWP_SHOWWINDOW);

	return 1;
}

void ImageWnd::clear(){
	m_width = m_height = 0;
	m_pBmpInfo = NULL;
	m_pBmpBits = NULL;
	m_myLife1 = m_myLife2 = 0;
	ShowWindow(SW_HIDE);
	if(m_pValuesDlg)
		m_pValuesDlg->ShowWindow(SW_HIDE);
	if(m_pValues2Dlg)
		m_pValues2Dlg->ShowWindow(SW_HIDE);
}

void ImageWnd::destroy(){
	clear();
	if(m_pValuesDlg)
		m_pValuesDlg->destroy();
	if(m_pValues2Dlg)
		m_pValues2Dlg->destroy();

	if(m_pPosWnd){
		::SendMessage(m_pPosWnd->m_hWnd, WM_QUIT,0,0);
		::DestroyWindow(m_pPosWnd->m_hWnd);
	}

	SendMessage(WM_QUIT,0,0);

	DestroyWindow();
}

//
// level > 0: 强制调整
void  ImageWnd::adjustRect(int level){
	if(m_rect1.width <= 0 || m_rect1.height <= 0 || level > 0){
		int cx = 0, cy = 0;
		if(m_pBmpInfo != NULL){
			cx = m_pBmpInfo->biWidth;
			cy = m_pBmpInfo->biHeight;
		}
		if(cx <= 0 || cy <= 0){
			//printMsg("未初始化的窗口尺寸");
			return;
		}

		setClientSize(cx,cy);
		CRect rect;
		GetClientRect(&rect);
		if((rect.Width() != cx) || (rect.Height() != cy) ){
			updateLayerout(rect.Width(),rect.Height());
			setClientSize(m_rect1.width, m_rect1.height);
			GetClientRect(&rect);
			//MessageBox("由于系统限制, 图像窗口尺寸未设置到图像原始尺寸");
		}

		updateLayerout(rect.Width(),rect.Height());
	}
}

//
// rectOnly: 不更新图像,仅更新大小
void  ImageWnd::preDisplayImage(int rectOnly){
	if(!rectOnly){
		m_pRender->getImageBmp(&m_pBmpInfo, &m_pBmpBits);
	}

	if(m_pBmpInfo == NULL)
		return;

	if(m_hMemDC == NULL){		
		m_hMemDC = CreateCompatibleDC(0);
	}

	if(m_width != m_pBmpInfo->biWidth || m_height != m_pBmpInfo->biHeight){
		m_width = m_pBmpInfo->biWidth;
		m_height = m_pBmpInfo->biHeight;

		HDC hdcWindow = ::GetDC(m_hWnd);
		HBITMAP hBmp = CreateCompatibleBitmap(hdcWindow, m_width, m_height);
		::ReleaseDC(m_hWnd, hdcWindow);

		if(m_hBitmap){
			SelectObject(m_hMemDC, hBmp);
			DeleteObject(m_hBitmap);
			m_hBitmap = hBmp;
		}
		else{
			SelectObject(m_hMemDC, hBmp);
			m_hBitmap = hBmp;
		}

		adjustRect(1);
	}

	if(m_hBitmap){
		int n;
		SetStretchBltMode(m_hMemDC, COLORONCOLOR);
		n = StretchDIBits(m_hMemDC,
			0,0,m_pBmpInfo->biWidth,m_pBmpInfo->biHeight,
			0,0,m_pBmpInfo->biWidth,m_pBmpInfo->biHeight,
			(CONST VOID*)m_pBmpBits,
			(CONST BITMAPINFO*)m_pBmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
		
		if(m_bGridOn){
			CRect  rect(0,0,m_pBmpInfo->biWidth, m_pBmpInfo->biHeight);
			CDC dc;
			dc.Attach(m_hMemDC);
			drawGrid(&dc,rect);
			dc.Detach();
		}
	}

			/*
		if(1){
			CPen pen;
			CPen *oldPen;
			LOGBRUSH logBrush;
			CDC dc;
			dc.Attach(m_hMemDC);

			m_gridColor = RGB(155,255,0);
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = m_gridColor;
			pen.CreatePen(PS_SOLID |PS_GEOMETRIC|PS_ENDCAP_ROUND, 1, &logBrush);
			oldPen = dc.SelectObject(&pen);

			
			dc.LineTo(100, 55);
			dc.TextOut(10, 10, "aaaaaa");
			dc.SelectObject(oldPen);
			dc.Detach();
			pen.DeleteObject();
			
		}
		*/
}

void  ImageWnd::displayImage(CDC *pDc){
	if(pDc == NULL){
		pDc = GetDC();
	}

	if(m_pBmpInfo != NULL && m_pBmpBits != NULL){
		int ret;

		if(m_rszFlag == 0){
			if(m_rect1.width != m_pBmpInfo->biWidth || m_rect1.height != m_pBmpInfo->biHeight){
				int n;
				SetStretchBltMode(pDc->m_hDC, COLORONCOLOR);
				n = StretchDIBits(pDc->m_hDC,
					m_rect1.left,m_rect1.top,m_rect1.width,m_rect1.height,
					0,0,m_pBmpInfo->biWidth,m_pBmpInfo->biHeight,
					(CONST VOID*)m_pBmpBits,
					(CONST BITMAPINFO*)m_pBmpInfo,
					DIB_RGB_COLORS,
					SRCCOPY);

				/*
				if(m_bGridOn){
					CRect  rect(m_rect1.left,m_rect1.top,m_rect1.width,m_rect1.height);
					drawGrid(pDc,rect);
				}
				*/
			}
			else{
				ret = BitBlt(pDc->m_hDC, m_rect1.left,m_rect1.top,
					m_rect1.width,m_rect1.height, m_hMemDC, 0,0, SRCCOPY);
			}
			ret = 1;
		}
		else{
			ret = BitBlt(pDc->m_hDC, m_rect1.left,m_rect1.top,
					m_rect1.width,m_rect1.height, m_hMemDC, m_rect3.left,m_rect3.top, SRCCOPY);
		}
	}
	else{
		CRect  rect(0,0,m_rect1.width,m_rect1.height);
		GetClientRect(&rect);
		pDc->FillRect(&rect,&CBrush(RGB(0,0,0)));
		pDc->TextOut(m_rect1.width/2-10,m_rect1.height/2,"无     图     像");
	} 

	/*
	if(m_pBmpInfo != NULL && m_pBmpBits != NULL){
		int n;
		SetStretchBltMode(pDc->m_hDC, COLORONCOLOR);
		n = StretchDIBits(pDc->m_hDC,
			0,0,m_cx/2,m_cy/2,
			0,0,m_pBmpInfo->biWidth,m_pBmpInfo->biHeight,
			(CONST VOID*)m_pBmpBits,
			(CONST BITMAPINFO*)m_pBmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
		
		if(m_bGridOn){
			CRect  rect(0,0,m_cx,m_cy);
			drawGrid(pDc,rect);
		}
	}
	else{
		CRect  rect(0,0,m_cx,m_cy);
		pDc->FillRect(&rect,&CBrush(RGB(0,0,0)));
		pDc->TextOut(m_cx/2-10,m_cy/2,"无     图     像");
	} 
*/
}

int ImageWnd::fillTheRest(CDC *pDC){
	if(pDC == NULL){
		pDC = GetDC();
	}

	int ret;
	CRect  rect, rect2;
	GetClientRect(&rect);

	rect2 = rect;
	rect2.right = rect2.left + m_rect1.left;
	if(!rect2.IsRectEmpty())
		pDC->FillRect(&rect2, &CBrush(RGB(0,0,0)));

	rect2 = rect;
	rect2.left = rect2.right - m_rect1.left;
	if(!rect2.IsRectEmpty())
		pDC->FillRect(&rect2, &CBrush(RGB(0,0,0)));

	rect2 = rect;
	rect2.bottom = rect2.top + m_rect1.top;
	if(!rect2.IsRectEmpty())
		pDC->FillRect(&rect2, &CBrush(RGB(0,0,0)));
	rect2 = rect;
	rect2.top = rect2.bottom - m_rect1.top;
	if(!rect2.IsRectEmpty())
		pDC->FillRect(&rect2, &CBrush(RGB(0,0,0)));

	return 1;
}


int  ImageWnd::setGridColor(COLORREF color){
	m_gridColor = color;
	if(m_pBmpInfo != NULL && m_pBmpBits != NULL)
		InvalidateRect(NULL);
	
	return 1;
}

int  ImageWnd::updateValues(CPoint pt, int flag){
	if(flag == 0){
		pt.x -= m_rect1.left;
		pt.y -= m_rect1.top;
		if(pt.x < 0 || pt.x > m_rect1.width || pt.y < 0 || pt.y > m_rect1.height){
			MessageBox("坐标超出图像范围");
			return 0;
		}
	}

	m_oldPt = pt;
	if(m_pValuesDlg == NULL){
		m_pValuesDlg = new ValuesDlg;
		m_pValuesDlg->m_hParent = m_hWnd;
		m_pValuesDlg->Create(IDD_DLG_VALUES,this);
		m_pValuesDlg->ShowWindow(SW_HIDE);
	}
	
	CString  buf;
	CString  buf2;
	CString  text;
	int numPixels,nLine;
	int x,y;
	int n;
	int i;
	int nLineT = 0;

	if(m_pBmpInfo == NULL || m_bGridFlag == 0)
		return -2;
	if(m_gridXNum <= 0 || m_gridYNum <= 0)
		return -3;
	
	// pixel pos 1
	CPoint pt2 = pt;
	getGridPos(pt2,0);
	x = pt2.x;
	y = pt2.y;
	
	numPixels = m_gridXNum;
	nLine = m_gridYNum;
	if(m_pBmpInfo->biWidth - x < numPixels)
		numPixels = m_pBmpInfo->biWidth - x;
	if(m_pBmpInfo->biHeight - y < nLine)
		nLine = m_pBmpInfo->biHeight - y;
	if(numPixels <= 0 || nLine <= 0)
		return - 4;

	// grid pos 2
	getGridPos(pt2,1);
	
	text.Format("(%d,%d)\r\n(%d,%d)",
		pt2.x,pt2.y,x,y);
	text += "\r\n";
	nLineT += 3;
	
	int valueMode = m_pValuesDlg->m_valueMode;
	int yuvFlag = m_pValuesDlg->m_YUV;

	// Y分量
	if(yuvFlag&1){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const  char *p1;
			
			// 返回实际字节数
			nBytes = m_pRender->getPixels_Y(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes <= 0)
				break;
			if(i == 0){
				buf2.Format("==Y(%dx%d)==\r\n",numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}

				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	// U分量
	if(yuvFlag&2){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const char *p1;
			
			// 返回实际字节数
			nBytes = m_pRender->getPixels_U(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes == 0)
				continue;
			else if(nBytes < 0)
				break;
			if(i == 0){
				buf2.Format("==U(%dx%d)==\r\n",numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}
				
				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	// V分量
	if(yuvFlag&4){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const char *p1;
			
			// 返回实际字节数
			nBytes = m_pRender->getPixels_V(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes == 0)
				continue;
			else if(nBytes < 0)
				break;
			if(i == 0){
				buf2.Format("==V(%dx%d)==\r\n",numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}
				
				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	if(nLineT > 2){
		if(m_myLife1 == 0){
			m_myLife1++;

			CRect rect1;
			CRect rect2;
			int a,b;
			int cxScreen,cyScreen;
			
			cxScreen = GetSystemMetrics(SM_CXSCREEN);
			cyScreen = GetSystemMetrics(SM_CYSCREEN);
			
			// adjust position
			GetWindowRect(&rect1);
			m_pValuesDlg->GetWindowRect(&rect2);
			a = rect2.Width();
			b = rect2.Height();
			
			if(rect1.right + a <= cxScreen && rect1.right < cxScreen){
				rect2.left = rect1.right;
				rect2.right = rect2.left + a;
				rect2.top = rect1.top;
				rect2.bottom = rect2.top + b;
			}
			else{
				rect2.right = cxScreen;
				rect2.left = rect2.right - a;
				rect2.top = rect1.top;
				rect2.bottom = rect2.top + b;
			}
			
			m_pValuesDlg->MoveWindow(&rect2,1);
		}
	}
	else{
		text = "先切换到原始大小. 方法: 双击标题栏";
		nLineT = 1;
	}
	
	m_pValuesDlg->addItem(text,nLineT,numPixels);
	m_pValuesDlg->ShowWindow(SW_SHOW);
	
	return 1;
}

int ImageWnd::transferRect(const CMyRect &rect1, const CMyRect &rect2, const CMyRect &rect3, CMyRect &rect4){
	if(m_rszFlag == 0){
		float f;
		f = rect1.width;
		f /= rect2.width;
		rect4.left = rect1.left/f;

		f = rect1.height;
		f /= rect2.height;
		rect4.top = rect1.top/f;

		if(rect4.left < 0) rect4.left = 0;
		if(rect4.left > rect2.width) rect4.left = rect2.width;
		if(rect4.top < 0) rect4.top = 0;
		if(rect4.top > rect2.height) rect4.top = rect2.height;
		if(rect4.width > rect2.width - rect4.left)
			rect4.width = rect2.width - rect4.left;
		if(rect4.height > rect2.height - rect4.top)
			rect4.height = rect2.height - rect4.top;
	}
	else{
		rect4.left = rect1.left + rect3.left;
		rect4.top = rect1.top + rect3.top;

		if(rect4.width > rect2.width - rect4.left)
			rect4.width = rect2.width - rect4.left;
		if(rect4.height > rect2.height - rect4.top)
			rect4.height = rect2.height - rect4.top;
	}
	
	return 1;
}


int  ImageWnd::updateValues2(CPoint pt, int flag){
	if(flag == 0){
		pt.x -= m_rect1.left;
		pt.y -= m_rect1.top;
		if(pt.x < 0 || pt.x > m_rect1.width || pt.y < 0 || pt.y > m_rect1.height){
			MessageBox("坐标超出图像范围");
			return 0;
		}
	}

	if(m_pValues2Dlg == NULL){
		m_pValues2Dlg = new CDlgValues2;
		m_pValues2Dlg->m_hParent = m_hWnd;
		m_pValues2Dlg->Create(IDD_DLG_VALUES2,this);
		m_pValues2Dlg->SetWindowText(m_title);
	}

	CString  buf;
	CString  buf2;
	CString  text;
	int numPixels,nLine;
	int x,y;
	int n;
	int i;
	int nLineT;
	CMyRect rect, rect2;

	rect.left = pt.x;
	rect.top = pt.y;
	rect.width = m_rect1.width;
	rect.height =  m_rect1.height;
	rect2.width = m_pValues2Dlg->m_width;
	rect2.height = m_pValues2Dlg->m_height;
	m_rect2.width = m_pBmpInfo->biWidth;
	m_rect2.height = m_pBmpInfo->biHeight;
	if(flag == 0)
		transferRect(rect, m_rect2, m_rect3, rect2);
	else{
		rect2.left = pt.x;
		rect2.top = pt.y;
		if(rect2.left < 0) rect2.left = 0;
		if(rect2.top < 0) rect2.top = 0;
		if(rect2.left >= m_rect2.width || rect2.top >= m_rect2.height){
			MessageBox("超出图像范围");
			return -1;
		}
		if(rect2.width > m_rect2.width - rect2.left) rect2.width = m_rect2.width - rect2.left;
		if(rect2.height > m_rect2.height - rect2.top) rect2.height = m_rect2.height - rect2.top;
	}

	if(m_pBmpInfo == NULL)
		return -2;

	
	numPixels = rect2.width;
	nLine = rect2.height;
	if(numPixels <= 0 || nLine <= 0)
		return - 4;
	x = rect2.left;
	y = rect2.top;
	m_pValues2Dlg->m_x = x;
	m_pValues2Dlg->m_y = y;
	if(m_pValues2Dlg->m_real_w != numPixels){
		m_pValues2Dlg->m_real_w = numPixels;
		m_pValues2Dlg->m_adjustRect = 1;
	}
	if(m_pValues2Dlg->m_real_h != nLine){
		m_pValues2Dlg->m_real_h = nLine;
		m_pValues2Dlg->m_adjustRect = 1;
	}


	int valueMode = m_pValues2Dlg->m_valueMode;
	int yuv_flag = m_pValues2Dlg->m_YUV;
	text.Empty();
	nLineT = 0;

	// Y分量
	if(yuv_flag&1){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const  char *p1;

			// 返回实际字节数
			nBytes = m_pRender->getPixels_Y(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes <= 0)
				break;
			if(i == 0){
				buf2.Format("==Y(%dx%d)==\r\n", numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}

				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	// U分量
	if(yuv_flag&2){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const char *p1;
			
			// 返回实际字节数
			nBytes = m_pRender->getPixels_U(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes == 0)
				continue;
			else if(nBytes < 0)
				break;
			if(i == 0){
				buf2.Format("==U(%dx%d)==\r\n", numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}
				
				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	// V分量
	if(yuv_flag&4){
		for(i = 0;i < nLine;i++){
			int nBytes;
			const char *p1;
			
			// 返回实际字节数
			nBytes = m_pRender->getPixels_V(x,y+i,numPixels,(unsigned char*)buf.GetBuffer(numPixels),numPixels);
			if(nBytes == 0)
				continue;
			else if(nBytes < 0)
				break;
			if(i == 0){
				buf2.Format("==V(%dx%d)==\r\n", numPixels,nLine);
				text += buf2;
				nLineT++;
			}
			
			p1 = (LPCTSTR)buf;
			while(nBytes-- > 0){
				if(valueMode == VALUE_MODE_DEC){
					buf2.Format("%3u ",*(const unsigned char*)p1);
				}
				else{
					buf2.Format("%.2X ",*(const unsigned char*)p1);
				}
				
				text += buf2;
				p1++;
			}
			text += "\r\n";
			nLineT++;
		}
	}
	
	if(yuv_flag == 0)
		text += "请选择显示分量";

	if(nLineT > 2){
		if(m_myLife2 == 0){
			m_myLife2++;

			CRect rect1;
			CRect rect2;
			int a,b;
			int cxScreen,cyScreen;
			
			cxScreen = GetSystemMetrics(SM_CXSCREEN);
			cyScreen = GetSystemMetrics(SM_CYSCREEN);
			
			// adjust position
			GetWindowRect(&rect1);
			m_pValues2Dlg->GetWindowRect(&rect2);
			a = rect2.Width();
			b = rect2.Height();
			
			if(rect1.right + a <= cxScreen && rect1.right < cxScreen){
				rect2.left = rect1.right;
				rect2.right = rect2.left + a;
				rect2.top = rect1.top;
				rect2.bottom = rect2.top + b;
			}
			else{
				rect2.right = cxScreen;
				rect2.left = rect2.right - a;
				rect2.top = rect1.top;
				rect2.bottom = rect2.top + b;
			}
			
			m_pValues2Dlg->MoveWindow(&rect2,1);
		}
	}

	
	m_pValues2Dlg->addItem(text,nLineT,numPixels);
	m_pValues2Dlg->ShowWindow(SW_SHOW);
	
	return 1;
}

void ImageWnd::setTitle(LPCTSTR title){
	m_title = title;

	CRect rect;
	GetClientRect(&rect);
	updateLayerout(rect.Width(), rect.Height());
}

int ImageWnd::enableValues2(int enable){
	if(enable){

	}

	return 1;
}


int ImageWnd::updateLayerout(int cx, int cy){
	if(m_pBmpInfo == NULL || m_pBmpBits == NULL)
		return 0;

	if(m_rszFlag == 0){
		/*if(cx == m_pBmpInfo->biWidth){  // 1
			m_rect1.width = cx;
			if(cy == m_pBmpInfo->biHeight)
				m_rect1.height = cy;
			else{
				float f1 = m_pBmpInfo->biHeight;
				f1 /= m_pBmpInfo->biWidth;

				f1 *= cx;
				m_rect1.height = f1;
				if(f1 - m_rect1.height > 0){
					m_rect1.height++;
				}
			}
		}
		else if(cy == m_pBmpInfo->biHeight){ // 2
			m_rect1.height = cy;
			float f1 = m_pBmpInfo->biWidth;
			f1 /= m_pBmpInfo->biHeight;
			
			f1 *= cy;
			m_rect1.width = f1;
			if(f1 - m_rect1.width > 0){
				m_rect1.width++;
			}
		}
		else*/
		{  // 3
			float f1 = m_pBmpInfo->biWidth;
			f1 /= m_pBmpInfo->biHeight;
			int t1;
			
			float f2 = cy*f1;
			t1 = f2;
			if(f2 - t1 > 0){
				t1++;
			}

			if(t1 >= cx){
				t1 = cx;
				m_rect1.width = t1;
				
				f2 = t1/f1;
				m_rect1.height = f2;
				if(f2 - m_rect1.height > 0)
					m_rect1.height++;
			}
			else{
				t1 = cy;				
				m_rect1.height = t1;

				f1 *= t1;
				m_rect1.width = f1;
				if(f1 - m_rect1.width > 0)
					m_rect1.width++;
			}
		}

		m_rect1.left = (cx - m_rect1.width)/2;
		m_rect1.top = (cy - m_rect1.height)/2;
	}
	else{  // m_rszFlag != 0
		// ????
		if(cx > m_pBmpInfo->biWidth){
			m_rect1.width = m_pBmpInfo->biWidth;
			m_rect1.left = (cx - m_pBmpInfo->biWidth)/2;
			m_rect3.left = 0;
			m_rect3.width = m_rect1.width;
		}
		else{
			m_rect1.width = cx;
			m_rect1.left = 0;
			m_rect3.width = m_rect1.width;

			int n = m_pBmpInfo->biWidth - cx;
			if(m_rect3.left > n)
				m_rect3.left = n;
		}
		
		if(cy > m_pBmpInfo->biHeight){
			m_rect1.height = m_pBmpInfo->biHeight;
			m_rect1.top = (cy - m_pBmpInfo->biHeight)/2;
			m_rect3.top = 0;
			m_rect3.height = m_rect1.height;
		}
		else{
			m_rect1.height = cy;
			m_rect1.top = 0;
			m_rect3.height = m_rect1.height;

			int n = m_pBmpInfo->biHeight - cy;
			if(m_rect3.top > n)
				m_rect3.top = n;
		}

		if(cx < m_pBmpInfo->biWidth){
			EnableScrollBarCtrl(SB_HORZ);
			
			CRect rect;
			GetClientRect(&rect);
			cx = rect.Width();

			int n = m_pBmpInfo->biWidth;// - cx;
			SCROLLINFO scrInfo;
			memset(&scrInfo, 0, sizeof(scrInfo));
			scrInfo.cbSize = sizeof(scrInfo);
			scrInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
			scrInfo.nPage = cx/IMAGE_MOVE_STEP_HORZ;
			scrInfo.nMin = 0;
			n = m_pBmpInfo->biWidth - cx;
			scrInfo.nMax = scrInfo.nPage + n/IMAGE_MOVE_STEP_HORZ;
			if((n%IMAGE_MOVE_STEP_HORZ) == 0) scrInfo.nMax -= 1;
			scrInfo.nPos = m_rect3.left/IMAGE_MOVE_STEP_HORZ;
			SetScrollInfo(SB_HORZ, &scrInfo);
			n = GetScrollLimit(SB_HORZ);
		}
		else
			EnableScrollBarCtrl(SB_HORZ,0);

		if(cy < m_pBmpInfo->biHeight){
			EnableScrollBarCtrl(SB_VERT);
			
			CRect rect;
			GetClientRect(&rect);
			cy = rect.Height();
			
			int n = m_pBmpInfo->biHeight;// - cy;
			SCROLLINFO scrInfo;
			memset(&scrInfo, 0, sizeof(scrInfo));
			scrInfo.cbSize = sizeof(scrInfo);
			scrInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
			scrInfo.nPage = cy/IMAGE_MOVE_STEP_VERT;
			scrInfo.nMin = 0;
			n = m_pBmpInfo->biHeight - cy;
			scrInfo.nMax = scrInfo.nPage + n/IMAGE_MOVE_STEP_VERT;
			if((n%IMAGE_MOVE_STEP_VERT) == 0) scrInfo.nMax -= 1;
			scrInfo.nPos = m_rect3.top/IMAGE_MOVE_STEP_VERT;
			SetScrollInfo(SB_VERT, &scrInfo);
			n = GetScrollLimit(SB_VERT);
		}
		else
			EnableScrollBarCtrl(SB_VERT,0);
	}
	
	
	CString buf;
	if(m_rect1.width == m_pBmpInfo->biWidth 
		&& m_rect1.height == m_pBmpInfo->biHeight){
		buf.Format("%s",m_title);
	}
	else{
		if(m_rszFlag == 0){
			buf.Format("*%s(%dx%d)",m_title,m_rect1.width,m_rect1.height);
			if(m_bGridFlag) m_bGridFlag = 0;
			if(m_pValuesDlg) m_pValuesDlg->ShowWindow(SW_HIDE);
		}
		else{
			buf.Format("+%s(%dx%d)",m_title,m_rect1.width,m_rect1.height);
			if(m_bGridOn){
				m_bGridFlag = 1;
				if(m_pValues2Dlg)
					m_pValues2Dlg->ShowWindow(SW_HIDE);
			}
		}
	}
	SetWindowText(buf);

	return 1;
}



//////////////////////////////////////////////////////////////////////////////////
//
//
BOOL ImageWnd::Create(CWnd* pParentWnd, UINT nID,LPCTSTR lpszWindowName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd::Create(NULL, lpszWindowName,
		WS_CHILD |WS_BORDER | WS_CAPTION |WS_SIZEBOX |WS_OVERLAPPEDWINDOW ,
		CRect(0,0,500,0),pParentWnd , nID, NULL);
	
	SetParent(FromHandle(HWND_DESKTOP));
	ModifyStyle(0, WS_POPUP|WS_CHILD|WS_SIZEBOX, SWP_FRAMECHANGED);
	//ModifyStyleEx(WS_EX_CLIENTEDGE,0, SWP_FRAMECHANGED);
	
	//
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);
	pSysMenu->AppendMenu(MF_STRING, IDM_ORIGIN_SIZE, "原始图像大小");
	pSysMenu->AppendMenu(MF_STRING, IDM_ONTOP,"窗口置顶");
	pSysMenu->AppendMenu(MF_STRING, IDM_SHOW_TITLE,"显示标题栏\tt");
	pSysMenu->AppendMenu(MF_STRING, IDM_SWITCH_SHOWMODE,"切换显示模式\tf");
	pSysMenu->AppendMenu(MF_STRING, IDM_HELP,"快捷键帮助\th");
	m_topWnd = 0;

	return 1;
}

void ImageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(IsIconic())
		return;
	updateLayerout(cx, cy);
	m_paintFlag = 1;
}


void ImageWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	displayImage(&dc);
	fillTheRest(&dc);

	// Do not call CWnd::OnPaint() for painting messages
}


void ImageWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	if(m_bGridOn == 0){
		updateValues2(point);
	}
	else{
		updateValues(point);
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void ImageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pW;
	pW = GetForegroundWindow();
	
	if(pW->m_hWnd != m_hWnd){
		SetForegroundWindow();
	}
	else{
		::PostMessage(m_hParent, WM_FRONT, 0, 0);
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void ImageWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),1);
	SetActiveWindow();
	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

void ImageWnd::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	//if(nID == SC_RESTORE){
	//	SetForegroundWindow();
	//}
	
	if((nID & 0xFFF0) == IDM_ONTOP){
		if(m_topWnd == 0){
			SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			m_topWnd = 1;
		}
		else{
			SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			m_topWnd = 0;
		}
		
		CMenu *pMenu = GetSystemMenu(FALSE);
		if(pMenu){
			UINT nFlags;
			nFlags = MF_STRING | MF_BYCOMMAND;
			nFlags |= m_topWnd ? MF_CHECKED : MF_UNCHECKED;
			pMenu->ModifyMenu(IDM_ONTOP, nFlags, IDM_ONTOP,"窗口置顶");
		}
	}
	else if( (nID & 0xFFF0) == IDM_ORIGIN_SIZE ){
		if(IsZoomed()){
			ShowWindow(SW_RESTORE);
		}

		m_pRender->getImageBmp(&m_pBmpInfo, &m_pBmpBits);
		adjustRect(1);
	}
	else if( (nID & 0xFFF0) == IDM_SHOW_TITLE ){
		CString buf;
		GetWindowText(buf);
		MessageBox(buf);
	}
	else if( (nID & 0xFFF0) == IDM_SWITCH_SHOWMODE ){
		{
			m_rszFlag = 1 - m_rszFlag;
			if(m_rszFlag == 0){
				EnableScrollBarCtrl(SB_HORZ, 0);
				EnableScrollBarCtrl(SB_VERT, 0);
			}
			CRect rect;
			GetClientRect(&rect);
			updateLayerout(rect.Width(),rect.Height());
			m_paintFlag = 1;
			Invalidate(0);
		}
	}
	else if( (nID&0xFFF0) == IDM_HELP ){
		MessageBox(HELP_MSG);
	}

	CWnd::OnSysCommand(nID, lParam);
}

void ImageWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}


void ImageWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags&MK_LBUTTON){
		if(0){
			POINT pt;
			pt.x=point.x;pt.y=point.y;
			ClientToScreen(&pt);
			SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
		}
	}
	else if(m_bGridFlag){
		if(m_pPosWnd == NULL){
			int width = 50;
			int height = 20;
			m_pPosWnd = new PosWnd;
			m_pPosWnd->Create(this,2,"pos wnd");
			m_pPosWnd->show(0);
			m_pPosWnd->MoveWindow(0,0,width,height,0);
			m_pPosWnd->m_hParent = m_hWnd;
		}

		if(m_pPosWnd){
			CRect rect(0,0,m_rect1.width,m_rect1.height);
			CPoint pt = point;
			pt.x -= m_rect1.left;
			pt.y -= m_rect1.top;
			if(pt.x <= 2 || pt.y <= 2 || pt.x >= rect.Width()-2 || pt.y >= rect.Height()-2)
				m_pPosWnd->show(0);
			else{
				getGridPos(pt,1);
				if(m_pt != pt){
					CRect rect2;
					CPoint pt2 = point;
					m_pt = pt;
					m_pPosWnd->setPos(pt, pt2);
					
					int x,y;
					int xx;
					int yy;
					m_pPosWnd->GetWindowRect(&rect2);
					xx = rect2.Width();
					yy = rect2.Height();
					x = point.x+m_gridXNum;
					y = point.y+m_gridYNum;
					if(x + xx > rect.right)
						x -= 10*2 + xx;
					if(y + yy > rect.bottom)
						y -= 10*2 + yy;
					m_pPosWnd->MoveWindow(x,y,xx,yy,1);
				}
			}
		}

		//return;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void ImageWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	::PostMessage(m_hParent, WM_IMAGEWND, 0, (LPARAM)m_hWnd);

	return;

	//CWnd::OnClose();
}

BOOL ImageWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void ImageWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if(nState == WA_ACTIVE)
		BringWindowToTop();
}

void ImageWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	// TODO: Add your message handler code here
	/*
	if(m_pPixelDlg){
		CRect rect1;
		CRect rect2;
		GetWindowRect(&rect1);
		m_pPixelDlg->GetWindowRect(&rect2);
		
		rect1.left += rect1.Width();
		rect1.right = rect1.left + rect2.Width();
		rect1.bottom = rect1.top + rect2.Height();
		
		m_pPixelDlg->MoveWindow(&rect1,1);
	}
	*/
}

void ImageWnd::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),1);
	SetActiveWindow();
	SetFocus();
	*/

	EnableScrollBarCtrl(0);
	EnableScrollBarCtrl(1);

	CWnd::OnMButtonDown(nFlags, point);
}

void ImageWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rszFlag){
		int n;
		switch(nChar){
		case VK_LEFT:
			OnHScroll(SB_LINELEFT, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnHScroll(SB_LINELEFT, 0, NULL);
			return;
		case VK_RIGHT:
			OnHScroll(SB_LINERIGHT, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnHScroll(SB_LINERIGHT, 0, NULL);
			return;
		case VK_UP:
			OnVScroll(SB_LINEUP, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnVScroll(SB_LINEUP, 0, NULL);
			return;
		case VK_DOWN:
			OnVScroll(SB_LINEDOWN, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnVScroll(SB_LINEDOWN, 0, NULL);
			return;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ImageWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar){
	case 'f':
	case 'F':
		{
			m_rszFlag = 1 - m_rszFlag;
			if(m_rszFlag == 0){
				EnableScrollBarCtrl(SB_HORZ, 0);
				EnableScrollBarCtrl(SB_VERT, 0);
			}
			CRect rect;
			GetClientRect(&rect);
			updateLayerout(rect.Width(),rect.Height());
			m_paintFlag = 1;
			Invalidate(0);
		}
		return;
	case '0':
	case 'o':
	case 'O':
	case 'x':
	case 'X':
		if(IsZoomed()){
			ShowWindow(SW_RESTORE);
		}
		
		m_pRender->getImageBmp(&m_pBmpInfo, &m_pBmpBits);
		adjustRect(1);
		return;
	case 'h':
	case 'H':
		MessageBox(HELP_MSG);
		return;
	case 't':
	case 'T':
		{
			CString buf;
			GetWindowText(buf);
			MessageBox(buf);
		}
		return;
	case 'g':
	case 'G':
		::SendMessage(m_hParent, WM_IMAGEWND, 1, 0);
		SetFocus();
		SetActiveWindow();
		return;
	case 'q':
	case 'Q':
		::PostMessage(m_hParent, WM_FRONT, 0, 0);
		return;
	}

	if(m_rszFlag){
		int n;
		switch(nChar){
		case 'a':
		case 'A':
			OnHScroll(SB_LINELEFT, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnHScroll(SB_LINELEFT, 0, NULL);
			return;
		case 'd':
		case 'D':
			OnHScroll(SB_LINERIGHT, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnHScroll(SB_LINERIGHT, 0, NULL);
			return;
		case 'w':
		case 'W':
			OnVScroll(SB_LINEUP, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnVScroll(SB_LINEUP, 0, NULL);
			return;
		case 's':
		case 'S':
			OnVScroll(SB_LINEDOWN, 0, NULL);
			if(GetKeyState(VK_CONTROL) >> 8)
				OnVScroll(SB_LINEDOWN, 0, NULL);
			return;
		}
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void ImageWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int n;
	switch(nSBCode){
		
	case SB_LEFT:
		SetScrollPos(SB_HORZ, 0);
		break;
	case SB_LINELEFT:
		n = IMAGE_MOVE_STEP_HORZ;
		if(n > 0){
			if(n > m_rect3.left) n = m_rect3.left;
			m_rect3.left -= n;
			Invalidate(0);
			n = GetScrollPos(SB_HORZ);
			SetScrollPos(SB_HORZ, n - 1);
		}
		break;
	case SB_RIGHT:
		n = GetScrollLimit(SB_HORZ);
		SetScrollPos(SB_HORZ, n);
		break;
	case SB_LINERIGHT:
		n = m_pBmpInfo->biWidth - m_rect1.width - m_rect3.left;
		if(n > 0){
			if(n > IMAGE_MOVE_STEP_HORZ) n = IMAGE_MOVE_STEP_HORZ;
			m_rect3.left += n;
			Invalidate(0);
			n = GetScrollPos(SB_HORZ);
			SetScrollPos(SB_HORZ, n + 1);
		}
		break;
	case SB_PAGERIGHT:
		break;
	case SB_PAGELEFT:
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		n = nPos*IMAGE_MOVE_STEP_HORZ;
		if(n > m_pBmpInfo->biWidth - m_rect1.width)
			n = m_pBmpInfo->biWidth - m_rect1.width;
		m_rect3.left = n;
		Invalidate(0);
		SetScrollPos(SB_HORZ, nPos);
		break;
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void ImageWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int n;
	switch(nSBCode){
		
	case SB_TOP:
		SetScrollPos(SB_VERT, 0);
		break;
	case SB_LINEUP:
		n = IMAGE_MOVE_STEP_VERT;
		if(n > 0){
			if(n > m_rect3.top) n = m_rect3.top;
			m_rect3.top -= n;
			Invalidate(0);
			n = GetScrollPos(SB_VERT);
			SetScrollPos(SB_VERT, n - 1);
		}
		break;
	case SB_BOTTOM:
		n = GetScrollLimit(SB_VERT);
		SetScrollPos(SB_VERT, n);
		break;
	case SB_LINEDOWN:
		n = m_pBmpInfo->biHeight - m_rect1.height - m_rect3.top;
		if(n > 0){
			if(n > IMAGE_MOVE_STEP_VERT) n = IMAGE_MOVE_STEP_VERT;
			m_rect3.top += n;
			Invalidate(0);
			n = GetScrollPos(SB_VERT);
			SetScrollPos(SB_VERT, n + 1);
		}
		break;
	case SB_PAGEDOWN:
		break;
	case SB_PAGEUP:
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		n = nPos*IMAGE_MOVE_STEP_VERT;
		if(n > m_pBmpInfo->biHeight - m_rect1.height)
			n = m_pBmpInfo->biHeight - m_rect1.height;
		m_rect3.top = n;
		Invalidate(0);
		SetScrollPos(SB_VERT, nPos);
		break;
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL ImageWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	int nh,nv;
	nv = rect.bottom - pt.y;
	nh = rect.right - pt.x;

	if( (nFlags&MK_SHIFT) || nv < 5 || rect.Height()/nv > 8){
		OnHScroll(zDelta > 0? SB_LINELEFT:SB_LINERIGHT, 0, NULL);
		if(nFlags&MK_CONTROL)
			OnHScroll(zDelta > 0? SB_LINELEFT:SB_LINERIGHT, 0, NULL);
		goto leave;
	}
	
	OnVScroll(zDelta > 0 ? SB_LINEUP:SB_LINEDOWN, 0, NULL);
	if(nFlags&MK_CONTROL)
		OnVScroll(zDelta > 0 ? SB_LINEUP:SB_LINEDOWN, 0, NULL);

leave:
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
