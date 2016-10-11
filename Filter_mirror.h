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

	// 设置参数, format, width, height
	int __stdcall setParams(RawImage_Info *pInfo);
	int __stdcall update(void *pErr);

	int __stdcall getOutInfo(RawImage_Info *out);

	// filter
	int __stdcall filter(unsigned char **ppData, int *pLen);
	
	int __stdcall destroy();
};



int filter_mirror_get_interface(void **ppIntf);



#endif



