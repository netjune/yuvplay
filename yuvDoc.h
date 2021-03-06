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

#ifndef YUVDOC_H
#define YUVDOC_H

#include "stdafx.h"
#include "FileSource.h"
#include "Plugin.h"

#define YUV_FORMAT_NONE     0x0
#define YUV_FORMAT_I422		0x1     // YUV422
#define YUV_FORMAT_I420		0x2		// YUV420
#define YUV_FORMAT_YV16		0x3
#define YUV_FORMAT_YV12		0x4
#define YUV_FORMAT_Y		0x5
#define YUV_FORMAT_YUYV		0x6
#define YUV_FORMAT_UYVY		0x7
#define YUV_FORMAT_BGR24    0x20
#define YUV_FORMAT_RGB444   0x21
#define YUV_FORMAT_YUV444   0x22
#define YUV_FORMAT_YUV422SP 0x23
#define YUV_FORMAT_YUV420SP 0x24


// levels
#define YUV_READY_NONE			0x0
#define YUV_READY_SRC			0x1
#define YUV_READY_PARAMS		0x2
#define YUV_READY_YUV			0x3
#define YUV_READY_IMAGE			0x4

// src type
#define  YUV_SRC_NONE			0x0
#define  YUV_SRC_FILE			0x1
#define  YUV_SRC_MEM			0x2
#define  YUV_SRC_FILL           0x3

class CYUVDoc{
private:
	FileSource m_fileSource;
	IYUV_Formater *m_pFormater;
	int m_formaterId;

	YUV_Params m_params;
	int m_bLoop;
	CString m_fileName;
	
	CString m_errMsg;

	void _init();
public:
	CYUVDoc();
	~CYUVDoc();

	int getFormaterID();
	int setFormater(IYUV_Formater *formater, int id);

	int setParams(YUV_Params *pParams);
	int getOutFormat(RawImage_Info *pInfo);

	int setFileName(const char *fileName);
	
	int update(void *pErr = NULL);
	

	int getFrame(unsigned char *pBuf, int bufSz);

	int seekFrameForward(int frame_num);
	int seekFrameBackward(int frame_num);
	int restart();
	int seekFrame(int frame_num, int start);

	int getFrameNum();
	int getFrameIndex();

	int destroy();
	
	int isValid(){ return 1;}

	int getFileName(CString& buf){ buf = m_fileName;return 1;}

	int updateFrame();

	const char *getErrMsg();
};







#endif


