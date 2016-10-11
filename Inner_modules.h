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

#ifndef INNER_MODULES_H_
#define INNER_MODULES_H_
#include "Plugin.h"


typedef int (*image_filter_get_interface_func)(void **ppIntf);
typedef int (*yuv_formater_get_interface_func)(void **ppIntf);




extern int inner_image_filters_num;
extern image_filter_get_interface_func inner_image_filters[];

extern int inner_formaters_num;
extern yuv_formater_get_interface_func inner_formaters[];




#endif



