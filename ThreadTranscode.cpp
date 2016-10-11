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

#include "stdafx.h"
#include "ThreadTranscode.h"
#include "Message.h"
#include "mFile.h"


UINT32  thread_transcode(LPVOID pArg0){
	if(pArg0 == NULL){
		return -1;
	}
	
	TranscodeArg *pArg = (TranscodeArg*)pArg0;

	int oldFramePos = -1;
	CYUVDoc *pDoc0 = pArg->pDoc;
	int frameNum = 0;
	int n;
		
	int skipNum;

	RawImage_Info rawInfo;
	unsigned char *out_buf = NULL;
	int out_bufsz;


	pArg->frameNow = 0;
	pArg->state = 1;
	
	oldFramePos = pDoc0->getFrameIndex();
	pDoc0->restart();
	pDoc0->seekFrameForward(pArg->startFrame);

	CStdioFile outFile(pArg->fileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if(outFile.m_hFile == CFile::hFileNull){
		pArg->errMsg = "打开文件失败";
		goto leave;
	}


	pArg->pRender->getRawInfo(&rawInfo);
	n = pArg->pFormater->toYourFormat(&rawInfo, NULL, 0, NULL, &out_bufsz);
	if(n <= 0 || out_bufsz <= 0){
		pArg->errMsg.Format("格式插件\"%s\"不支持此 格式转换, ret = %d",
			pArg->pFormater->getName(), n);
		goto leave;
	}
	
	out_buf = (unsigned char*)malloc(out_bufsz);
	if(out_buf == NULL){
		pArg->errMsg.Format("申请内存失败, sz: %d bytes for \"%s\"",
			out_bufsz, pArg->pFormater->getName());
		goto leave;
	}

	frameNum = 0;
	skipNum = 0;
	while(frameNum < pArg->frameNum){
		//
		if(pArg->cmd == 0){
			pArg->errMsg = "转换被取消";
			break;
		}
		
		// skip frames, use loop
		if(pArg->skipNum > 0){
			if(skipNum > 0){
				pDoc0->seekFrameForward(1);
				skipNum--;
				continue;
			}
			else
				skipNum = pArg->skipNum;
		}

		unsigned char *pBuf;
		int bufsz;
		const unsigned char *pData;
		int len;

		pArg->pRender->getBuffer(&pBuf, &bufsz);
		if(pDoc0->getFrame(pBuf, bufsz) < 0){
			pArg->errMsg = "读取数据源出错";
			goto leave;
		}
		pArg->pRender->putData(pBuf);
		pData = pArg->pRender->getRaw(&len);
		if(pData == NULL || len <= 0)
			goto leave;

		if(pArg->pFormater->toYourFormat(&rawInfo, pData, len, out_buf, &out_bufsz) < 0 || out_bufsz < 0){
			pArg->errMsg.Format("转换出错, 插件: %s",pArg->pFormater->getName());
			goto leave;
		}
		
		if(out_bufsz > 0)
			outFile.Write(out_buf, out_bufsz);
		
		pDoc0->seekFrameForward(1);
		
		frameNum++;
		pArg->frameNow = frameNum;
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,1,0);
	}
	

leave:
	outFile.Close();
	
	if(out_buf)
		free(out_buf);
	
	if(pDoc0 != NULL && oldFramePos >= 0){
		pDoc0->restart();
		pDoc0->seekFrameForward(oldFramePos);
	}

	if(frameNum < pArg->frameNum){
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,-1,0);
	}
	else{
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,2,0);
	}

	pArg->state = 0;

	return 1;
}


#include <Vfw.h>
#define FOURCC(a,b,c,d)  (a | (b<<8) | (c<<16) | (d<<24) )

static int yuv444_to_yv12(const unsigned char *src_buf, unsigned char *dst_buf, int width, int height);

