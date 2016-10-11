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
#include "FileSource.h"

FileSource::FileSource(){
	m_file = MFILE_INVALID;
	m_block_size = 1;
	m_header_skip = 0;
	m_file_size = 0;
}
FileSource::~FileSource(){
	destroy();
}

int FileSource::init(const char *fn, int skip_bytes){
	destroy();
	if(fn == NULL)
		return -1;
	
	m_file = mOpenFile(fn, MO_READ|MO_BINARY);
	if(m_file == MFILE_INVALID)
		return -2;
	m_file_size = mGetFileSize64(m_file);
	if(skip_bytes < 0)
		skip_bytes = 0;
	m_header_skip = skip_bytes;
	mSeekFile64(m_file, m_header_skip, SEEK_SET);
	return 1;
}

int FileSource::setBlockSize(int block_size){
	if(block_size <= 0)
		return -1;
	m_block_size = block_size;
	return 1;
}
int FileSource::getBlockNum(){
	return (int)((m_file_size - m_header_skip)/(int64)m_block_size);
}


int FileSource::seekForward(int block_num){
	int64 offset = block_num*m_block_size;
	int64 pos = mGetFilePos64(m_file);
	if(offset + pos + m_header_skip > m_file_size - m_block_size){
		int64 fsz = mGetFileSize64(m_file);
		if(offset + pos + m_header_skip > fsz - m_block_size)
			return 0;  // 文件结束
		m_file_size = fsz;
	}
	mSeekFile64(m_file, offset, SEEK_CUR);
	return 1;
}

int FileSource::seekBackward(int block_num){
	int64 offset = block_num*m_block_size;
	int64 pos = mGetFilePos64(m_file);
	if(offset > pos - m_header_skip)
		return 0;  // 文件结束
	offset = -offset;
	return (mSeekFile64(m_file, offset, SEEK_CUR) >= 0);
}

int FileSource::restart(){
	return mSeekFile64(m_file, m_header_skip, SEEK_SET);
}
int FileSource::getBlockIndex(){
	return (mGetFilePos64(m_file) - m_header_skip)/m_block_size;
}
int FileSource::fillData(unsigned char *buf, int *p_bufsz){
	if(buf == NULL || p_bufsz == NULL || *p_bufsz <= 0)
		return -1;
	*p_bufsz = mReadFile(m_file, buf, *p_bufsz);
	if(*p_bufsz > 0){
		mSeekFile64(m_file, -*p_bufsz, SEEK_CUR);
		return 1;
	}
	return -2;
}

int FileSource::destroy(){
	if(m_file != MFILE_INVALID){
		mCloseFile(m_file);
		m_file = MFILE_INVALID;
	}
	m_file = MFILE_INVALID;
	m_block_size = 1;
	m_header_skip = 0;
	m_file_size = 0;
	return 1;
}

