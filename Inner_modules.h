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



