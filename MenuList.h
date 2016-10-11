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

#ifndef MENULIST_H_
#define MENULIST_H_

#define  IDM_BK_ADD             2001
#define  IDM_BK_MAN             2002

#define  IDM_RECENT_NEW       2999
#define  IDM_RECENT_OFFSET    3000
#define  IDM_RECENT_SIZE        10

#define  IDM_FAVOR_OFFSET     4000
#define  IDM_FAVOR_SIZE          20

#define  CONF_FILE             "yuvplay.ini"
#define  BOOKMARK_FILE         "yuvplay.ini"

#define  RECENT_STRING         "[Recent files]"
#define  FAVOR_STRING          "[Bookmark]"
#define  RESOLUTIONS_STRING           "[Resolution presets]"
#define  LAST_STATE_STRING            "[Last state]"
#define  FILTERS_STRING         "[filters]"
#define  APP_STRING             "[yuvplayer]"


#define LINESZ    1024



class MenuItem{
public:
	MenuItem *next;
	int id;
	CString name;
};

class MenuList{
private:
	int m_size;
	int m_num;
	MenuItem *m_pItem;
	MenuItem *m_pLast;  // last valid

	int freeList();

public:
	MenuList();
	~MenuList();
	int create(int size, int startId);
	int appendItem(LPCTSTR name, int& id);
	int insertItem(int pos, LPCTSTR name, int& id);
	int deleteItem(int id);
	int deleteItem(LPCTSTR name);
	int getItemName(int id, CString& name);
	int getItemId(LPCTSTR name, int& id);
	int getItem(int index, int &id, CString& name);

	int isFull();
	int isEmpty();
	
	int destroy();
};











#endif






