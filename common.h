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

