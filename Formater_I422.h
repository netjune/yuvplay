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

#ifndef FORMATER_I422_H_
#define FORMATER_I422_H_
#include "Plugin.h"





class CFormater_I422:  public IYUV_Formater{
	unsigned char *m_buf;
	int m_bufsz;

	int m_block_size;
	
	int m_frame_num;
	int m_index;
	
	unsigned short m_width;
	unsigned short m_height;
	int m_field;
	int m_y_size;
	
	int m_id;	
	
	void _init();

public:
	CFormater_I422();
	~CFormater_I422();
	
	// format name
	const char *__stdcall getName();
	// other information
	int __stdcall getModuleInfo(Module_Info *pInfo);
	
	int __stdcall simpleConfig(int val, unsigned int val2, const char *str);
	int __stdcall selfConfig();
	
	int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz);
	
	// 设置源数据格式, width, height, field
	int __stdcall setParams(YUV_Params *params);
	int __stdcall update(void *pErr);
	
	// 查询输出数据格式, format, width, height
	int __stdcall getOutInfo(RawImage_Info *out);
	
	int __stdcall getMinBlockSize();
	int __stdcall getMinBlockFrameNum();
	
	// 获取buffer, buf大小应为minBlockSize
	int __stdcall getBuffer(unsigned char **ppBuf);
	int __stdcall putData(unsigned char *pBuf);
	
	int __stdcall getFrameIndex();
	int __stdcall setFrameIndex(int index);
	
	// offset: < 0: 上一帧, ==0: 当前帧, >0: 下一帧
	int __stdcall getFrame(int offset, unsigned char *pBuf, int bufsz);  // a
	
	// 格式转换: 
	// 每次转换, 此函数可能会被调用两次
	// 1. 若dst_buf为NULL, 用来让外部应用查询dst_buf的大小, 请在p_dst_bufsz中返回dst_buf的大小, 外部应用会根据此大小去申请dst_buf
	// 2. 第二次调用dst_buf将为有效值, 此时可做转换, 并将转换后的有效数据字节数填入p_dst_bufsz中
	// 3. 同时, 请检查src_info, 若不支持, 请返回负值
	// 4. 此函数不要修改对象的其他成员, 不要影响到其他函数的行为即可, 尽量独立
	int __stdcall toYourFormat(RawImage_Info *src_info, const unsigned char *src_buf, int src_bufsz, unsigned char *dst_buf, int *p_dst_bufsz);
	
	int __stdcall destroy();
};


int formater_i422_get_interface(void **ppIntf);


#endif


