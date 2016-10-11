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

#ifndef FORMATER_RGB24_H_
#define FORMATER_RGB24_H_
#include "Plugin.h"



class CFormater_RGB24:  public IYUV_Formater{
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
	CFormater_RGB24();
	~CFormater_RGB24();
	
	// format name
	const char *__stdcall getName();
	// other information
	int __stdcall getModuleInfo(Module_Info *pInfo);
	
	int __stdcall simpleConfig(int val, unsigned int val2, const char *str);
	int __stdcall selfConfig();
	
	int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz);
	
	// ����Դ���ݸ�ʽ, width, height, field
	int __stdcall setParams(YUV_Params *params);
	int __stdcall update(void *pErr);
	
	// ��ѯ������ݸ�ʽ, format, width, height
	int __stdcall getOutInfo(RawImage_Info *out);
	
	int __stdcall getMinBlockSize();
	int __stdcall getMinBlockFrameNum();
	
	// ��ȡbuffer, buf��СӦΪminBlockSize
	int __stdcall getBuffer(unsigned char **ppBuf);
	int __stdcall putData(unsigned char *pBuf);
	
	int __stdcall getFrameIndex();
	int __stdcall setFrameIndex(int index);
	
	// offset: < 0: ��һ֡, ==0: ��ǰ֡, >0: ��һ֡
	int __stdcall getFrame(int offset, unsigned char *pBuf, int bufsz);  // a
	
	// ��ʽת��: 
	// ÿ��ת��, �˺������ܻᱻ��������
	// 1. ��dst_bufΪNULL, �������ⲿӦ�ò�ѯdst_buf�Ĵ�С, ����p_dst_bufsz�з���dst_buf�Ĵ�С, �ⲿӦ�û���ݴ˴�Сȥ����dst_buf
	// 2. �ڶ��ε���dst_buf��Ϊ��Чֵ, ��ʱ����ת��, ����ת�������Ч�����ֽ�������p_dst_bufsz��
	// 3. ͬʱ, ����src_info, ����֧��, �뷵�ظ�ֵ
	// 4. �˺�����Ҫ�޸Ķ����������Ա, ��ҪӰ�쵽������������Ϊ����, ��������
	int __stdcall toYourFormat(RawImage_Info *src_info, const unsigned char *src_buf, int src_bufsz, unsigned char *dst_buf, int *p_dst_bufsz);
	
	int __stdcall destroy();
};


int formater_rgb24_get_interface(void **ppIntf);


#endif



