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
#include "Render.h"


void CRender::_init(){
	m_inBuf = NULL;
	m_inBufSz = 0;

	m_frameSize = 0;
	m_showYUV = YUV_SHOW_YUV;

	m_ppFilters = NULL;

	m_rawBuf = NULL;
	m_rawBufSz = 0;
	m_imageBuf = NULL;
	m_imageBufSz = 0;

	m_rawInfo.raw_format = RAW_FORMAT_UNKNOWN;

	m_bValid = 0;
}
CRender::CRender(){
	_init();
}

CRender::~CRender(){
	destroy();
}

int CRender::_checkFilters(int *pErr){
	RawImage_Info last_info = m_inInfo;
	IImage_Filter *pFilter;
	int i;
	int num = 0;
	
	for(i = 0;;i++){
		if(m_ppFilters[i] == NULL)
			break;
		
		int ret;
		
		ret = m_ppFilters[i]->setParams(&last_info);
		if(ret < 0){
			if(pErr) *pErr = i;
			m_errMsg.Format("image filter \"%s\" doesn't support the given params, %d",
				m_ppFilters[i]->getName(), ret);
			return -2;
		}
		
		int err;
		ret = m_ppFilters[i]->update(&err);
		if(ret < 0){
			if(pErr) *pErr = i;
			m_errMsg.Format("image filter \"%s\" update failed, %d",
				m_ppFilters[i]->getName(), ret);
			return -3;
		}

		m_ppFilters[i]->getOutInfo(&last_info);
		num++;
	}

	m_rawInfo = last_info;
	m_filterNum = num;
	return num;
}

// 1
int CRender::_calcFrameSize(RawImage_Info *pInfo){
	if(pInfo == NULL)
		return -1;
	if(pInfo->width <= 0 || pInfo->height <= 0){
		m_errMsg.Format("bad resolution: %dx%d",pInfo->width, pInfo->height);
		return -2;
	}

	if(pInfo->raw_format == RAW_FORMAT_YUV444){
		pInfo->frame_size = pInfo->width*pInfo->height*3;

		// pixel info
		m_linePitch = pInfo->width;
		m_offs[0] = 0;
		m_offs[1] = pInfo->frame_size/3;
		m_offs[2] = pInfo->frame_size/3*2;
		m_step = 1;

		m_pixelFormat = 0;
		return 1;
	}
	else if(pInfo->raw_format == RAW_FORMAT_RGB24){
		pInfo->frame_size = pInfo->width*pInfo->height*3;
		
		// pixel info
		m_linePitch = pInfo->width*3;
		m_offs[0] = 0;
		m_offs[1] = 1;
		m_offs[2] = 2;
		m_step = 3;

		m_pixelFormat = 1;
		return 1;
	}
	else if(pInfo->raw_format == RAW_FORMAT_BGR24){
		int tmp = pInfo->width*3;
		while(tmp&0x3) tmp++;
		pInfo->frame_size = tmp*pInfo->height;

		// pixel info
		m_linePitch = tmp;
		m_offs[0] = 2;
		m_offs[1] = 1;
		m_offs[2] = 0;
		m_step = 3;

		m_pixelFormat = 1;
		return 1;
	}
	else if(pInfo->raw_format == RAW_FORMAT_GREY8){
		pInfo->frame_size = pInfo->width*pInfo->height;

		m_linePitch = pInfo->width;
		m_offs[0] = 0;
		m_offs[1] = -1;
		m_offs[2] = -1;
		m_step = 1;

		m_pixelFormat = 2;
		return 1;
	}
	else if(pInfo->frame_size > 0){
		return 0;
	}
	
	m_errMsg = "a custom format need valid frame size";
	return -3;
}

int CRender::setShowYUV(int showYUV){
	if(showYUV != YUV_SHOW_YUV 
		&& showYUV != YUV_SHOW_Y 
		&& showYUV != YUV_SHOW_U 
		&& showYUV != YUV_SHOW_V)
		return -1;
	
	m_showYUV = showYUV;
	return 1;
}

int CRender::setParams(RawImage_Info *pInfo){
	if(pInfo == NULL)
		return -1;
	m_inInfo = *pInfo;
	return 1;
}

