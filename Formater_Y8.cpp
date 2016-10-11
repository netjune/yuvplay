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

#include "StdAfx.h"
#include "Formater_Y8.h"
#include "DlgFilter_Y8.h"


#define YUV_FORMATER_ID    0x1
#define YUV_FORMATER_NAME  "Y8"

#define MODULE_VERSION    "0.0.1"
#define MODULE_AUTHOR     "netjunegg@gmail.com"
#define MODULE_COMMENT    "y8, ֧��ģ���Y8��ʽ(UV���Ϊ128), ѡ��2������"


void CFormater_Y8::_init(){
	m_buf = NULL;
	m_bufsz = 0;
	
	m_block_size = 0;
	
	m_frame_num = 0;
	m_index = -1;
	
	m_width = m_height = 0;
	m_field = FIELD_UNKNOWN;
	m_y_size = 0;

	m_bEmu = 0;
	m_outFormat = RAW_FORMAT_UNKNOWN;
}
CFormater_Y8::CFormater_Y8(){
	_init();
}
CFormater_Y8::~CFormater_Y8(){
	destroy();
}

// format name
const char *__stdcall CFormater_Y8::getName(){
	return YUV_FORMATER_NAME;
}
// other information
int __stdcall CFormater_Y8::getModuleInfo(Module_Info *pInfo){
	sprintf(pInfo->author_name,MODULE_AUTHOR);
	sprintf(pInfo->comment,MODULE_COMMENT);
	sprintf(pInfo->version,MODULE_VERSION);
	return E_OK;
}

int __stdcall CFormater_Y8::simpleConfig(int val, unsigned int val2, const char *str){
	return 0;
}
int __stdcall CFormater_Y8::selfConfig(){
	CDlgFilter_Y8 dlg;
	dlg.m_bEmu = m_bEmu;
	if(dlg.DoModal() == IDOK)
		m_bEmu = dlg.m_bEmu;

	return 1;
}

int __stdcall CFormater_Y8::queryRuntimeInfo(char *infoBuf, int bufsz){
	if(infoBuf == NULL || bufsz <= 0)
		return -1;

	int flag = 0;
	if(m_bEmu && m_outFormat == RAW_FORMAT_GREY8)
		flag = 1;
	if( (!m_bEmu) && m_outFormat == RAW_FORMAT_YUV444)
		flag = 1;
	
	if(flag == 0){
		sprintf(infoBuf,"ģ���Y8��ʽ: %s",m_bEmu ? "��":"��");
	}
	else{
		sprintf(infoBuf,"ģ���Y8��ʽ: %s\r\n��Ҫ���³�ʼ��",m_bEmu ? "��":"��");
	}

	return 1;
}

// ����Դ���ݸ�ʽ, width, height, field
int __stdcall CFormater_Y8::setParams(YUV_Params *params){
	if(params == NULL)
		return E_BAD_ARG;
	if(params->width <= 0 || params->height <= 0 
		|| (params->width&1) || (params->height&1) )
		return E_UNSUPPORTTED;
	if(params->field != FIELD_ALL && params->field != FIELD_TOP_ONLY
		&& params->field != FIELD_BOTTOM_ONLY && params->field != FIELD_TOP_BOTTOM_SEQ)
		return E_UNSUPPORTTED;

	m_width = params->width;
	m_height = params->height;
	m_field = params->field;

	m_y_size = m_width*m_height;
	m_block_size = m_width*m_height;
	if(m_field == FIELD_TOP_BOTTOM_SEQ)
		m_frame_num = 2;
	else
		m_frame_num = 1;

	return E_OK;
}

int __stdcall CFormater_Y8::update(void *pErr){
	if(m_bufsz < m_block_size){
		if(m_buf)
			free(m_buf);
		m_bufsz = 0;
		m_buf = (unsigned char *)malloc(m_block_size);
		if(m_buf == NULL)
			return E_NO_MEMORY;
		m_bufsz = m_block_size;
	}
	
	return 1;
}

// ��ѯ������ݸ�ʽ, format, width, height
int __stdcall CFormater_Y8::getOutInfo(RawImage_Info *out){
	if(out == NULL)
		return E_BAD_ARG;
	if(m_bEmu) m_outFormat = RAW_FORMAT_YUV444;
	else m_outFormat = RAW_FORMAT_GREY8;
	out->raw_format = m_outFormat;
	if(m_field == FIELD_ALL){
		out->height = m_height;
	}
	else{
		out->height = m_height/2;
	}
	out->width = m_width;
	return E_OK;
}

int __stdcall CFormater_Y8::getMinBlockSize(){
	return m_block_size;
}
int __stdcall CFormater_Y8::getMinBlockFrameNum(){
	return m_frame_num;
}