UINT32  thread_transcode_avi(LPVOID pArg0){
	if(pArg0 == NULL){
		return -1;
	}
	
	TranscodeArg *pArg = (TranscodeArg*)pArg0;

	int oldFramePos = -1;
	CYUVDoc *pDoc0 = pArg->pDoc;
	int frameNum = 0;
	int n;
		
	unsigned int fcc = 0;
	int bitCount = 0;
	int frameSize = 0;
	int format = 0;

	// new
	AVISTREAMINFO streamInfo;
	PAVIFILE  pFile = NULL;
	PAVISTREAM  pStream;
	
	HRESULT hr;
	int FPS = 25;
	int width = 352;
	int height = 288;
	int pixel_num;
	
	BITMAPINFOHEADER bmpInfo;
	
	unsigned char *pFrame = NULL;
	int skipNum;
	unsigned char *out_buf = NULL;
	int out_bufsz;


	pArg->frameNow = 0;
	pArg->state = 1;
	
	oldFramePos = pDoc0->getFrameIndex();
	pDoc0->restart();
	pDoc0->seekFrameForward(pArg->startFrame);
	pixel_num = width*height;

	RawImage_Info rawInfo;
	pArg->pRender->getRawInfo(&rawInfo);
	
	width = rawInfo.width;
	height = rawInfo.height;

	if(rawInfo.raw_format == RAW_FORMAT_YUV444){
		fcc = FOURCC('Y','V','1','2');
		bitCount = 12;
		out_bufsz = yuv444_to_yv12(NULL, NULL, rawInfo.width, rawInfo.height);
		out_buf = (unsigned char*)malloc(out_bufsz);
		if(out_buf == NULL){
			pArg->errMsg = "malloc failed";
			goto leave;
		}
		frameSize = out_bufsz;
	}
	else if(rawInfo.raw_format == RAW_FORMAT_BGR24){
		fcc = 0x20424944;
		frameSize = rawInfo.frame_size;
	}
	else{
		pArg->errMsg = "暂时不支持此格式转换为AVI\r\n"
			"请先使用滤镜处理成合适的格式再做转换";
		goto leave;
	}


	AVIFileInit();
	
	hr = AVIFileOpen(&pFile,pArg->fileName,OF_WRITE | OF_CREATE,NULL);
	if(hr){
		pArg->errMsg.Format("创建文件 %s失败, 若该文件已存在, 请先删除之",
			pArg->fileName);
		goto leave;
	}

	memset(&streamInfo,0,sizeof(streamInfo));
	streamInfo.fccType = streamtypeVIDEO;
	streamInfo.fccHandler = fcc;
	streamInfo.dwScale = 1;
	streamInfo.dwRate = FPS;
	streamInfo.dwSuggestedBufferSize = frameSize;
	SetRect(&streamInfo.rcFrame, 0, 0, width, height);
	
	hr = AVIFileCreateStream(pFile,&pStream,&streamInfo);
	if(hr){
		pArg->errMsg.Format("创建avi stream 失败");
		goto leave;
	}

	memset(&bmpInfo,0,sizeof(bmpInfo));
	bmpInfo.biSize = sizeof(bmpInfo);
	bmpInfo.biWidth = width;
	bmpInfo.biHeight = height;
	bmpInfo.biBitCount = bitCount;
	bmpInfo.biCompression = fcc;
	bmpInfo.biSizeImage = frameSize;


	frameNum = 0;
	skipNum = 0;
	while(frameNum < pArg->frameNum){
		//
		if(pArg->cmd == 0){
			pArg->errMsg = "转换被取消";
			break;
		}
		
		// skip frames, use loop
		if(pArg->skipNum > 0){
			if(skipNum > 0){
				pDoc0->seekFrame(1,SEEK_CUR);
				skipNum--;
				continue;
			}
			else
				skipNum = pArg->skipNum;
		}
		
		unsigned char *pBuf;
		int bufsz;
		const unsigned char *pData;
		int len;
		
		pArg->pRender->getBuffer(&pBuf, &bufsz);
		if(pDoc0->getFrame(pBuf, bufsz) < 0){
			pArg->errMsg = "读取数据源出错";
			goto leave;
		}
		pArg->pRender->putData(pBuf);
		pData = pArg->pRender->getRaw(&len);
		if(pData == NULL || len <= 0)
			goto leave;

		if(rawInfo.raw_format == RAW_FORMAT_YUV444){
			yuv444_to_yv12(pData, out_buf, rawInfo.width, rawInfo.height);
			pFrame = out_buf;
			frameSize = out_bufsz;
		}
		else{   // BGR24
			pFrame = (unsigned char*)pData;
			frameSize = len;
		}

		hr = AVIStreamSetFormat(pStream,frameNum,&bmpInfo,sizeof(bmpInfo));
		if(hr){
			pArg->errMsg.Format("出错, 错误码: -10");
			goto leave;
		}
		
		hr = AVIStreamWrite(pStream,
							frameNum,
							1,
							pFrame,
							frameSize,
							AVIIF_KEYFRAME,
							NULL,
							NULL);
		if(hr){
			pArg->errMsg.Format("出错, 错误码: -11");
			goto leave;
		}
		
		pDoc0->seekFrameForward(1);
		
		frameNum++;
		pArg->frameNow = frameNum;
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,1,0);
	}
	