int CRender::setFilters(IImage_Filter **ppFilters){
	m_ppFilters = ppFilters;
	return 1;
}

int CRender::update(void *pErr){
	int format = _calcFrameSize(&m_inInfo);

	if(format < 0)
		return -2;
	
	if(m_ppFilters){
		int err;
		if(_checkFilters(&err) < 0){
			return -2;
		}
	}
	else{
		m_filterNum = 0;
		m_rawInfo = m_inInfo;
	}

	if(format == 0 && m_filterNum <= 0){
		m_errMsg.Format("unsupportted format from formater plugin, maybe you need image filter, format id: %d(0x%X)",
			m_inInfo.raw_format, m_inInfo.raw_format);
		return -2;
	}

	if(_calcFrameSize(&m_rawInfo) <= 0){  // 未知类型
		return -4;
	}

	// inbuf size
	m_frameSize = m_inInfo.frame_size;
	if(m_inBufSz < m_frameSize){
		if(m_inBuf) 
			free(m_inBuf);
		m_inBuf = (unsigned char*)malloc(m_frameSize);
		if(m_inBuf == NULL){
			m_inBufSz = 0;
			return -5;
		}
		m_inBufSz = m_frameSize;
	}
	// extra
	m_rawBuf = m_inBuf;
	m_rawBufSz = m_inBufSz;

	// imagebuf size
	m_linePitch_bgr24 = m_rawInfo.width*3;
	while(m_linePitch_bgr24&0x3) m_linePitch_bgr24++;
	int tmp = m_linePitch_bgr24*m_rawInfo.height;
	if(m_imageBufSz < tmp){
		if(m_imageBuf)
			free(m_imageBuf);
		m_imageBufSz = 0;
		m_imageBuf = (unsigned char*)malloc(tmp);
		if(m_imageBuf == NULL)
			return -4;
		m_imageBufSz = tmp;
	}
	
	memset(&m_bmpInfo,0,sizeof(m_bmpInfo));
	m_bmpInfo.biSize = 40;
	m_bmpInfo.biWidth = m_rawInfo.width;
	m_bmpInfo.biHeight = m_rawInfo.height;
	m_bmpInfo.biBitCount = 24;
	m_bmpInfo.biSizeImage = m_imageBufSz;
	m_bmpInfo.biPlanes = 1;
	m_bmpInfo.biCompression = BI_RGB;
	
//	m_skipRending = 0;
	return 1;
}

int CRender::getBuffer(unsigned char **ppBuf, int *p_bufsz){
	if(ppBuf == NULL)
		return -1;
	*ppBuf = m_inBuf;
	if(p_bufsz) *p_bufsz = m_inBufSz;

	m_bValid = 0;
	return 1;
}

