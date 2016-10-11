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

// PosWnd.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "PosWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PosWnd

PosWnd::PosWnd()
{
	m_show = 1;
}

PosWnd::~PosWnd()
{
}


BEGIN_MESSAGE_MAP(PosWnd, CWnd)
	//{{AFX_MSG_MAP(PosWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PosWnd message handlers
int  PosWnd::setPos(CPoint& pt_grid, CPoint& pt_pixel){
	int flag = 0;

	if(m_gridX != pt_grid.x)
		m_gridX = pt_grid.x,flag++;
	if(m_gridY != pt_grid.y)
		m_gridY = pt_grid.y,flag++;

	m_buf.Format("(%d,%d)",m_gridX,m_gridY);
	
	/*
	CSize sz;
	CDC *pDC = GetDC();
	m_buf.Format("(%d,%d)",m_gridX,m_gridY);
	sz = pDC->GetTextExtent(m_buf);
	pDC->DeleteDC();

	CRect rect2;
	GetWindowRect(&rect2);
	rect2.right = rect2.left + sz.cx + 5;
	MoveWindow(&rect2);
*/

	if(flag > 0)
		InvalidateRect(NULL);
	
	if(m_show == 0)
		show();

	return 1;
}

int PosWnd::show(int show){
	if(m_show != show){
		m_show = show;
		ShowWindow(m_show ? SW_SHOW : SW_HIDE);
	}
	else{
		if(show == 0)
			ShowWindow(SW_HIDE);
	}

	return 1;
}

BOOL PosWnd::Create(CWnd* pParentWnd, UINT nID,LPCTSTR lpszWindowName)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::Create(NULL, lpszWindowName,
			WS_CHILD|WS_VISIBLE ,
			CRect(0,0,0,0),pParentWnd , nID, NULL);
	
	return 1;
}

BOOL PosWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect,&CBrush(RGB(236,233,216)));

	return CWnd::OnEraseBkgnd(pDC);
}

void PosWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect  rect;

	GetClientRect(&rect);

	//buf.Format("(%d,%d)",m_gridX,m_gridY);
	//dc.SetTextColor(RGB(236,233,216));
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_buf,&rect,DT_CENTER);

	// Do not call CWnd::OnPaint() for painting messages
}

void PosWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	show(0);
	//return;
/*
	CPoint pt = point;
	CRect rect;
	unsigned int x;

	GetWindowRect(&rect);
	
	pt.x += rect.left;
	pt.y += rect.top;
	x = 1000;
	//::PostMessage(m_hParent,WM_MOUSEMOVE,x,x-1);
*/
	CWnd::OnMouseMove(nFlags, point);
}
