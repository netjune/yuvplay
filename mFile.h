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

#ifndef  MFILE_H_
#define  MFILE_H_
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


typedef  __int64  int64;

typedef  int MFILE;

#define  MFILE_INVALID   -1

#define  MO_READ		_O_RDONLY
#define  MO_WRITE      _O_WRONLY
#define  MO_CREATE    _O_CREAT
#define  MO_APPEND    _O_APPEND
#define  MO_BINARY     _O_BINARY


MFILE  mOpenFile(const char *fileName, int flag);
int  mFileOK(MFILE f);

int mReadFile(MFILE f, unsigned char *pBuf, int bufSz);
int mWriteFile(MFILE f, unsigned char *pData, int len);

int mSeekFile(MFILE f, long offset, int origin);
int mGetFileSize(MFILE f);
int mGetFilePos(MFILE f);

int mCloseFile(MFILE f);


// 64 ext
int mSeekFile64(MFILE f, int64 offset, int origin);
int64 mGetFileSize64(MFILE f);
int64 mGetFilePos64(MFILE f);




#endif


