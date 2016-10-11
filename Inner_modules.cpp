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
#include "Inner_modules.h"

#include "Formater_Y8.h"
#include "Formater_Y16.h"
#include "Formater_I420.h"
#include "Formater_YV12.h"
#include "Formater_I422.h"
#include "Formater_YV16.h"
#include "Formater_YUYV.h"
#include "Formater_UYVY.h"
#include "Formater_YUV444.h"
#include "Formater_420SP.h"
#include "Formater_422SP.h"
#include "Formater_RGB24.h"


#include "Filter_test.h"
#include "Filter_upset.h"
#include "Filter_mirror.h"


int inner_formaters_num = 12;
yuv_formater_get_interface_func inner_formaters[] = {
	formater_y8_get_interface,
	formater_y16_get_interface,
	formater_i420_get_interface,
	formater_yv12_get_interface,
	formater_i422_get_interface,
	formater_yv16_get_interface,
	formater_yuyv_get_interface,
	formater_uyvy_get_interface,
	formater_yuv444_get_interface,
	formater_420sp_get_interface,
	formater_422sp_get_interface,
	formater_rgb24_get_interface,
};

int inner_image_filters_num = 3;
image_filter_get_interface_func inner_image_filters[] = {
	filter_test_get_interface,
	filter_upset_get_interface,
	filter_mirror_get_interface,
};






