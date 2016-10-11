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

#ifndef FORMATERMAN_H_
#define FORMATERMAN_H_


#include "Plugin.h"





class CFormaterItem{
public:
	int flag;
	IYUV_Formater *pItem;
	CString path;
};

typedef int (*yuv_formater_get_interface_func)(void **ppIntf);

class CFormaterItemInner{
public:
	HINSTANCE m_hDLL;
	yuv_formater_get_interface_func pf_get_interface;
	CString path;
};

class CFormaterMan{
	CFormaterItemInner *m_pItems;
	int m_num;
	int m_size;
	int m_initialized;	
	
	CString m_prefix;
	
	int expand();
	
public:
	CFormaterMan();
	~CFormaterMan();
	
	int beInitialized();
	int init(const char *path);
	
	int getNum();
	CFormaterItem **getList();
	
	static int destroyList(CFormaterItem **list);
	int destroy();
};


extern CFormaterMan gbl_formater_man;









#endif




