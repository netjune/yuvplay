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

#ifndef PLUGIN_H_
#define PLUGIN_H_

// use it
#define E_OK              1
#define E_BAD_ARG         -2   // 参数错误, 不合理参数
#define E_UNSUPPORTTED    -2   // 参数合理, 但自己不支持
#define E_NO_MEMORY       -3   // 申请内存失败
#define E_NO_INIT         -4   // 未初始化
#define E_OOPS            -5   // 出现了不该出现的状态
#define E_USER            -6


#define MODULE_NAME_MAX_LENGTH      64
#define MODULE_VERSION_MAX_LENGTH   64
#define MODULE_AUTHOR_MAX_LENGTH    64
#define MODULE_COMMENT_MAX_LENGTH   1024

// 模块属性信息
typedef struct Module_Info{
	char version[MODULE_VERSION_MAX_LENGTH];		// 版本号
	char author_name[MODULE_AUTHOR_MAX_LENGTH];	// 作者名字
	char comment[MODULE_COMMENT_MAX_LENGTH];		// 注释
}Module_Info;


#define FIELD_UNKNOWN         0
#define FIELD_ALL             1     // 两场都显示, 最常用
#define FIELD_TOP_ONLY        2     // 仅显示偶场, top
#define FIELD_BOTTOM_ONLY     3     // 仅显示奇场, bottom
#define FIELD_TOP_BOTTOM_SEQ  4     // 逐场显示, 先top, 然后bottom
#define FIELD_TOP_BOTTOM_SP   5     // 偶上奇下, 一幅画面


typedef struct YUV_Params{
	unsigned short width;
	unsigned short height;
	int field;
}YUV_Params;


#define RAW_FORMAT_UNKNOWN    0
#define RAW_FORMAT_YUV444     1
#define RAW_FORMAT_RGB24      2   // rgb,rgb...
#define RAW_FORMAT_BGR24      3   // bmp图片格式
#define RAW_FORMAT_GREY8      4
#define RAW_FORMAT_USER       10  // 后面为用户自定义格式


typedef struct RawImage_Info{
	int raw_format;
	int width;
	int height;
	int frame_size;
}RawImage_Info;


class IYUV_Formater{
public:
	// format name
	virtual const char *__stdcall getName() = 0;
	// other information
	virtual int __stdcall getModuleInfo(Module_Info *pInfo) = 0;
	
	// 两种配置方式
	virtual int __stdcall simpleConfig(int val, unsigned int val2, const char *str) = 0;
	virtual int __stdcall selfConfig() = 0;

	// 调试用, 可查看一些内部状态
	virtual int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz) = 0;
	
	// 设置源数据格式, width, height, field, 若不支持,请返回负值
	virtual int __stdcall setParams(YUV_Params *params) = 0;
	// 更新内部状态, 会在setParams之后紧接着调用
	virtual int __stdcall update(void *pErr = NULL) = 0;

	// 查询输出数据格式, 在out中填写自己支持的输出格式, format, width, height
	virtual int __stdcall getOutInfo(RawImage_Info *out) = 0;
	
	virtual int __stdcall getMinBlockSize() = 0;
	virtual int __stdcall getMinBlockFrameNum() = 0;
	
	// 获取buffer, buf大小应为minBlockSize
	virtual int __stdcall getBuffer(unsigned char **ppBuf) = 0;
	// 更新帧数据
	virtual int __stdcall putData(unsigned char *pBuf) = 0;
	
	virtual int __stdcall getFrameIndex() = 0;
	virtual int __stdcall setFrameIndex(int index) = 0;
	
	// offset: < 0: 上一帧, ==0: 当前帧, >0: 下一帧
	virtual int __stdcall getFrame(int offset, unsigned char *pBuf, int bufsz) = 0;  // a

	// 格式转换: 
	// 每次转换, 此函数可能会被调用两次
	// 1. 若dst_buf为NULL, 用来让外部应用查询dst_buf的大小, 请在p_dst_bufsz中返回dst_buf的大小, 外部应用会根据此大小去申请dst_buf
	// 2. 第二次调用dst_buf将为有效值, 此时可做转换, 并将转换后的有效数据字节数填入p_dst_bufsz中
	// 3. 同时, 请检查src_info, 若不支持, 请返回负值
	// 4. 此函数不要修改对象的其他成员, 不要影响到其他函数的行为即可, 尽量独立
	virtual int __stdcall toYourFormat(RawImage_Info *src_info, const unsigned char *src_buf, int src_bufsz, unsigned char *dst_buf, int *p_dst_bufsz) = 0;

	// 释放内部申请的内存, 清除状态, 
	virtual int __stdcall destroy() = 0;
};



class IImage_Filter{
public:
	// filter name
	virtual const char *__stdcall getName() = 0;
	// other information
	virtual int __stdcall getModuleInfo(Module_Info *pInfo) = 0;
	
	// 两种配置方式
	virtual int __stdcall simpleConfig(int val, unsigned int val2, const char *str) = 0;
	virtual int __stdcall selfConfig() = 0;
	
	// 调试用, 可查看一些内部状态
	virtual int __stdcall queryRuntimeInfo(char *infoBuf, int bufsz) = 0;

	// 设置参数, format, width, height
	virtual int __stdcall setParams(RawImage_Info *pInfo) = 0;
	// 更新内部状态, 紧接着setParams调用
	virtual int __stdcall update(void *pErr = NULL) = 0;

	// 查询输出数据格式, 在out中填写自己支持的输出格式, format, width, height
	virtual int __stdcall getOutInfo(RawImage_Info *out) = 0;

	// filter
	virtual int __stdcall filter(unsigned char **ppData, int *pLen) = 0;

	virtual int __stdcall destroy() = 0;
};




#endif

