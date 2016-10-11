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

#ifndef FILESOURCE_H_
#define FILESOURCE_H_
#include "mFile.h"

class FileSource{
	MFILE m_file;
	int m_block_size;
	int m_header_skip;
	int64 m_file_size;

public:
	FileSource();
	~FileSource();

	int init(const char *fn, int skip_bytes);
	
	int setBlockSize(int block_size);
	int getBlockNum();
	

	int seekForward(int block_num);
	
	int seekBackward(int block_num);

	int restart();
	int getBlockIndex();
	int fillData(unsigned char *buf, int *p_bufsz);

	int destroy();
};








#endif
