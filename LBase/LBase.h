#ifndef _L_BASE_H_
#define _L_BASE_H_

#include <cctype>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <sstream>
#include <algorithm>


#define  _WIN32_WINNT  0x0600


#include <boost/thread.hpp>
#include <boost/asio.hpp>
//#include <boost/make_shared.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/lexical_cast.hpp>

#include <mysql.h>
//#include <mysql/mysql.h>

//#include <msgpack/msgpack.hpp>
#include <msgpack.hpp>


#if _LINUX
#define sprintf_s snprintf
#define gmtime_s gmtime_r
#include <json/json.h>
#else
#include <jsoncpp/json.h>
#endif

#include "memory"


//#include <Windows.h>


//基本的整数类型如下
//char(8) short(16) int(32) float(32) double(64)
//
//
#if _LINUX
	typedef unsigned long       DWORD;
	typedef void *PVOID;
#endif

typedef char Lchar;
typedef unsigned char Luchar;

typedef short Lshort;
typedef unsigned short Lushort;

typedef int	Lint;
typedef unsigned int Luint;

typedef long long Llong;
typedef unsigned long long Lulong;

typedef float Lfloat;

typedef double Ldouble;

typedef std::size_t Lsize;

typedef std::string Lstring;


enum USER_STATUS
{
	US_CENTER = 0, //大厅
	US_ROOM = 1,  //在桌子里
};



enum ROOM_STATUS
{
	RS_WAIT,  //
	RS_GAME,
};



enum
{
	ROOM_USER_CNT = 2,  //房间人数
	DESK_USER_CNT = 2,	 //桌子人数
	LIST_USER_MAX_CNT = 10000000,  //玩家队列最大数
};

enum
{
	RT_SUCCEED = 0,   //成功
	RT_ERROR = 1,	  //错误
};


enum CARD_TYPE
{
	CARD_NULL,
	CARD_BU,		//布
	CARD_JIAN_DAO,	//剪刀
	CARD_SHI_TOU,	//石头
};

//玩家筹码都是1颗星
enum DeskType
{
	DT_FIRST_TYPE = 1,  //普通玩法
	DT_SECOND_TYPE = 2,  //先手玩家亮牌奖励玩法（A和B两玩家对决，开局时系统为A提示一张牌，系统奖励为1颗星，系统金币奖励只会A在选择布并且赢的时候获得，其他的可能就按照正常判断奖励，即B永远也不会获得系统奖励）
	//筹码至少是2颗星
	DT_THIRD_TYPE = 3,		//开局前（A和B两个玩家对决，筹码n颗星。开始系统选出一张牌，假设为布并且有n颗星的奖励。根据玩家选出不同的牌有以下结果 1如果两个人不是平局，如果两个人中存在一个出现布，则赢家赢取输家和系统的星星一共2n颗星星，如果不存在则赢家只能赢取输家的n颗星星。 2如果两个人都出布则两个人平分系统的n金币,每人获得n/2金币 3，如果两个人平局并且都没有选择布，则两个人各输系统n/2金币）
	//筹码至少是3颗星
	DT_FOURTH_TYPE = 4,  //系统提示两张牌奖励（A和B两个玩家对决，筹码n金币。开始时系统为A和B两个人各提示一张牌，不能一样。，假设A提示的是剪刀，B提示的是布，因为剪刀胜于布，所以系统对A的奖励为n/2+1，对B的奖励为n/2。根据不同的玩家有以下的结果。 1 ，赢局:如果A真的出剪刀而B也真的出布，则A和B各得系统奖励，不结算各自输赢。如果其中一个出现与系统提示牌相同的，则获取输家的金币和与系统提示相同的玩家的系统奖励金币，如果都没有出现与系统提示相同，则赢家只赢取输家的金币  2，如果平局，如果玩家与系统提示的牌相同，则只赢取系统提示的牌。 ）
};

enum
{
	MAX_POS = 2,
};

enum
{
	POS_1 = 1,
	POS_2 = 2
};

enum
{
	MAX_OPER_TIME = 20,

};


enum ROOMUSER_MODIFY
{
	MODIFY_DEL_USER = 1,
	MODIFY_UPDATE_USER = 2,
};

enum UPGRADE_TYPE
{
	UPGRADE_HP,
	UPGRADE_ATTACK,
};


#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
    }\
    inline type get##name() {\
        return m_##name;\
    }\



//红方在下边，黑方在上边。期盼是左下角为（0,0）

enum CHESS_COLOR
{
	CHESS_ERROR_COLOR,
	CHESS_RED,
	CHESS_BLACK,
	
};

enum PosLimit
{
	POS_LIMIT_X = 8,
	POS_LIMIT_Y = 9
};

enum TYPE_CHESS
{
	CHESS_NULL,
	CHESS_JIANG,
	CHESS_SHI,
	CHESS_XIANG,
	CHESS_MA,
	CHESS_CHE,
	CHESS_PAO,
	CHESS_BING,
	CHESS_MAX,

	CHESS_SHI_TOU,  //石头，只存在一个回合，不能吃，只能占地方用
	CHESS_ZHA_DAN   //炸弹，炸自己四周一个格子距离的所有对方的棋子，也是存在一个回合
};

//动态前进就是在最大值和最小值的范围内随便移动
enum TYPE_OPERATOR_MOVE
{
	MOVE_NULL,							//没有动作
	MOVE_ADVANCE_FIXED,				//前进固定个数
	MOVE_ADVANCE_DYNAMIC,				//动态前进
	MOVE_BACK_FIXED,					//后退
	MOVE_BACK_DYNAMIC,					//动态后退
	MOVE_HORIZONTAL_FIXED,				//水平固定移动
	MOVE_HORIZONTAL_DYNAMIC,			//水平动态移动
	MOVE_LEAN_FIXED,					//倾斜固定移动
	MOVE_LEAN_DYNAMIC,					//倾斜固定移动
	MOVE_RI_ZI_GE						//日字移动

};

//方向
enum MOVE_DIREC
{
	MT_HORIZONTAL = 1,	//水平方向
	MT_VERTICAL,		//垂直方向
	MT_LEAN,			//倾斜方向

};


enum BAG_BLOCK_PROERTY
{
	BP_HP,
	BP_MAX_HP,
	BP_ATTACK,
	BP_MAX_ATTACK,
};

#if _LINUX
typedef bool BOOL;
#define TRUE                1
#endif


typedef char *NPSTR, *LPSTR, *PSTR;

//var 转换成字符串 var"
#define NAME_TO_STR(var)  (#var)

#endif
