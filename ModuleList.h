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

#ifndef MODULELIST_H_
#define MODULELIST_H_


class CListItem;

class CListItem{
	CListItem *m_next;
	void *m_item;
	int m_id;
	int m_flag;

public:
	int setNext(CListItem *pItem);
	int setData(void *data);
	int setId(int id);
	int setFlag(int flag);

	CListItem* getNext();
	void* getData();
	int getId();
	int getFlag();
};


class CModuleList{
public:
	int append(void *item);

};




#endif


