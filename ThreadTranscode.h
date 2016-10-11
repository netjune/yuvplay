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

#ifndef  THREADTRANSCODE_H_
#define THREADTRANSCODE_H_
#include <windows.h>
#include "yuvDoc.h"
#include "FormaterMan.h"
#include "Render.h"

class TranscodeArg{
public:
	int state;

	int cmd;
	HWND  m_hParent;
	
	CString  errMsg;

	//
	CYUVDoc *pDoc;
	CRender *pRender;
	IYUV_Formater *pFormater;

	int startFrame;
	int frameNum;
	int skipNum;
	CString  fileName;

	// statics
	int frameNow;
};

UINT32  thread_transcode_raw(LPVOID pArg0);
UINT32  thread_transcode_avi(LPVOID pArg0);
UINT32  thread_transcode(LPVOID pArg);





#endif