// ��ȡbuffer, buf��СӦΪminBlockSize
int __stdcall CFormater_Y8::getBuffer(unsigned char **ppBuf){
	if(ppBuf == NULL)
		return E_BAD_ARG;
	*ppBuf = NULL;
	if(m_block_size <= 0 ||m_bufsz < m_block_size)
		return E_NO_INIT;
	
	*ppBuf = m_buf;
	return E_OK;
}
int __stdcall CFormater_Y8::putData(unsigned char *pBuf){
	if(pBuf != m_buf)
		return E_BAD_ARG;
	m_index = 0;
	return E_OK;
}

int __stdcall CFormater_Y8::getFrameIndex(){
	return m_index;
}
int __stdcall CFormater_Y8::setFrameIndex(int index){
	if(index < 0 || index > m_frame_num)
		return E_BAD_ARG;
	m_index = index;
	return E_OK;
}

// offset: < 0: ��һ֡, ==0: ��ǰ֡, >0: ��һ֡
int __stdcall CFormater_Y8::getFrame(int offset, unsigned char *pBuf, int bufsz){
	int index = m_index;
	if(index < 0)
		return E_NO_INIT;
	if(offset > 0) index++;
	else if(offset < 0) index--;
	
	if(index < 0 || index > m_frame_num)
		return 0;
	// added!!!
	if(index == 0 && m_field == FIELD_BOTTOM_ONLY)
		index = 1;
	
	int n;
	if(m_field == FIELD_ALL){  // all field
		unsigned char *p1,*p2;
		int i,j;
		
		// copy Y
		memcpy(pBuf, m_buf, m_y_size);
		if(m_bEmu)
			memset(pBuf + m_y_size, 128, m_y_size*2);
	}
	else if(index == 0){   // top field
		unsigned char *p1,*p2;
		int i,j;
		
		// copy Y
		p1 = m_buf;
		p2 = pBuf;
		for(i = 0;i < m_height/2;i++){
			for(j = 0;j < m_width;j++){
				*p2++ = *p1++;
			}
			p1 += m_width;
		}

		if(m_bEmu)
			memset(pBuf + m_y_size/2, 128, m_y_size);
	}
	else if(index == 1){
		unsigned char *p1,*p2;
		int i,j;
		
		// copy Y
		p1 = m_buf;
		p2 = pBuf;
		p1 += m_width;
		for(i = 0;i < m_height/2;i++){
			for(j = 0;j < m_width;j++){
				*p2++ = *p1++;
			}
			p1 += m_width;
		}

		if(m_bEmu)
			memset(pBuf + m_y_size/2, 128, m_y_size);
	}
	else
		return E_OOPS;
	
	return index;
}

// ��ʽת��: 
// ÿ��ת��, �˺������ܻᱻ��������
// 1. ��dst_bufΪNULL, �������ⲿӦ�ò�ѯdst_buf�Ĵ�С, ����p_dst_bufsz�з���dst_buf�Ĵ�С, �ⲿӦ�û���ݴ˴�Сȥ����dst_buf
// 2. �ڶ��ε���dst_buf��Ϊ��Чֵ, ��ʱ����ת��, ����ת�������Ч�����ֽ�������p_dst_bufsz��
// 3. ͬʱ, ����src_info, ����֧��, �뷵�ظ�ֵ
// 4. �˺�����Ҫ�޸Ķ����������Ա, ��ҪӰ�쵽������������Ϊ����, ��������
int __stdcall CFormater_Y8::toYourFormat(RawImage_Info *src_info, const unsigned char *src_buf, int src_bufsz, unsigned char *dst_buf, int *p_dst_bufsz){
	if(src_info == NULL)
		return -1;
	
	if(src_info->raw_format != RAW_FORMAT_YUV444)
		return -2;
	
	int outsize = src_info->width*src_info->height;
	if(outsize <= 0)
		return -3;
	
	if(dst_buf == NULL){
		if(p_dst_bufsz == NULL)
			return -4;
		*p_dst_bufsz = outsize;
		return E_OK;
	}
	
	if(src_buf == NULL || dst_buf == NULL)
		return -5;
	
	int width = src_info->width;
	int height = src_info->height;
	int y_size = width*height;
	const unsigned char *p1;
	unsigned char *p2;
	int i,j;
	
	// Y
	p1 = src_buf;
	p2 = dst_buf;
	memcpy(p2, p1, y_size);
	
	return 1;
}

int __stdcall CFormater_Y8::destroy(){
	if(m_buf)
		free(m_buf);
	_init();
	return 1;
}


int formater_y8_get_interface(void **ppIntf){
	if(ppIntf == NULL)
		return -1;
	*ppIntf = new CFormater_Y8;
	return (ppIntf != NULL);
}