int CRender::putData(unsigned char *pBuf){
	if(pBuf != m_inBuf)
		return -1;

	if(m_filterNum > 0){
		unsigned char *pBuf = m_inBuf;
		int bufsz = m_frameSize;
		int i;
		int ret;
		for(i = 0;;i++){
			if(m_ppFilters[i] == NULL)
				break;
			ret = m_ppFilters[i]->filter(&pBuf,&bufsz);
			if(ret < 0){
				m_errMsg.Format("image filter failed, name: %s, ret: %d",
					m_ppFilters[i]->getName(), ret);	
				return -2;
			}
		}
		m_rawBuf = pBuf;
		m_rawBufSz = bufsz;
	}
	else{
		m_rawBuf = m_inBuf;
		m_rawBufSz = m_inBufSz;
	}

	if(m_rawInfo.raw_format == RAW_FORMAT_YUV444){
		if(m_showYUV == YUV_SHOW_YUV)
			yuv444_2_bgr24(m_rawBuf, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
		else if(m_showYUV == YUV_SHOW_Y)
			grey8_2_bgr24(m_rawBuf, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
		else if(m_showYUV == YUV_SHOW_U)
			grey8_2_bgr24(m_rawBuf + m_frameSize/3, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
		else if(m_showYUV == YUV_SHOW_V)
			grey8_2_bgr24(m_rawBuf + m_frameSize/3*2, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
	}
	else if(m_rawInfo.raw_format == RAW_FORMAT_BGR24){
		bgr24_2_bgr24(m_rawBuf, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
	}
	else if(m_rawInfo.raw_format == RAW_FORMAT_RGB24){
		rgb24_2_bgr24(m_rawBuf, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
	}
	else if(m_rawInfo.raw_format == RAW_FORMAT_GREY8){
		grey8_2_bgr24(m_rawBuf, m_imageBuf, m_rawInfo.width, m_rawInfo.height);
	}
	else
		return -3;

	m_bValid = 1;
	return 1;
}

// 输出格式1, 类bmp
int CRender::getImageBmp(BITMAPINFOHEADER **ppBmpInfo, unsigned char **ppBits){
	if(m_bValid == 0) 
		return -3;

	if(ppBmpInfo == NULL || ppBits == NULL)
		return -1;
	if(m_frameSize < 0 || m_inBufSz < m_frameSize)
		return -2;

	*ppBmpInfo = &m_bmpInfo;
	*ppBits = m_imageBuf;
	return 1;
}

int CRender::getRawInfo(RawImage_Info *pInfo){
	if(pInfo == NULL || m_rawInfo.raw_format == RAW_FORMAT_UNKNOWN)
		return -1;
	*pInfo = m_rawInfo;
	return 1;
}

const unsigned char* CRender::getRaw(int *pLen){
	if(m_bValid == 0)
		return NULL;

	if(pLen) *pLen = m_rawBufSz;
	return m_rawBuf;
}


int CRender::getBitCount_C(int arg){
	if(m_bValid == 0)
		return -3;
	return 8;
}
int CRender::getPixels_Y(int x, int y, int cx, unsigned char *buf, int bufsz){
	if(m_bValid == 0)
		return -3;

	if(m_offs[0] < 0)
		return 0;
	if(x < 0 || y < 0 || cx <= 0 || buf == NULL || bufsz <= 0)
		return -1;
	if(x >= m_rawInfo.width || y >= m_rawInfo.height)
		return -1;
	if(cx > m_rawInfo.width - x)
		cx = m_rawInfo.width - x;
	if(cx <= 0 || bufsz < cx)
		return -1;

	unsigned char *p = m_rawBuf;
	p += m_offs[0] + y*m_linePitch + x*m_step;

	for(int i = 0;i < cx;i++){
		buf[i] = *p;
		p += m_step;
	}

	return cx;
}
int CRender::getPixels_U(int x, int y, int cx, unsigned char *buf, int bufsz){
	if(m_bValid == 0)
		return -3;
	
	if(m_offs[1] < 0)
		return 0;
	if(x < 0 || y < 0 || cx <= 0 || buf == NULL || bufsz <= 0)
		return -1;
	if(x >= m_rawInfo.width || y >= m_rawInfo.height)
		return -1;
	if(cx > m_rawInfo.width - x)
		cx = m_rawInfo.width - x;
	if(cx <= 0 || bufsz < cx)
		return -1;
	
	unsigned char *p = m_rawBuf;
	p += m_offs[1] + y*m_linePitch + x*m_step;
	
	for(int i = 0;i < cx;i++){
		buf[i] = *p;
		p += m_step;
	}
	
	return cx;
}
int CRender::getPixels_V(int x, int y, int cx, unsigned char *buf, int bufsz){
	if(m_bValid == 0)
		return -3;

	if(m_offs[2] < 0)
		return 0;
	if(x < 0 || y < 0 || cx <= 0 || buf == NULL || bufsz <= 0)
		return -1;
	if(x >= m_rawInfo.width || y >= m_rawInfo.height)
		return -1;
	if(cx > m_rawInfo.width - x)
		cx = m_rawInfo.width - x;
	if(cx <= 0 || bufsz < cx)
		return -1;
	
	unsigned char *p = m_rawBuf;
	p += m_offs[2] + y*m_linePitch + x*m_step;
	
	for(int i = 0;i < cx;i++){
		buf[i] = *p;
		p += m_step;
	}
	
	return cx;
}

int CRender::getPixels_3x3(int x, int y, unsigned char *y_buf, unsigned char* u_buf, unsigned char *v_buf){
	if(m_bValid == 0)
		return -3;

	if(x < 0 || y < 0 || y_buf == NULL || u_buf == NULL || v_buf == NULL)
		return -1;
	
	if(x <1 || y < 1 ||x >= m_rawInfo.width - 1 || y >= m_rawInfo.height -1){
		memset(y_buf, 0, 9);
		memset(u_buf, 0, 9);
		memset(v_buf, 0, 9);
		return m_pixelFormat;
	}

	int offset = (y-1)*m_linePitch + (x-1)*m_step;
	int line_pitch = m_linePitch - m_step*3;
	if(m_offs[0] >= 0 && 1){
		unsigned char *p = m_rawBuf + offset + m_offs[0];
		for(int i=0;i < 9;i++){
			y_buf[i] = p[0];
			p += m_step;
			if(i == 2 || i == 5){
				p += m_linePitch;
			}
		}
	}
	else{
		memset(y_buf, 0, 9);
	}

	if(m_offs[1] >= 0 && 1){
		unsigned char *p = m_rawBuf + offset + m_offs[1];
		for(int i=0;i < 9;i++){
			u_buf[i] = p[0];
			p += m_step;
			if(i == 2 || i == 5){
				p += m_linePitch;
			}
		}
	}
	else{
		memset(u_buf, 0, 9);
	}

	if(m_offs[2] >= 0 && 1){
		unsigned char *p = m_rawBuf + offset + m_offs[2];
		for(int i=0;i < 9;i++){
			v_buf[i] = p[0];
			p += m_step;
			if(i == 2 || i == 5){
				p += m_linePitch;
			}
		}
	}
	else{
		memset(v_buf, 0, 9);
	}
	
	return m_pixelFormat;
}

int CRender::destroy(){
	if(m_inBuf)
		free(m_inBuf);
	if(m_imageBuf)
		free(m_imageBuf);
	
	_init();
	return 0;
}



int CRender::yuv444_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height){
	int i,j;
	int linePitch_bgr;
	unsigned char *pY,*pU,*pV;
	unsigned char *pB;
	
	i = width*height;
	pY = yuv_buf;
	pU = pY + i;
	pV = pU + i;

	i = width*3;
	linePitch_bgr = i;
	while(linePitch_bgr&3) linePitch_bgr++;
	pB = bgr_buf;
	pB += linePitch_bgr*(height-1);
	linePitch_bgr += i;

	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			yuv2rgb(*pY,*pU,*pV,pB+2,pB+1,pB);
			pY++;
			pU++;
			pV++;
			pB += 3;
		}
		pB -= linePitch_bgr;
		
	}	
	return 1;
}

int CRender::grey8_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height){
	int i,j;
	int linePitch_bgr;
	unsigned char *pY,*pU,*pV;
	unsigned char *pB;
	
	pY = yuv_buf;
	
	i = width*3;
	linePitch_bgr = i;
	while(linePitch_bgr&3) linePitch_bgr++;
	pB = bgr_buf;
	pB += linePitch_bgr*(height-1);
	linePitch_bgr += i;
	
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			*pB++ = *pY;
			*pB++ = *pY;
			*pB++ = *pY++;
		}
		pB -= linePitch_bgr;
	}
	return 1;	
}
int CRender::rgb24_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height){
	int i,j;
	int linePitch_bgr;
	unsigned char *p1,*p2;
	
	p1 = yuv_buf;

	i = width*3;
	linePitch_bgr = i;
	while(linePitch_bgr&3) linePitch_bgr++;
	p2 = bgr_buf;
	p2 += linePitch_bgr*(height-1);
	linePitch_bgr += i;
	
	for(i = 0;i < height;i++){
		for(j = 0;j < width;j++){
			p2[0] = p1[2];
			p2[1] = p1[1];
			p2[2] = p1[0];

			p1 += 3;
			p2 += 3;
		}
		p2 -= linePitch_bgr;
	}
	return 1;
}
int CRender::bgr24_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height){
	int frame_size = width*3;
	while(frame_size&0x3) frame_size++;
	frame_size *= height;
	
	memcpy(bgr_buf, yuv_buf, frame_size);
	return 1;
}

