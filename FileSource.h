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
