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

#include "stdafx.h"
#include "mFile.h"
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


MFILE  mOpenFile(const char *fileName, int flag){
	int fd;
	
	if(fileName == NULL)
		return -1;
	
	if((flag & _O_WRONLY)){
		if( (flag & _O_APPEND) == 0 )
			flag |= _O_TRUNC;
		fd = _open(fileName,flag,_S_IREAD |_S_IWRITE);
	}
	else
		fd = _open(fileName,flag);

	return fd;
}

int  mFileOK(MFILE f){
	return f >= 0;
}

int mReadFile(MFILE f, unsigned char *pBuf, int bufSz){
	if(pBuf == NULL || bufSz <= 0)
		return -1;

	return _read(f,pBuf,bufSz);
}

int mWriteFile(MFILE f, unsigned char *pData, int len){
	if(pData == NULL || len <= 0)
		return -1;

	return _write(f,pData,len);
}

int mSeekFile(MFILE f, long offset, int origin){
	return _lseek(f,offset,origin);
}

int mGetFileSize(MFILE f){
	long oldPos;

	oldPos = _tell(f);
	if(oldPos < 0)
		return oldPos;

	int fsize;
	_lseek(f,0,SEEK_END);
	fsize = _tell(f);
	_lseek(f,oldPos,SEEK_SET);

	return fsize;
}

int mGetFilePos(MFILE f){
	return _tell(f);
}

int mCloseFile(MFILE f){
	return _close(f);
}


// 64 ext
int mSeekFile64(MFILE f, int64 offset, int origin){
	return _lseeki64(f,offset,origin);
}
int64 mGetFileSize64(MFILE f){
	int64 oldPos;
	
	oldPos = _telli64(f);
	if(oldPos < 0)
		return oldPos;
	
	int64 fsize;
	_lseeki64(f,0,SEEK_END);
	fsize = _telli64(f);
	_lseeki64(f,oldPos,SEEK_SET);
	
	return fsize;
}
int64 mGetFilePos64(MFILE f){
	return _telli64(f);
}


