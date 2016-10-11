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

#ifndef FILTER_MIRROR_H_
#define FILTER_MIRROR_H_
#include "Plugin.h"


class CFilter_Mirror: public IImage_Filter{
public:
	RawImage_Info m_inInfo;
	int m_frame_size;
	int m_line_extra_bytes;
	int m_off1;
	int m_off2;
	int m_off3;
	
	int m_testVal;
	unsigned int m_testVal2;
	char *m_testStr;
	
	unsigned char *m_pBuf;
	int m_bufSz;

	CFilter_Mirror();
	~CFilter_Mirror();

	// filter name
	const char *__stdcall getName();
	// other information
	int __stdcall getModuleInfo(Module_Info *pInfo);
	
	int __stdcall simpleConfig(int val, unsigned int val2, const char *str);
	int __stdcall selfConfig();
	
	int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz);

	// ���ò���, format, width, height
	int __stdcall setParams(RawImage_Info *pInfo);
	int __stdcall update(void *pErr);

	int __stdcall getOutInfo(RawImage_Info *out);

	// filter
	int __stdcall filter(unsigned char **ppData, int *pLen);
	
	int __stdcall destroy();
};



int filter_mirror_get_interface(void **ppIntf);



#endif