leave:
	
	if(out_buf)
		free(out_buf);

	if(pDoc0 != NULL && oldFramePos != -1){
		pDoc0->restart();
		pDoc0->seekFrameForward(oldFramePos);
	}
	
	if(pFile != NULL){
		AVIStreamClose(pStream);
		if(pFile != NULL)
			AVIFileRelease(pFile);
		AVIFileExit();
	}

	if(frameNum < pArg->frameNum){
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,-1,0);
	}
	else{
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,2,0);
	}

	pArg->state = 0;
	return 1;
}

UINT32  thread_transcode_raw(LPVOID pArg0){
	if(pArg0 == NULL){
		return -1;
	}
	
	TranscodeArg *pArg = (TranscodeArg*)pArg0;

	int oldFramePos = -1;
	CYUVDoc *pDoc0 = pArg->pDoc;
	int frameNum = 0;
	int n;
		
	int skipNum;
	
	pArg->frameNow = 0;
	pArg->state = 1;
	
	oldFramePos = pDoc0->getFrameIndex();
	pDoc0->restart();
	pDoc0->seekFrameForward(pArg->startFrame);

	CStdioFile outFile(pArg->fileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if(outFile.m_hFile == CFile::hFileNull){
		pArg->errMsg = "打开文件失败";
		goto leave;
	}

	frameNum = 0;
	skipNum = 0;
	while(frameNum < pArg->frameNum){
		//
		if(pArg->cmd == 0){
			pArg->errMsg = "转换被取消";
			break;
		}
		
		// skip frames, use loop
		if(pArg->skipNum > 0){
			if(skipNum > 0){
				pDoc0->seekFrameForward(1);
				skipNum--;
				continue;
			}
			else
				skipNum = pArg->skipNum;
		}

		unsigned char *pBuf;
		int bufsz;
		pArg->pRender->getBuffer(&pBuf, &bufsz);
		if(pDoc0->getFrame(pBuf, bufsz) < 0){
			pArg->errMsg = "读取数据源出错";
			goto leave;
		}
		pArg->pRender->putData(pBuf);

		const unsigned char *pData;
		int len;
		pData = pArg->pRender->getRaw(&len);
		if(pData == NULL || len <= 0)
			goto leave;

		outFile.Write(pData, len);
		
		pDoc0->seekFrameForward(1);
		
		frameNum++;
		pArg->frameNow = frameNum;
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,1,0);
	}
	
	outFile.Close();

leave:
	
	if(pDoc0 != NULL && oldFramePos >= 0){
		pDoc0->restart();
		pDoc0->seekFrameForward(oldFramePos);
	}

	if(frameNum < pArg->frameNum){
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,-1,0);
	}
	else{
		::PostMessage(pArg->m_hParent,WM_TRANSCODE,2,0);
	}

	pArg->state = 0;
	return 1;
}




//////////////////////////////////////////

static int yuv444_to_yv12(const unsigned char *src_buf, unsigned char *dst_buf, int width, int height){
	int y_size = width*height;
	if(y_size <= 0)
		return -1;
	if(dst_buf == NULL)
		return y_size*3/2;

	if(src_buf == NULL)
		return -2;

	const unsigned char *p1;
	unsigned char *p2;
	int i,j;
	
	// Y
	p1 = src_buf;
	p2 = dst_buf;
	memcpy(p2, p1, y_size);
	
	// U
	p1 = src_buf + y_size*2;
	p2 = dst_buf + y_size;
	for(i = 0;i < height/2;i++){
		for(j = 0;j < width/2;j++){
			*p2++ = *p1;
			p1 += 2;
		}
		p1 += width;
	}
	
	// U
	p1 = src_buf + y_size;
	p2 = dst_buf + y_size + y_size/4;
	for(i = 0;i < height/2;i++){
		for(j = 0;j < width/2;j++){
			*p2++ = *p1;
			p1 += 2;
		}
		p1 += width;
	}
	
	return 1;
}



