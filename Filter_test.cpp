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
#include "Filter_test.h"
#include "DlgFilterGain.h"



#define IMAGE_FILTER_NAME  "简单增益调节"

#define MODULE_VERSION    "0.0.1"
#define MODULE_AUTHOR     "netjunegg@gmail.com"
#define MODULE_COMMENT    "一个简单滤镜, 调节分量增益\r\n" \
						"不改变格式, 不改变分辨率"



CFilter_Test::CFilter_Test(){
	m_frame_size = 0;
	m_line_extra_bytes = 0;

	m_gain1 = 1;
	m_gain2 = 1;
	m_gain3 = 1;
	
	m_testVal = 1010;
	m_testVal2 = 1020;
	m_testStr = NULL;
}

CFilter_Test::~CFilter_Test(){
	destroy();
}

// filter name
const char *__stdcall CFilter_Test::getName(){
	return IMAGE_FILTER_NAME;
}
// other information
int __stdcall CFilter_Test::getModuleInfo(Module_Info *pInfo){
	sprintf(pInfo->author_name,MODULE_AUTHOR);
	sprintf(pInfo->comment,MODULE_COMMENT);
	sprintf(pInfo->version,MODULE_VERSION);
	return E_OK;
}

int __stdcall CFilter_Test::simpleConfig(int val, unsigned int val2, const char *str){
	m_testVal = val;
	m_testVal2 = val2;
	m_testStr = strdup(str);
	return 1;
}

int __stdcall CFilter_Test::selfConfig(){
	CDlgFilterGain dlg;
	dlg.m_gain1 = m_gain1;
	dlg.m_gain2 = m_gain2;
	dlg.m_gain3 = m_gain3;
	if(dlg.DoModal() == IDOK){
		m_gain1 = dlg.m_gain1;
		m_gain2 = dlg.m_gain2;
		m_gain3 = dlg.m_gain3;
	}

	return E_OK;
}

int __stdcall CFilter_Test::queryRuntimeInfo(char *infoBuf, int bufsz){
	if(infoBuf == NULL || bufsz <= 100)
		return -1;

	sprintf(infoBuf, "frame size = %d\r\n"
			"Y gain = %f\r\n"
			"U gain = %f\r\n"
			"V gain = %f\r\n"
			"test val = %d\r\n"
			"test val2 = %u\r\n"
			"test str = \"%s\"",
			m_frame_size, m_gain1, m_gain2,m_gain3,
			m_testVal, m_testVal2, m_testStr ? m_testStr:"NULL");

	return 1;
}

// 设置参数, format, width, height
int __stdcall CFilter_Test::setParams(RawImage_Info *pInfo){
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

int __stdcall CFilter_Test::update(void *pErr){
	return 1;
}

int __stdcall CFilter_Test::getOutInfo(RawImage_Info *out){
	if(out == NULL)
		return E_BAD_ARG;
	*out = m_inInfo;
	return E_OK;
}


// filter
int __stdcall CFilter_Test::filter(unsigned char **ppData, int *pLen){
	if(ppData == NULL || pLen == NULL)
		return E_BAD_ARG;

	unsigned char *pData = *ppData;
	int nBytes = *pLen;
	if(ppData == NULL || nBytes < m_frame_size)
		return E_BAD_ARG;

	int i,j;
	unsigned char *p1,*p2,*p3;
	p1 = pData + m_off1;
	p2 = pData + m_off2;
	p3 = pData + m_off3;
	for(i = 0;i < m_inInfo.height;i++){
		for(j = 0;j < m_inInfo.width;j++){
			register unsigned int tmp;
			if(m_off1 >= 0){
				tmp = *p1*m_gain1;
				if(tmp > 255) tmp = 255;
				*p1++ = tmp;
			}
			if(m_off2 >= 0){
				tmp = *p2*m_gain2;
				if(tmp > 255) tmp = 255;
				*p2++ = tmp;
			}
			if(m_off3 >= 0){
				tmp = *p3*m_gain3;
				if(tmp > 255) tmp = 255;
				*p3++ = tmp;
			}
		}
	}

	return 1;
}

int __stdcall CFilter_Test::destroy(){
	return 1;
}


int filter_test_get_interface(void **ppIntf){
	if(ppIntf == NULL)
		return -1;

	*ppIntf = new CFilter_Test;
	return *ppIntf != NULL;
}



