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

#ifndef FILTERMAN_H_
#define FILTERMAN_H_

#include "Plugin.h"





class CFilterItem{
public:
	int flag;
	IImage_Filter *pItem;
	CString path;
};

typedef int (*image_filter_get_interface_func)(void **ppIntf);

class CFilterItemInner{
public:
	HINSTANCE m_hDLL;
	image_filter_get_interface_func pf_get_interface;
	CString path;
};

class CFilterMan{
	CFilterItemInner *m_pItems;
	int m_num;
	int m_size;
	int m_initialized;	
	
	CString m_prefix;
	
	int expand();

public:
	CFilterMan();
	~CFilterMan();

	int beInitialized();
	int init(const char *path);

	int getNum();
	CFilterItem **getList();

	static int destroyList(CFilterItem **list);
	int destroy();
};


extern CFilterMan gbl_fielter_nam;



#endif



