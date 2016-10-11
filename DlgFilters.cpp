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

// CDlgFilters.cpp : implementation file
//

#include "stdafx.h"
#include "yuvplay.h"
#include "DlgFilters.h"
#include "DlgFilter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFilters dialog


CDlgFilters::CDlgFilters(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilters::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilters)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_ppFilters = NULL;
}


void CDlgFilters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilters)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

int  CDlgFilters::initColumn(){
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	CRect rect;
	
	list->GetWindowRect(&rect);
	
	list->ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS |LVS_SINGLESEL);// |LVS_OWNERDRAWFIXED);
	list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES |LVS_EX_HEADERDRAGDROP );// |LVS_EX_GRIDLINES);// |LVS_EX_HEADERDRAGDROP );
	
	list->InsertColumn(0, _T("滤镜列表"), LVCFMT_LEFT);
	list->SetColumnWidth(0,rect.Width() - 24);
	
	//GetCurrentDirectory(MAX_PATH, m_prefix.GetBuffer(MAX_PATH));
	//m_prefix.ReleaseBuffer();
	
	return 1;
}

int  CDlgFilters::addItem(const char *name, int id, int checked){
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int nItem;
	nItem = list->GetItemCount();
	list->InsertItem(nItem,name);
	list->SetItemData(nItem,id);
	if(checked)
		list->SetCheck(nItem);
	return 1;
}

int  CDlgFilters::seeItem(int index){
	CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	
	if(index < 0 || index >= list->GetItemCount())
		return 0;
	
	int item = list->GetTopIndex();
	int pageSz = list->GetCountPerPage();
	
	CRect rect;
	list->GetItemRect(0,&rect,LVIR_BOUNDS);
	
	if(index < item){
		CSize sz;
		
		sz.cy = index - item;
		sz.cy *= rect.Height();
		sz.cx = 0;
		
		list->Scroll(sz);
	}
	else if(index >= item + pageSz){
		CSize sz;
		
		sz.cy = index + 1 - pageSz -item;
		sz.cy *= rect.Height();
		sz.cx = 0;
		
		list->Scroll(sz);
	}
	
	return 1;
}
BEGIN_MESSAGE_MAP(CDlgFilters, CDialog)
	//{{AFX_MSG_MAP(CDlgFilters)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY, OnButtonProperty)
	ON_BN_CLICKED(IDC_BUTTON_CFG, OnButtonCfg)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILTERS, OnDblclkListFilters)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_FILTERS, OnRdblclkListFilters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFilters message handlers

BOOL CDlgFilters::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initColumn();
	
	if(m_ppFilters){
		int i;
		for(i = 0;;i++){
			if(m_ppFilters[i] == NULL)
				break;
			addItem(m_ppFilters[i]->pItem->getName(), i, m_ppFilters[i]->flag);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFilters::OnButtonProperty() 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index < 0){
		AfxMessageBox("没有选择插件");
		return;
	}
	
	int id;
	id = list->GetItemData(index);
	Module_Info info;
	m_ppFilters[id]->pItem->getModuleInfo(&info);

	CDlgFilter dlg;
	dlg.m_pFilter = m_ppFilters[id];
	dlg.DoModal();
}

void CDlgFilters::OnButtonCfg() 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index < 0){
		AfxMessageBox("没有选择插件");
		return;
	}
	
	int id = list->GetItemData(index);
	if(m_ppFilters[id]->pItem->selfConfig() == 0){
		MessageBox("没有配置");
		return;
	}
}

void CDlgFilters::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index < 0){
		AfxMessageBox("没有选择模块");
		return;
	}
	
	if(index == 0)
		return;
	
	CString str = list->GetItemText(index,0);
	int itemData = list->GetItemData(index);
	int bCheck = list->GetCheck(index);
	
	list->DeleteItem(index);
	list->InsertItem(index-1, str);
	list->SetItemData(index -1, itemData);
	list->SetCheck(index - 1, bCheck);
	
	list->SetItemState(index-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED |LVIS_FOCUSED);
	
	seeItem(index - 1);
	
	list->SetSelectionMark(index-1);
	//myscroll(1);
}

void CDlgFilters::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index < 0){
		AfxMessageBox("没有选择模块");
		return;
	}
	
	if(index >= list->GetItemCount() - 1)
		return;
	
	CString str = list->GetItemText(index,0);
	int itemData = list->GetItemData(index);
	int bCheck = list->GetCheck(index);
	
	list->DeleteItem(index);
	
	list->InsertItem(index+1, str);
	list->SetItemData(index +1, itemData);
	list->SetCheck(index + 1, bCheck);
	
	list->SetItemState(index+1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED |LVIS_FOCUSED);
	
	seeItem(index+1);
	
	list->SetSelectionMark(index+1);
	
	POSITION pos = list->GetFirstSelectedItemPosition();
	index = list->GetNextSelectedItem(pos);
	
	//myscroll(0);
}

void CDlgFilters::OnDblclkListFilters(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index >= 0){	
		OnButtonCfg();
	}

	*pResult = 0;
}

void CDlgFilters::OnRdblclkListFilters(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CListCtrl  *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int index = list->GetSelectionMark();
	if(index >= 0){
		OnButtonProperty();
	}
	
	*pResult = 0;
}

void CDlgFilters::OnOK() 
{
	// TODO: Add extra validation here
	CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_LIST_FILTERS);
	int nItem = list->GetItemCount();
	int i;
	int j;
	
	if(nItem <= 0)
		return;

	CFilterItem **ppItems = (CFilterItem**)malloc((nItem+1)*sizeof(CFilterItem*));
	if(ppItems == NULL){
		AfxMessageBox("malloc failed");
		return;
	}

	for(i = 0,j = 0;i < nItem;i++){
		int id = list->GetItemData(i);
		ppItems[i] = m_ppFilters[id];
		ppItems[i]->flag = list->GetCheck(i);
		if(ppItems[i]->flag)
			m_ppFilters2[j++] = ppItems[i]->pItem;
	}
	m_ppFilters2[j] = NULL;

	// 保存顺序
	for(i = 0;i < nItem;i++){
		m_ppFilters[i] = ppItems[i];
	}
	m_ppFilters[i] = NULL;
	free(ppItems);

	CDialog::OnOK();
}

void CDlgFilters::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
