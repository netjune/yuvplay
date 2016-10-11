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

#include "StdAfx.h"
#include "Filter_upset.h"



#define IMAGE_FILTER_NAME  "图像上下翻转"

#define MODULE_VERSION    "1.0"
#define MODULE_AUTHOR     "netjunegg@gmail.com"
#define MODULE_COMMENT    "图像上下翻转"



CFilter_Upset::CFilter_Upset(){
	m_frame_size = 0;
	m_line_extra_bytes = 0;
	
	m_testVal = 1010;
	m_testVal2 = 1020;
	m_testStr = NULL;

	m_pBuf = NULL;
	m_bufSz = 0;
}

CFilter_Upset::~CFilter_Upset(){
	destroy();
}

// filter name
const char *__stdcall CFilter_Upset::getName(){
	return IMAGE_FILTER_NAME;
}
// other information
int __stdcall CFilter_Upset::getModuleInfo(Module_Info *pInfo){
	sprintf(pInfo->author_name,MODULE_AUTHOR);
	sprintf(pInfo->comment,MODULE_COMMENT);
	sprintf(pInfo->version,MODULE_VERSION);
	return E_OK;
}

int __stdcall CFilter_Upset::simpleConfig(int val, unsigned int val2, const char *str){
	m_testVal = val;
	m_testVal2 = val2;
	m_testStr = strdup(str);
	return 1;
}

int __stdcall CFilter_Upset::selfConfig(){

	return E_OK;
}

int __stdcall CFilter_Upset::queryRuntimeInfo(char *infoBuf, int bufsz){
	if(infoBuf == NULL || bufsz <= 100)
		return -1;

	sprintf(infoBuf, "frame size = %d\r\n"
			"test val = %d\r\n"
			"test val2 = %u\r\n"
			"test str = \"%s\"",
			m_frame_size, 
			m_testVal, m_testVal2, m_testStr ? m_testStr:"NULL");

	return 1;
}

// 设置参数, format, width, height
int __stdcall CFilter_Upset::setParams(RawImage_Info *pInfo){
	if(pInfo == NULL)
		return E_BAD_ARG;

	if(pInfo->raw_format != RAW_FORMAT_YUV444 
		&& pInfo->raw_format != RAW_FORMAT_BGR24 
		&& pInfo->raw_format != RAW_FORMAT_RGB24
		&& pInfo->raw_format != RAW_FORMAT_GREY8){
		return E_UNSUPPORTTED;
	}

	if(pInfo->width <= 0 || pInfo->height <= 0)
		return E_UNSUPPORTTED;

	m_inInfo = *pInfo;
	if(m_inInfo.raw_format == RAW_FORMAT_YUV444 ){
		m_frame_size = m_inInfo.width*m_inInfo.height;
		m_off1 = 0;
		m_off2 = m_frame_size;
		m_off3 = m_frame_size*2;
		m_frame_size *= 3;
		m_line_extra_bytes = 0;
	}
	else if(m_inInfo.raw_format == RAW_FORMAT_RGB24){
		m_frame_size = m_inInfo.width*m_inInfo.height;
		m_off1 = 0;
		m_off2 = 1;
		m_off3 = 2;
		m_frame_size *= 3;
		m_line_extra_bytes = 0;
	}
	else if(m_inInfo.raw_format == RAW_FORMAT_BGR24){
		m_line_extra_bytes = m_inInfo.width*3;
		while(m_line_extra_bytes&0x3) m_line_extra_bytes++;
		m_frame_size = m_line_extra_bytes*m_inInfo.height;
		m_line_extra_bytes -= m_inInfo.width*3;
		m_off1 = 2;
		m_off2 = 1;
		m_off3 = 0;
	}
	else if(m_inInfo.raw_format == RAW_FORMAT_GREY8){
		m_frame_size = m_inInfo.width*m_inInfo.height;
		m_off1 = 0;
		m_off2 = -1;
		m_off3 = -1;
		m_line_extra_bytes = 0;
	}
	else
		return E_UNSUPPORTTED;

	return E_OK;
}

int __stdcall CFilter_Upset::update(void *pErr){
	if(m_frame_size > m_bufSz){
		if(m_pBuf)
			free(m_pBuf);
		m_pBuf = NULL;
		m_bufSz = 0;
	}
	
	if(m_pBuf == NULL){
		m_pBuf = (unsigned char*)malloc(m_frame_size);
		if(m_pBuf == NULL)
			return -2;
	}

	return 1;
}

int __stdcall CFilter_Upset::getOutInfo(RawImage_Info *out){
	if(out == NULL)
		return E_BAD_ARG;
	*out = m_inInfo;
	return E_OK;
}


// filter
int __stdcall CFilter_Upset::filter(unsigned char **ppData, int *pLen){
	if(ppData == NULL || pLen == NULL)
		return E_BAD_ARG;

	unsigned char *pData = *ppData;
	int nBytes = *pLen;
	if(ppData == NULL || nBytes < m_frame_size)
		return E_BAD_ARG;

	if(m_inInfo.raw_format == RAW_FORMAT_BGR24
		|| m_inInfo.raw_format == RAW_FORMAT_RGB24){
		unsigned char *p1, *p2;
		int i;
		int line_pitch;
		
		p1 = pData;
		p2 = m_pBuf;
		line_pitch = m_inInfo.width*3+m_line_extra_bytes;
		p2 += line_pitch*(m_inInfo.height-1);
		
		for(i = 0;i < m_inInfo.height;i++){
			memcpy(p2, p1, line_pitch - m_line_extra_bytes);
			p1 += line_pitch;
			p2 -= line_pitch;
		}
	}
	else{
		int i,j;
		unsigned char *p1,*p2,*p3;
		unsigned char *p21,*p22,*p23;

		p1 = pData + m_off1;
		p2 = pData + m_off2;
		p3 = pData + m_off3;
	
		p21 = m_pBuf + m_off1;
		p22 = m_pBuf + m_off2;
		p23 = m_pBuf + m_off3;

		p21 += m_inInfo.width*(m_inInfo.height - 1);
		p22 += m_inInfo.width*(m_inInfo.height - 1);
		p23 += m_inInfo.width*(m_inInfo.height - 1);
	
		for(i = 0;i < m_inInfo.height;i++){
			if(m_off1 >= 0){
				memcpy(p21, p1, m_inInfo.width);
				p1 += m_inInfo.width;
				p21 -= m_inInfo.width;
			}
			if(m_off2 >= 0){
				memcpy(p22, p2, m_inInfo.width);
				p2 += m_inInfo.width;
				p22 -= m_inInfo.width;
			}
			if(m_off3 >= 0){
				memcpy(p23, p3, m_inInfo.width);
				p3 += m_inInfo.width;
				p23 -= m_inInfo.width;
			}
		}
	}

	*ppData = m_pBuf;
	return 1;
}

int __stdcall CFilter_Upset::destroy(){
	if(m_pBuf)
		free(m_pBuf);
	m_pBuf = NULL;
	return 1;
}


int filter_upset_get_interface(void **ppIntf){
	if(ppIntf == NULL)
		return -1;

	*ppIntf = new CFilter_Upset;
	return *ppIntf != NULL;
}



