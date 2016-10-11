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

#ifndef PLUGIN_H_
#define PLUGIN_H_

// use it
#define E_OK              1
#define E_BAD_ARG         -2   // ��������, ���������
#define E_UNSUPPORTTED    -2   // ��������, ���Լ���֧��
#define E_NO_MEMORY       -3   // �����ڴ�ʧ��
#define E_NO_INIT         -4   // δ��ʼ��
#define E_OOPS            -5   // �����˲��ó��ֵ�״̬
#define E_USER            -6


#define MODULE_NAME_MAX_LENGTH      64
#define MODULE_VERSION_MAX_LENGTH   64
#define MODULE_AUTHOR_MAX_LENGTH    64
#define MODULE_COMMENT_MAX_LENGTH   1024

// ģ��������Ϣ
typedef struct Module_Info{
	char version[MODULE_VERSION_MAX_LENGTH];		// �汾��
	char author_name[MODULE_AUTHOR_MAX_LENGTH];	// ��������
	char comment[MODULE_COMMENT_MAX_LENGTH];		// ע��
}Module_Info;


#define FIELD_UNKNOWN         0
#define FIELD_ALL             1     // ��������ʾ, ���
#define FIELD_TOP_ONLY        2     // ����ʾż��, top
#define FIELD_BOTTOM_ONLY     3     // ����ʾ�泡, bottom
#define FIELD_TOP_BOTTOM_SEQ  4     // ����ʾ, ��top, Ȼ��bottom
#define FIELD_TOP_BOTTOM_SP   5     // ż������, һ������


typedef struct YUV_Params{
	unsigned short width;
	unsigned short height;
	int field;
}YUV_Params;


#define RAW_FORMAT_UNKNOWN    0
#define RAW_FORMAT_YUV444     1
#define RAW_FORMAT_RGB24      2   // rgb,rgb...
#define RAW_FORMAT_BGR24      3   // bmpͼƬ��ʽ
#define RAW_FORMAT_GREY8      4
#define RAW_FORMAT_USER       10  // ����Ϊ�û��Զ����ʽ


typedef struct RawImage_Info{
	int raw_format;
	int width;
	int height;
	int frame_size;
}RawImage_Info;


class IYUV_Formater{
public:
	// format name
	virtual const char *__stdcall getName() = 0;
	// other information
	virtual int __stdcall getModuleInfo(Module_Info *pInfo) = 0;
	
	// �������÷�ʽ
	virtual int __stdcall simpleConfig(int val, unsigned int val2, const char *str) = 0;
	virtual int __stdcall selfConfig() = 0;

	// ������, �ɲ鿴һЩ�ڲ�״̬
	virtual int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz) = 0;
	
	// ����Դ���ݸ�ʽ, width, height, field, ����֧��,�뷵�ظ�ֵ
	virtual int __stdcall setParams(YUV_Params *params) = 0;
	// �����ڲ�״̬, ����setParams֮������ŵ���
	virtual int __stdcall update(void *pErr = NULL) = 0;

	// ��ѯ������ݸ�ʽ, ��out����д�Լ�֧�ֵ������ʽ, format, width, height
	virtual int __stdcall getOutInfo(RawImage_Info *out) = 0;
	
	virtual int __stdcall getMinBlockSize() = 0;
	virtual int __stdcall getMinBlockFrameNum() = 0;
	
	// ��ȡbuffer, buf��СӦΪminBlockSize
	virtual int __stdcall getBuffer(unsigned char **ppBuf) = 0;
	// ����֡����
	virtual int __stdcall putData(unsigned char *pBuf) = 0;
	
	virtual int __stdcall getFrameIndex() = 0;
	virtual int __stdcall setFrameIndex(int index) = 0;
	
	// offset: < 0: ��һ֡, ==0: ��ǰ֡, >0: ��һ֡
	virtual int __stdcall getFrame(int offset, unsigned char *pBuf, int bufsz) = 0;  // a

	// ��ʽת��: 
	// ÿ��ת��, �˺������ܻᱻ��������
	// 1. ��dst_bufΪNULL, �������ⲿӦ�ò�ѯdst_buf�Ĵ�С, ����p_dst_bufsz�з���dst_buf�Ĵ�С, �ⲿӦ�û���ݴ˴�Сȥ����dst_buf
	// 2. �ڶ��ε���dst_buf��Ϊ��Чֵ, ��ʱ����ת��, ����ת�������Ч�����ֽ�������p_dst_bufsz��
	// 3. ͬʱ, ����src_info, ����֧��, �뷵�ظ�ֵ
	// 4. �˺�����Ҫ�޸Ķ����������Ա, ��ҪӰ�쵽������������Ϊ����, ��������
	virtual int __stdcall toYourFormat(RawImage_Info *src_info, const unsigned char *src_buf, int src_bufsz, unsigned char *dst_buf, int *p_dst_bufsz) = 0;

	// �ͷ��ڲ�������ڴ�, ���״̬, 
	virtual int __stdcall destroy() = 0;
};



class IImage_Filter{
public:
	// filter name
	virtual const char *__stdcall getName() = 0;
	// other information
	virtual int __stdcall getModuleInfo(Module_Info *pInfo) = 0;
	
	// �������÷�ʽ
	virtual int __stdcall simpleConfig(int val, unsigned int val2, const char *str) = 0;
	virtual int __stdcall selfConfig() = 0;
	
	// ������, �ɲ鿴һЩ�ڲ�״̬
	virtual int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz) = 0;

	// ���ò���, format, width, height
	virtual int __stdcall setParams(RawImage_Info *pInfo) = 0;
	// �����ڲ�״̬, ������setParams����
	virtual int __stdcall update(void *pErr = NULL) = 0;

	// ��ѯ������ݸ�ʽ, ��out����д�Լ�֧�ֵ������ʽ, format, width, height
	virtual int __stdcall getOutInfo(RawImage_Info *out) = 0;

	// filter
	virtual int __stdcall filter(unsigned char **ppData, int *pLen) = 0;

	virtual int __stdcall destroy() = 0;
};




#endif