const char *CRender::getErrMsg(){
	if(m_errMsg.IsEmpty())
		return "unexpected error";
	return m_errMsg;
}


int CRender::yuvDiff_diff_all(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2){
	int diff_num = 0;
	unsigned char *p4 = m_rawBuf;
	RawImage_Info info;

	pref->getRawInfo(&info);
		
	if(info.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		int line_extra;
		int i,j;
		
		if(info.raw_format == RAW_FORMAT_BGR24){
			i = info.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;
		
		for(i = 0;i < 3;i++){
			p1[i] = pSrc1 + pref->m_offs[i];
			p2[i] = pSrc2 + pref->m_offs[i];
		}
		
		for(i = 0;i < info.height;i++){
			for(j = 0;j < info.width;j++){
				int diff_val = 0;
				int k;
				for(k = 0;k < 3;k++){
					register int tmp;
					tmp = *p1[k] - *p2[k];
					if(tmp < 0) tmp = -tmp;
					diff_val += tmp;
				}
				
				// meanning is changed
				if(diff_val > 255) diff_val = 255;
				
				// r
				*p4++ = diff_val;
				
				diff_num += (diff_val>0);
				
				for(k = 0;k < 3;k++){
					p1[k] += pref->m_step;
					p2[k] += pref->m_step;
				}
			}
			
			if(line_extra > 0){
				for(int k = 0;k < 3;k++){
					p1[k] += line_extra;
					p2[k] += line_extra;
				}
			}
			
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = info.width*info.height;
		int i;
		int width = info.width;
		
		p1 = pSrc1;
		p2 = pSrc2;
		i = 0;
		while(i++ < num){
			int diff_val;
			diff_val = *p1 - *p2;
			if(diff_val < 0) diff_val = -diff_val;
			if(diff_val > 255) diff_val = 255;
			
			*p4++ = diff_val;
			
			diff_num += (diff_val>0);
			
			p1++;
			p2++;
		}
	}
	
	return diff_num;
}

int CRender::yuvDiff_bi_all(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int threshold){
	int diff_num = 0;
	unsigned char *p4 = m_rawBuf;
	RawImage_Info info;
	
	pref->getRawInfo(&info);

	if(info.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		int line_extra;
		int i,j;
		
		if(info.raw_format == RAW_FORMAT_BGR24){
			i = info.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;

		for(i = 0;i < 3;i++){
			p1[i] = pSrc1 + pref->m_offs[i];
			p2[i] = pSrc2 + pref->m_offs[i];
		}
		
		for(i = 0;i < info.height;i++){
			for(j = 0;j < info.width;j++){
				int diff_val = 0;
				int k;
				for(k = 0;k < 3;k++){
					register int tmp;
					tmp = *p1[k] - *p2[k];
					if(tmp < 0) tmp = -tmp;
					tmp -= threshold;
					if(tmp < 0) tmp = 0;
					diff_val += tmp;
				}

				// meanning is changed
				if(diff_val > 0) diff_val = 1;
				
				// r
				*p4++ = 0x100 - diff_val;
				
				diff_num += diff_val;
				
				for(k = 0;k < 3;k++){
					p1[k] += pref->m_step;
					p2[k] += pref->m_step;
				}
			}
			
			if(line_extra > 0){
				for(int k = 0;k < 3;k++){
					p1[k] += line_extra;
					p2[k] += line_extra;
				}
			}
			
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = info.width*info.height;
		int i;
		int width = info.width;
		
		p1 = pSrc1;
		p2 = pSrc2;
		i = 0;
		while(i++ < num){
			int diff_val;
			diff_val = *p1 - *p2;
			if(diff_val < 0) diff_val = -diff_val;
			diff_val -= threshold;
			if(diff_val <= 0) diff_val = 0;
			else diff_val = 1;

			*p4++ = 0x100 - diff_val;
			
			diff_num += diff_val;
			
			p1++;
			p2++;
		}
	}

	return diff_num;
}

int CRender::yuvDiff_diff_comp(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int comp_id){
	int diff_num = 0;
	unsigned char *p4 = m_rawBuf;
	int index = comp_id;
	
	if(index > 2)
		return -1;

	RawImage_Info info;
	
	pref->getRawInfo(&info);

	
	if(info.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		int line_extra;
		int i,j;
		
		if(info.raw_format == RAW_FORMAT_BGR24){
			i = info.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;
		
		p1[index] = pSrc1 + pref->m_offs[index];
		p2[index] = pSrc2 + pref->m_offs[index];
		
		
		for(i = 0;i < info.height;i++){
			for(j = 0;j < info.width;j++){
				int diff_val = 0;
				register int tmp;

				tmp = *p1[index] - *p2[index];
				if(tmp < 0) tmp = -tmp;
				diff_val += tmp;
				
				// meanning is changed
				if(diff_val > 255) diff_val = 255;
				
				// r
				*p4++ = diff_val;
				
				diff_num += (diff_val>0);
				
				p1[index] += pref->m_step;
				p2[index] += pref->m_step;
				
			}
			
			if(line_extra > 0){
				p1[index] += line_extra;
				p2[index] += line_extra;
			}
			
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = info.width*info.height;
		int i;
		int width = info.width;
		
		p1 = pSrc1;
		p2 = pSrc2;
		i = 0;
		while(i++ < num){
			int diff_val;
			diff_val = *p1 - *p2;
			if(diff_val < 0) diff_val = -diff_val;
			if(diff_val > 255) diff_val = 255;
			
			*p4++ = diff_val;
			
			diff_num += (diff_val>0);
			
			p1++;
			p2++;
		}
	}
	
	return diff_num;
}

int CRender::yuvDiff_bi_comp(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int threshold, int comp_id){
	int diff_num = 0;
	unsigned char *p4 = m_rawBuf;
	int index = comp_id;
	RawImage_Info info;
	
	pref->getRawInfo(&info);
	if(index > 2)
		return -1;

	if(info.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		int line_extra;
		int i,j;
		
		if(info.raw_format == RAW_FORMAT_BGR24){
			i = info.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;

		p1[index] = pSrc1 + pref->m_offs[index];
		p2[index] = pSrc2 + pref->m_offs[index];
		
		for(i = 0;i < info.height;i++){
			for(j = 0;j < info.width;j++){
				int diff_val = 0;
				register int tmp;

				tmp = *p1[index] - *p2[index];
				if(tmp < 0) tmp = -tmp;
				tmp -= threshold;
				if(tmp < 0) tmp = 0;
				diff_val += tmp;


				// meanning is changed
				if(diff_val > 0) diff_val = 1;
				
				// r
				*p4++ = 0x100 - diff_val;
				
				diff_num += diff_val;
				
				p1[index] += pref->m_step;
				p2[index] += pref->m_step;
			}
			
			if(line_extra > 0){
				p1[index] += line_extra;
				p2[index] += line_extra;
			}
			
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = info.width*info.height;
		int i;
		int width = info.width;
		
		p1 = pSrc1;
		p2 = pSrc2;
		i = 0;
		while(i++ < num){
			int diff_val;
			diff_val = *p1 - *p2;
			if(diff_val < 0) diff_val = -diff_val;
			diff_val -= threshold;
			if(diff_val <= 0) diff_val = 0;
			else diff_val = 1;

			*p4++ = 0x100 - diff_val;
			
			diff_num += diff_val;
			
			p1++;
			p2++;
		}
	}

	return diff_num;
}



const char *getRawFormatName(int format){
	if(format == RAW_FORMAT_YUV444)
		return "YUV444";
	else if(format == RAW_FORMAT_RGB24)
		return "RGB24";
	else if(format == RAW_FORMAT_BGR24)
		return "BGR24";
	else if(format == RAW_FORMAT_GREY8)
		return "GREY8";
	else
		return "unknown format";
}

/*
int CRender::yuvDiff_bi(const unsigned char *pSrc, const unsigned char *pDst, unsigned char *pRes, int threshold){
	int diff_num = 0;
	if(m_rawInfo.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		unsigned char *p3;
		unsigned char *p4;
		int line_extra;
		int i,j;
		int line_pitch_bgr24;

		line_pitch_bgr24 = m_linePitch_bgr24;
		line_pitch_bgr24 += m_rawInfo.width*3;

		if(m_rawInfo.raw_format == RAW_FORMAT_BGR24){
			i = m_rawInfo.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;

		for(i = 0;i < 3;i++){
			p1[i] = pSrc + m_offs[i];
			p2[i] = pDst + m_offs[i];
		}
		p3 = pRes;
		p4 = m_imageBuf;

		for(i = 0;i < m_rawInfo.height;i++){
			for(j = 0;j < m_rawInfo.width;j++){
				if(threshold <= 0){
					if(*p1[0] != *p2[0] || *p1[1] != *p2[1] || *p1[2] != *p2[2]){
						*p3++ = 255;
						diff_num++;
					}
					else{
						*p3++ = 0;
					}
				}
				else{
					int k;
					*p3 = 0;
					for(k = 0;k < 3;k++){
						register int tmp;
						tmp = *p1[k] - *p2[k];
						if(tmp < 0) tmp = -tmp;
						if(tmp > threshold){
							*p3++ = 255;
							diff_num++;
							break;
						}
					}
				}

				p1[0] += m_step;
				p1[1] += m_step;
				p1[2] += m_step;
				p2[0] += m_step;
				p2[1] += m_step;
				p2[2] += m_step;
			}

			if(line_extra > 0){
				p1[0] += line_extra;
				p1[1] += line_extra;
				p1[2] += line_extra;
				p2[0] += line_extra;
				p2[1] += line_extra;
				p2[2] += line_extra;
			}
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = m_rawInfo.width*m_rawInfo.height;

		p1 = pSrc;
		p2 = pDst;
		p3 = pRes;
		while(num-- > 0){
			*p3 = 0;
			if(threshold <= 0){
				if(*p1 != *p2){
					*p3 = 255;
					diff_num++;
				}
			}
			else{
				int tmp = *p1 - *p2;
				if(tmp < 0) tmp = -tmp;
				if(tmp > threshold){
					*p3 = 255;
					diff_num++;
				}
			}

			p1++;
			p2++;
			p3++;
		}
	}

	return diff_num;
}


int CRender::yuvDiff_diff(const unsigned char *pSrc, const unsigned char *pDst, unsigned char *pRes){
	int diff_num = 0;
	if(m_rawInfo.raw_format != RAW_FORMAT_GREY8){
		const unsigned char *p1[3],*p2[3];
		unsigned char *p3;
		unsigned char *p4;
		int line_extra;
		int i,j;
		
		if(m_rawInfo.raw_format == RAW_FORMAT_BGR24){
			i = m_rawInfo.width*3;
			line_extra = i;
			while(line_extra&0x3) line_extra++;
			line_extra -= i;
		}
		else
			line_extra = 0;

		for(i = 0;i < 3;i++){
			p1[i] = pSrc + m_offs[i];
			p2[i] = pDst + m_offs[i];
		}
		p3 = pRes;
		p4 = m_imageBuf;

		for(i = 0;i < m_rawInfo.height;i++){
			for(j = 0;j < m_rawInfo.width;j++){
				int diff_val = 0;
				int k;
				for(k = 0;k < 3;k++){
					register int tmp;
					tmp = *p1[k] - *p2[k];
					if(tmp < 0) tmp = -tmp;
					diff_val += tmp;
				}
				if(diff_val > 255) diff_val = 255;
				*p3++ = diff_val;
				diff_num += (diff_val > 0);
				
				p1[0] += m_step;
				p1[1] += m_step;
				p1[2] += m_step;
				p2[0] += m_step;
				p2[1] += m_step;
				p2[2] += m_step;
			}
			
			if(line_extra > 0){
				p1[0] += line_extra;
				p1[1] += line_extra;
				p1[2] += line_extra;
				p2[0] += line_extra;
				p2[1] += line_extra;
				p2[2] += line_extra;
			}
		}
	}
	else{  // gray8
		const unsigned char *p1,*p2;
		unsigned char *p3;
		int num = m_rawInfo.width*m_rawInfo.height;

		p1 = pSrc;
		p2 = pDst;
		p3 = pRes;
		while(num-- > 0){
			int diff_val;
			diff_val = *p1 - *p2;
			if(diff_val > 255) diff_val = 255;
			*p3 = diff_val;
			diff_num += (diff_val > 0);
			
			p1++;
			p2++;
			p3++;
		}
	}
	
	return diff_num;
}

*/