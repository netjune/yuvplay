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


