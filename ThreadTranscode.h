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


