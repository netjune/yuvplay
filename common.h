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

#ifndef COMMON_H_
#define COMMON_H_
#include "stdafx.h"


extern int myYUVToRGB(unsigned char *pYuv,unsigned char *pRgb,
				int width,int height,int yuvType,int RGBType);

extern int myRGBToYUV(unsigned char *pRgb,unsigned char *pYuv,
			   int width,int height,int rgbType,int yuvType);

extern int myGetIntFromStr(const char *str, int *pVal);

extern int myGetRangeUint(const char *s,char c,int *pmin,int *pmax,int sort);

#endif

