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

#ifndef RENDER_H_
#define RENDER_H_
#include "Plugin.h"


#define YUV_SHOW_YUV	    0x1
#define YUV_SHOW_Y		    0x2
#define YUV_SHOW_U		    0x3
#define YUV_SHOW_V		    0x4


//
//  ��������: ת��, ��ʾ, �Ƚ�
//
//
class CRender{
	RawImage_Info m_inInfo;
	RawImage_Info m_rawInfo;

	int m_frameSize;
	unsigned char *m_inBuf;
	int m_inBufSz;
	
	int m_showYUV;
	IImage_Filter **m_ppFilters;
	int m_filterNum;

	unsigned char *m_rawBuf;
	int m_rawBufSz;

	unsigned char *m_imageBuf;
	unsigned int m_imageBufSz;
	int m_linePitch_bgr24;

	BITMAPINFOHEADER m_bmpInfo;
	void *m_pBits;
	
	// pixel info
	int m_linePitch;   // for 3 compnent
	int m_step;
	int m_offs[3];
	int m_pixelFormat;
	
	//
	int m_bValid;

	CString m_errMsg;

	void _init();
	int _checkFilters(int *pErr);
	int _calcFrameSize(RawImage_Info *pInfo);
public:
	CRender();
	~CRender();

	int setShowYUV(int showYUV);
	int setFilters(IImage_Filter **ppFilters);
	int setParams(RawImage_Info *pInfo);
	int update(void *pErr = NULL);	

	int getBuffer(unsigned char **ppBuf, int *p_bufsz);
	int putData(unsigned char *pBuf);

	// �����ʽ1, ��bmp
	int getImageBmp(BITMAPINFOHEADER **ppBmpInfo, unsigned char **ppBits);
	
	int getRawInfo(RawImage_Info *pInfo);
	const unsigned char* getRaw(int *pLen);

	int getBitCount_C(int arg);
	int getPixels_Y(int x, int y, int cx, unsigned char *buf, int bufsz);
	int getPixels_U(int x, int y, int cx, unsigned char *buf, int bufsz);
	int getPixels_V(int x, int y, int cx, unsigned char *buf, int bufsz);
	
	int getPixelFormat(){ 
		if(m_pixelFormat != 2 )
			return 3;
		else
			return m_pixelFormat;
	};
	int getPixels_3x3(int x, int y, unsigned char *y_buf, unsigned char* u_buf, unsigned char *v_buf);

	int destroy();

	int yuv444_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height);
	int rgb24_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height);
	int bgr24_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height);
	int grey8_2_bgr24(unsigned char *yuv_buf, unsigned char *bgr_buf, int width, int height);

	const char *getErrMsg();

	int yuvDiff_bi_all(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int threshold);
	int yuvDiff_diff_all(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2);
	int yuvDiff_bi_comp(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int threshold, int comp_id);
	int yuvDiff_diff_comp(CRender *pref, const unsigned char *pSrc1, const unsigned char *pSrc2, int comp_id);

};



const char *getRawFormatName(int format);


// fixed point
__inline void yuv2rgb00(unsigned char y,unsigned char u,unsigned char v,
					  unsigned char *pr,unsigned char *pg,unsigned char *pb){
	__int64 bt,gt,rt;
	
	// B...
	bt = 1192*(y - 16) + 2066*(u - 128);
	if(bt >= 0)
		bt >>= 10;
	else
		bt = 0;
	// G
	gt = 1192*(y - 16) - 389*(u-128) - 833*(v - 128);
	if(gt >= 0)
		gt >>= 10;
	else
		gt = 0;
	// R
	rt = 1192*(y - 16) + 1187*(v - 128);
	if(rt >= 0)
		rt >>= 10;
	else
		rt = 0;
	
	if(bt > 255)
		bt = 255;
	if(gt > 255)
		gt = 255;
	if(rt > 255)
		rt = 255;
	
	*pr = rt;
	*pg = gt;
	*pb = bt;
}

// float point
__inline void yuv2rgb(int y,int u,int v,
					  unsigned char *pr,unsigned char *pg,unsigned char *pb){
	int r,g,b;

	u -= 128;
	v -= 128;

	r = y + 1.4075*v;
	g = y - 0.3455*u - 0.7169*v;
	b = y + 1.779*u;

	if(r > 255) r = 255;
	else if(r < 0) r = 0;
	if(g > 255) g = 255;
	else if(g < 0) g = 0;
	if(b > 255) b = 255;
	else if(b < 0) b = 0;

	*pr = r;
	*pg = g;
	*pb = b;
}



#endif



