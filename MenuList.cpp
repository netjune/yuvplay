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

#include "stdafx.h"
#include "MenuList.h"

MenuList::MenuList(){
	m_size = 0;
	m_num = 0;
	m_pItem = NULL;
	m_pLast = NULL;
}

MenuList::~MenuList(){
	destroy();
}

int MenuList::create(int size, int startId){
	if(m_pItem){
		destroy();
	}

	if(size <= 0)
		return -1;

	MenuItem *pItem;
	int i;
	for(i = 0;i < size;i++){
		if(m_pItem == NULL){
			m_pItem = new MenuItem;
			pItem = m_pItem;
		}
		else{
			pItem->next = new MenuItem;
			pItem = pItem->next;
		}
		pItem->id = startId++;
		pItem->next = NULL;
	}

	m_size = size;
	m_num = 0;
	m_pLast = NULL;

	return 1;
}

int MenuList::appendItem(LPCTSTR name, int& id){
	if(m_size <= 0 || m_num >= m_size)
		return -2;  // full
	
	//deleteItem(name);

	MenuItem *pItem;
	if(m_pLast == NULL){
		pItem = m_pItem;
	}
	else{
		pItem = m_pLast->next;
	}
	
	pItem->name = name;
	id = pItem->id;

	m_num++;
	m_pLast = pItem;

	return 1;
}

int MenuList::insertItem(int pos, LPCTSTR name, int& id){
	if(m_size <= 0 || m_num >= m_size)
		return -2;

	if(pos < 0 || pos >= m_num)
		return appendItem(name,id);

	//deleteItem(name);

	MenuItem *pItem;
	int i;

	pItem = m_pItem;
	for(i = 0;i < pos - 1;i++)
		pItem = pItem->next;

	MenuItem *pItem2 = m_pLast->next;
	m_pLast->next = pItem2->next;

	pItem2->name = name;
	pItem2->next = pItem->next;

	pItem->next = pItem2;

	id = pItem->id;

	m_num++;

	return 1;
}

int MenuList::deleteItem(int id){
	if(m_size <= 0 || m_num <= 0)
		return -1;

	MenuItem *pItem = m_pItem;
	int i;
	MenuItem *pParent;

	for(i = 0;i < m_num;i++){
		if(pItem->id == id)
			break;

		pParent = pItem;
		pItem = pItem->next;
	}
	
	if(i >= m_num)
		return 0;

	if(pItem == m_pItem){
		m_pItem = m_pItem->next;
		pItem->next = m_pLast->next;
		m_pLast->next = pItem;
	}
	else if(pItem == m_pLast){
		m_pLast = pParent;
	}
	else{
		pParent->next = pItem->next;
		pItem->next = m_pLast->next;
		m_pLast->next = pItem;
	}


	m_num--;
	return 1;
}

int MenuList::deleteItem(LPCTSTR name){
	if(m_size <= 0 || m_num <= 0)
		return -1;

	MenuItem *pItem = m_pItem;
	int i;
	MenuItem *pParent;

	for(i = 0;i < m_num;i++){
		if(!pItem->name.CompareNoCase(name))
			break;

		pParent = pItem;
		pItem = pItem->next;
	}

	if(i >= m_num)
		return 0;

	if(pItem == m_pItem){
		m_pItem = m_pItem->next;
		pItem->next = m_pLast->next;
		m_pLast->next = pItem;
	}
	else{
		pParent->next = pItem->next;
		pItem->next = m_pLast->next;
		m_pLast->next = pItem;
	}

	m_num--;
	return 1;
}

int MenuList::getItemName(int id, CString& name){
	if(m_size <= 0 || m_num <= 0)
		return -1;

	int i;
	MenuItem *pItem = m_pItem;

	for(i = 0;i < m_num;i++){
		if(pItem->id == id){
			name = pItem->name;
			return 1;
		}

		pItem = pItem->next;
	}

	return -2;
}

int MenuList::getItemId(LPCTSTR name, int& id){
	if(m_size <= 0 || m_num <= 0)
		return -1;

	int i;
	MenuItem *pItem = m_pItem;

	for(i = 0;i < m_num;i++){
		if(!pItem->name.CompareNoCase(name)){
			id = pItem->id;
			return 1;
		}

		pItem = pItem->next;
	}

	return -2;
}

int MenuList::getItem(int index, int &id, CString& name){
	if(m_size < 0 || m_num < index + 1)
		return -1;

	int i;
	MenuItem *pItem = m_pItem;

	for(i = 0;i < index;i++){
		pItem = pItem->next;
	}

	id = pItem->id;
	name = pItem->name;

	return 1;
}

int MenuList::isFull(){
	return m_num >= m_size;
}
int MenuList::isEmpty(){
	return m_num == 0;
}

int MenuList::destroy(){
	if(m_size > 0 && m_pItem){
		MenuItem *pItem = m_pItem;
		
		while(pItem){
			m_pItem = pItem;
			pItem = pItem->next;
			free(m_pItem);
		}
	}
	
	m_pLast = NULL;
	m_size = m_num = 0;

	return 1;
}
