#ifndef _L_MSG_H_
#define _L_MSG_H_

#pragma warning (disable : 4018)
#pragma warning (disable : 4996)
#pragma warning (disable : 4819)
#pragma warning (disable : 4099)

#include "LSocket.h"
#include "LSingleton.h"
#include "LTool.h"
#include "LLog.h"
#include "LVideo.h"
#include "GameDefine.h"
#include "ScoreItem.h"
#include "ActiveDefine.h"
#include "LRoom.h"

static void ReadMapVector(msgpack::object & obj, const char * name, std::vector<Lint> & out)
{
	msgpack::object array;
	ReadMapData(obj, name, array);
	if (array.type == msgpack::v1::type::ARRAY)
	{
		for (Lsize i = 0; i < array.via.array.size; ++i)
		{
			Lint v;
			msgpack::object& obj =  *(array.via.array.ptr+i);
			obj.convert(v);
			out.push_back(v);
		}
	}
}

struct LMsg;

class LMsgFactory : public LSingleton<LMsgFactory>
{
public:
	virtual bool Init();

	virtual bool Final();

	void RegestMsg(LMsg* pMsg);

	LMsg* CreateMsg(int msgId);

private:
	std::map<int,LMsg*> m_msgMap;
};


struct LMsg
{
	LMsg(int id);
	virtual~LMsg();

	LBuffPtr		GetSendBuff();
	void			WriteMsgId(LBuff& buff);
	void			ReadMsgId(LBuff& buff);

	virtual bool	Write(LBuff& buff);
	virtual bool	Write(msgpack::packer<msgpack::sbuffer>& pack);
	virtual bool	Read(LBuff& buff);
	virtual bool	Read(msgpack::object& obj);
	virtual LMsg*	Clone();
	virtual Lint	preAllocSendMemorySize();

	LBuffPtr			m_send;
	LSocketPtr			m_sp;
	Lint				m_msgId;
	bool				m_isMsgPack;
};

//客户端 服务器交互的消息
struct LMsgSC :public LMsg
{
	LMsgSC(int id) :LMsg(id)
	{
		m_isMsgPack = true;
		m_user_id = 0;
	}

	int		m_user_id;
};
//玩家状态信息定义
enum LGUserMsgState
{
	LGU_STATE_IDLE,			//空闲，发到Center
	LGU_STATE_CENTER,		//玩家在大厅，发到Center
	LGU_STATE_DESK,			//玩家在桌子，发到Logic
	LGU_STATE_MATCH,		//玩家在比赛状态，发到Logic
};
//服务器端跟客户端通讯的消息号
enum LMSG_ID
{
	MSG_ERROR_MSG = 0,//错误消息

	MSG_C_2_S_MSG = 1,//客户端发给服务器
	MSG_S_2_C_MSG = 2,//服务器返回结果


	//
	MSG_C_2_S_RE_LOGIN = 10, //客户端请求重连
	MSG_C_2_S_LOGIN = 11, //客户端请求登录
	MSG_S_2_C_LOGIN = 12, //服务器返回客户登录结果

	MSG_C_2_S_TEST = 13,
	MSG_S_2_C_TEST = 14,

	MSG_C_2_S_CREATE_XIANGQI_ROOM = 15,
	MSG_S_2_C_CREATE_XIANGQI_ROOM = 16,

	MSG_C_2_S_ADD_INTO_ROOM = 17,
	MSG_S_2_C_ADD_INTO_ROOM = 18,

	MSG_C_2_S_QUICK_ROOM_OPT = 19,	//快速加入房间操作
	MSG_S_2_C_QUICK_ROOM_OPT = 20,	//
	
	MSG_C_2_S_GET_BAG_BLOCK_LIST = 21,	
	MSG_S_2_C_GET_BAG_BLOCK_LIST = 22,	

	MSG_C_2_S_BAG_BLOCK_UPGRADE = 23,
	MSG_S_2_C_BAG_BLOCK_UPGRADE = 24,

	MSG_TO_GAME_MAX_MSG = 1000,   //以上是转发到gameServer里的





	MSG_C_2_S_LM_LOGIN = 1001,		//登录LM
	MSG_S_2_C_LM_LOGIN = 1002,		//登录LM

	

	MSG_S_2_C_QUICK_ROOM_RESULT = 1005,	//快速创建房间结果

	MSG_C_2_S_NOTICE_LOGIN = 1006,			//通知登录
	

	MSG_TO_LOGIC_MANAGER_MAX_MSG = 2000,   //以上是转发到LogicManager上的







	

	

	MSG_C_2_S_CREATE_DESK_ASK = 3001,			//请求创建桌子
	MSG_S_2_C_NOTICE_CREATE_DESK_ASK = 3002,	//通知请求建桌子
	MSG_C_2_S_CREATE_DESK_RESPON = 3003,		//回复创建桌子
	MSG_S_2_C_CREATE_DESK_RESULT = 3004,	//创建桌子结果

	MSG_C_2_S_SELECT_CARD			= 3005,		//选择卡牌
	MSG_S_2_C_SELECT_CARD			= 3006,		//选择卡牌

	MSG_S_2_C_ROOM_INFO_MODIFY		= 3007,		//房间信息修改
	MSG_S_2_C_DESK_OVER_RESULT		= 3008,		//桌子比试结果

	MSG_S_2_C_TURN_DESK_POS = 3009,
	MSG_S_2_C_ROOM_USER_MODIFY = 3010,
	MSG_C_2_S_QUIT_ROOM = 3011,
	MSG_S_2_C_QUIT_ROOM = 3012,
	MSG_S_2_C_GAME_OVER = 3013,
	MSG_S_2_C_ROOM_INFO_WHEN_RELOGIN = 3014,

	MSG_C_2_S_READY_OPT = 3015,
	MSG_S_2_C_READY_OPT = 3016,
	MSG_S_2_C_SEND_GAME_INFO = 3017,
	MSG_S_2_C_QUICK_START_GAME = 3018,

	MSG_C_2_S_ADD_BLOCK_TO_CHESS = 3019,
	MSG_S_2_C_ADD_BLOCK_TO_CHESS = 3020,

	MSG_S_2_C_SEND_TURN = 3021,

	MSG_C_2_S_CHESS_BLOCK_MOVE = 3022,
	MSG_S_2_C_CHESS_BLOCK_MOVE = 3023,

	MSG_C_2_S_CHESS_BLOCK_ATTACK = 3024,
	MSG_S_2_C_CHESS_BLOCK_ATTACK = 3025,
	MSG_S_2_C_CHESS_COIN_MODIFY = 3026,

	MSG_C_2_S_CHESS_BLOCK_UPGRADE = 3027,
	MSG_S_2_C_CHESS_BLOCK_UPGRADE = 3028,
	MSG_S_2_C_CHESS_GAME_OVER = 3029,

	MSG_C_2_S_CHESS_BLOCK_ZHUI_SHA = 3030,
	MSG_S_2_C_CHESS_BLOCK_ZHUI_SHA = 3031,

	MSG_S_2_C_CHESS_REWARD_USE = 3032,

	MSG_TO_LOGIC_SERVER_MAX_MSG = 4000,		//以上是转发到LogicServer的


	MSG_C_2_S_HEART = 4001, //心跳包
	MSG_S_2_C_HEART = 4002, //心跳包

	MSG_S_2_S_MODIFY_COIN = 4003,  //金币修改
	MSG_L_2_L_AUTO_SELECT_CARD = 4004, //自动选择卡牌
	MSG_L_2_LM_QUIT_ROOM = 4005,

	
	MSG_S_2_C_MAX = 4096, //服务器客户端之间最大消息号

	//////////////////////////////////////////////////////////////////////////

	MSG_S_2_S_KEEP_ALIVE = 5000,
	MSG_S_2_S_KEEP_ALIVE_ACK = 5001,
	//下面这是服务器与服务器之间的消息交互

    MSG_G_2_L_LOGIN = 7000, //gate登录logic		由LogicManager转发
   
    MSG_G_2_L_USER_MSG = 8000, //gate转发玩家消息到logic
    MSG_L_2_G_USER_MSG = 8001, //logic转发玩家消息到gate
    
    MSG_G_2_L_USER_OUT_MSG = 8002, //gate转发玩家退出消息到logic
    MSG_L_2_G_USER_OUT_MSG = 8003, //logic转发玩家退出消息到gate

	MSG_G_2_S_USER_OUT = 8004,		//玩家掉线
	MSG_LM_2_L_USER_OUT = 8005,		//玩家掉线
	MSG_LM_2_L_USER_LOGIN = 8006,		//玩家denglu
    
	
	//////////////////////////////////////////////////////////////////////////
	
										
	//

    //////////////////////////////////////////////////////////////////////////
    //LogicManager 与 Logic
	MSG_L_2_LMG_LOGIN = 9001,				//逻辑管理登陆

	//................

	//分隔符
	MSG_GAME_2_LM_LOGIN = 10001,
	MSG_LMG_2_G_SYNC_GAME_SERVER = 10002,

	MSG_G_2_GAME_USER_MSG = 10003, //gate转发玩家消息到gameserver
	MSG_GAME_2_G_USER_MSG = 10004, //gameserver转发玩家消息到gate
	
	MSG_G_2_LOGIC_MANAGER_USER_MSG = 10005, //gate转发玩家消息到logicManager
	MSG_LOGIC_MANAGER_2_G_USER_MSG = 10006, //logicManager转发玩家消息到gate
	

    //LogicManager 与 Gate
    MSG_G_2_SERVER_LOGIN = 12001,			//逻辑管理登陆
                              
    MSG_LMG_2_G_SYNC_LOGIC = 12002,		//同步逻辑信息
  
	//业务逻辑


		
	MSG_LM_2_L_QUICK_CREATE_ROOM_OPT = 20003,  //快速创建房间
	MSG_L_2_LM_QUICK_CREATE_ROOM_OPT = 20004,

	MSG_LM_2_G_USER_STATUS_MODIFY = 20005,//  大厅和房间状态修改
	MSG_LM_2_LM_QUICK_CREATE_ROOM = 20006,//  创建房间

	MSG_L_2_LM_RECYLE_ROOM = 20007,// 回收房间号

	MSG_S_2_S_CREATE_XIANGQI_ROOM = 20008,
	MSG_L_2_LM_CREATE_XIANGQI_ROOM_RESULT = 20009,
	MSG_S_2_S_ADD_INTO_XIANGQI_ROOM = 20010,
	MSG_L_2_LM_ADD_INTO_XIANGQI_ROOM_RESULT = 20011,
	MSG_GAME_2_LM_QUICK_START_ROOM_OPT = 20012,
};

struct BirdInfo
{
	Lint	m_pos;
	CardValue m_card;

	MSGPACK_DEFINE(m_pos, m_card); 

	BirdInfo():m_pos(0){}
	bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteKeyValue(pack,NAME_TO_STR(m_pos),m_pos);
		m_card.Write(pack);
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
//socket断开的消息
#define MSG_CLIENT_KICK 0x7FFFFFFF

struct LMsgKick:public LMsg
{
	LMsgKick():LMsg(MSG_CLIENT_KICK){}

	virtual LMsg* Clone()
	{
		return new LMsgKick();
	}
};

//////////////////////////////////////////////////////////////////////////
//所有socket连上的消息
#define MSG_CLIENT_IN 0x7FFFFFFE

struct LMsgIn:public LMsg
{
	LMsgIn():LMsg(MSG_CLIENT_IN){}

	virtual LMsg* Clone()
	{
		return new LMsgIn();
	}
};

//////////////////////////////////////////////////////////////////////////
//http消息号
#define MSG_HTTP 0x7FFFFFFD

struct LMsgHttp:public LMsg
{
	std::string m_head;//消息头
	std::string m_body;//消息体

	LMsgHttp():LMsg(MSG_HTTP){}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_head);
		buff.Write(m_body);
		return true;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_head);
		buff.Read(m_body);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgHttp();
	}
};
//////////////////////////////////////////////////////////////////////////
// 向工会创建者发送房卡不足的警报
#define MSG_TEAM_CARD_ALERT	0x7FFFFFFC
struct LMsgTeamCardAlert :public LMsg
{
	Lint nCreaterID;
	Lint nTeamID;
	Lstring strTeamName;

	LMsgTeamCardAlert() :LMsg(MSG_TEAM_CARD_ALERT),
		nCreaterID(0),
		nTeamID(0)
	{}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(nCreaterID);
		buff.Write(nTeamID);
		buff.Write(strTeamName);
		return true;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(nCreaterID);
		buff.Read(nTeamID);
		buff.Read(strTeamName);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgTeamCardAlert();
	}
};
//////////////////////////////////////////////////////////////////////////
//客户端发送消息
struct LMsgC2SMsg :public LMsgSC
{
	Lstring			m_openId;
	Lstring			m_nike;
	Lstring			m_sign;
	Lstring			m_plate;
	Lstring			m_accessToken;
	Lstring			m_refreshToken;
	Lstring			m_md5;
	Lint			m_severID;
	Lstring			m_uuid;
	Lint			m_sex;
	Lstring			m_imageUrl;
	Lstring			m_nikename;
	Lint			m_wxConfirmed;
	Lstring			m_clientIp;
	Lint			m_nAppId;
	Lint			m_reLogin;		// 是否断线重连 0 正常登陆 1 断线重连
	Lint			m_deviceType;	// 设备类型 0 Android 1 IOS
	Lstring			m_deviceId;		// 设备编号

	LMsgC2SMsg() :LMsgSC(MSG_C_2_S_MSG)
		,m_severID(0)
		,m_sex(0)
		,m_wxConfirmed(0)
		,m_nAppId(0)
		,m_reLogin(0)
		,m_deviceType(0)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, "m_openId", m_openId);
		ReadMapData(obj, "m_nike", m_nike);
		ReadMapData(obj, "m_sign", m_sign);
		ReadMapData(obj, "m_plate", m_plate);
		ReadMapData(obj, "m_accessToken", m_accessToken);
		ReadMapData(obj, "m_refreshToken", m_refreshToken);
		ReadMapData(obj, "m_md5", m_md5);
		ReadMapData(obj, "m_severID", m_severID);
		ReadMapData(obj, "m_uuid", m_uuid);
		ReadMapData(obj, "m_sex", m_sex);
		ReadMapData(obj, "m_imageUrl", m_imageUrl);
		ReadMapData(obj, "m_nikename", m_nikename);
		ReadMapData(obj, "m_wxConfirmed", m_wxConfirmed);
		ReadMapData(obj, "m_clientIp", m_clientIp);
		ReadMapData(obj, "m_nAppId", m_nAppId);
		ReadMapData(obj, "m_reLogin", m_reLogin);
		ReadMapData(obj, "m_deviceId", m_deviceId);
		ReadMapData(obj, "m_deviceType", m_deviceType);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack,19);
		WriteKeyValue(pack, "m_msgId", m_msgId);
		WriteKeyValue(pack, "m_openId", m_openId);
		WriteKeyValue(pack, "m_nike", m_nike);
		WriteKeyValue(pack, "m_sign", m_sign);
		WriteKeyValue(pack, "m_plate", m_plate);
		WriteKeyValue(pack, "m_accessToken", m_accessToken);
		WriteKeyValue(pack, "m_refreshToken", m_refreshToken);
		WriteKeyValue(pack, "m_md5", m_md5);
		WriteKeyValue(pack, "m_severID", m_severID);
		WriteKeyValue(pack, "m_uuid", m_uuid);
		WriteKeyValue(pack, "m_sex", m_sex);
		WriteKeyValue(pack, "m_imageUrl", m_imageUrl);
		WriteKeyValue(pack, "m_nikename", m_nikename);
		WriteKeyValue(pack, "m_wxConfirmed", m_wxConfirmed);
		WriteKeyValue(pack, "m_clientIp", m_clientIp);
		WriteKeyValue(pack, "m_nAppId", m_nAppId);
		WriteKeyValue(pack, "m_reLogin", m_reLogin);
		WriteKeyValue(pack, "m_deviceId", m_deviceId);
		WriteKeyValue(pack, "m_deviceType", m_deviceType);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SMsg();
	}
};

//////////////////////////////////////////////////////////////////////////
//客户端发送消息
struct LMsgS2CMsg :public LMsgSC
{
	Lint			m_errorCode;//0-成功，1-服务器还没启动成功 2-微信登陆失败 3- 微信返回失败 4-创建角色失败 5- 在原APP未退  7,非法客户端
	Lint			m_seed;
	Lint			m_id;
	Lstring			m_gateIp;
	Lshort			m_gatePort;
	Lstring			m_errorMsg;	//若登陆失败，返回玩家错误原因;
	Lint			m_totalPlayNum;


	LMsgS2CMsg() :LMsgSC(MSG_S_2_C_MSG)
		,m_errorCode(0)
		,m_seed(0)
		,m_id(0)
		,m_gatePort(0)
		,m_totalPlayNum(0)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, "m_errorCode", m_errorCode);
		ReadMapData(obj, "m_seed", m_seed);
		ReadMapData(obj, "m_id", m_id);
		ReadMapData(obj, "m_gateIp", m_gateIp);
		ReadMapData(obj, "m_gatePort", m_gatePort);
		ReadMapData(obj, "m_errorMsg", m_errorMsg);
		ReadMapData(obj, "m_totalPlayNum", m_totalPlayNum);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 8);
		WriteKeyValue(pack, "m_msgId", m_msgId);
		WriteKeyValue(pack, "m_errorCode", m_errorCode);
		WriteKeyValue(pack, "m_seed", m_seed);
		WriteKeyValue(pack, "m_id", m_id);
		WriteKeyValue(pack, "m_gateIp", m_gateIp);
		WriteKeyValue(pack, "m_gatePort", m_gatePort);
		WriteKeyValue(pack, "m_errorMsg", m_errorMsg);
		WriteKeyValue(pack, "m_totalPlayNum", m_totalPlayNum);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CMsg();
	}
};

//////////////////////////////////////////////////////////////////////////
//客户端发送心跳包
struct LMsgC2SHeart:public LMsgSC
{
	//Lint	m_time;//时间戳

	LMsgC2SHeart() :LMsgSC(MSG_C_2_S_HEART){}

	virtual bool Read(msgpack::object& obj)
	{
		//buff.Read(m_time);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 1);
		WriteKeyValue(pack, "m_msgId", m_msgId);
		return true;
	}

	virtual LMsg* Clone(){return new LMsgC2SHeart();}
};


//////////////////////////////////////////////////////////////////////////
//服务器发送心跳包
struct LMsgS2CHeart :public LMsgSC
{
	Lint     m_returnId;
	Lint	m_time;//时间戳

	LMsgS2CHeart() :LMsgSC(MSG_S_2_C_HEART)
		,m_time(0), m_returnId(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_msgId), m_msgId);
		ReadMapData(obj, NAME_TO_STR(m_time), m_time);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack,2);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_returnId), m_returnId);
		WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
		return true;
	}

	virtual LMsg* Clone(){ return new LMsgS2CHeart(); }
};



//////////////////////////////////////////////////////////////////////////
//客户端请求登录大厅

struct LMsgC2SLogin:public LMsgSC
{
	Lint	m_seed;
	Lstring	m_md5;
	Lstring m_location;


	LMsgC2SLogin() :LMsgSC(MSG_C_2_S_LOGIN)
	,m_seed(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_seed), m_seed);
		
		ReadMapData(obj, NAME_TO_STR(m_md5), m_md5);
		ReadMapData(obj, NAME_TO_STR(m_location), m_location);
	
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_seed), m_seed);
		WriteKeyValue(pack, NAME_TO_STR(m_md5), m_md5);
		WriteKeyValue(pack, NAME_TO_STR(m_location), m_location);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SLogin();
	}
};

//////////////////////////////////////////////////////////////////////////
//返回玩家登陆结果消息
struct LMsgS2CLogin:public LMsgSC
{
	Lint		m_errorCode;//登录错误码，0-登录成功,1-帐号未注册，2-帐号已登录，3-未知错误
	Lstring		m_name;
	Lint		m_sex;
	Lstring		m_head_image;//
	Lint		m_money;
	
	LMsgS2CLogin() :LMsgSC(MSG_S_2_C_LOGIN)
		,m_errorCode(0)
		,m_sex(0)
		,m_money(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_name), m_name);
		
		ReadMapData(obj, NAME_TO_STR(m_sex), m_sex);
		ReadMapData(obj, NAME_TO_STR(m_head_image), m_head_image);
		ReadMapData(obj, NAME_TO_STR(m_money), m_money);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack,7);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
		
		WriteKeyValue(pack, NAME_TO_STR(m_name), m_name);
		WriteKeyValue(pack, NAME_TO_STR(m_sex), m_sex);
		WriteKeyValue(pack, NAME_TO_STR(m_head_image), m_head_image);
		WriteKeyValue(pack, NAME_TO_STR(m_money), m_money);
		return true;
	}

	virtual LMsg* Clone(){return new LMsgS2CLogin();}
};





//*****************************************************************************

struct LMsgC2SLMLogin :public LMsgSC
{
	LMsgC2SLMLogin() :LMsgSC(MSG_C_2_S_LM_LOGIN)
		
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 2);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SLMLogin();
	}
};

//返回玩家登陆结果消息
struct LMsgS2CLMLogin :public LMsgSC
{
	Lint		m_result;//登录错误码，0-登录成功,

	LMsgS2CLMLogin() :LMsgSC(MSG_S_2_C_LM_LOGIN)
		, m_result(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);

		return true;
	}

	virtual LMsg* Clone() { return new LMsgS2CLMLogin(); }
};








//快速加入
struct LMsgC2SQuickRoomOpt :public LMsgSC
{
	Lint	m_type;			// 0，快速加入，  1取消加入	
//	Lstring	m_name;
	//Lstring m_head_icon;

	

	LMsgC2SQuickRoomOpt() :LMsgSC(MSG_C_2_S_QUICK_ROOM_OPT)
		, m_type(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		//ReadMapData(obj, NAME_TO_STR(m_name), m_name);

		//ReadMapData(obj, NAME_TO_STR(m_head_icon), m_head_icon);

		/*msgpack::object array;
		ReadMapData(obj, NAME_TO_STR(m_list), array);
		if (array.type == msgpack::v1::type::ARRAY)
		{
			Lint m_count = array.via.array.size;
			for (Lsize i = 0; i < array.via.array.size; ++i)
			{
				DeskMsg v;
				msgpack::object& obj = *(array.via.array.ptr + i);
				obj.convert(v);
				m_list.push_back(v);
			}
		}*/
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
		//WriteKeyValue(pack, NAME_TO_STR(m_name), m_name);

		//WriteKeyValue(pack, NAME_TO_STR(m_head_icon), m_head_icon);
		
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SQuickRoomOpt();
	}
};




struct LMsgS2CQuickRoomOpt :public LMsgSC
{
	Lint		m_result;//0-成功,1,失败
	Lint		m_type;

	LMsgS2CQuickRoomOpt() :LMsgSC(MSG_S_2_C_QUICK_ROOM_OPT)
		, m_result(0),m_type(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
		
		return true;
	}

	virtual LMsg* Clone() { return new LMsgS2CQuickRoomOpt(); }
};

struct LMsgS2CQuickRoomResult :public LMsgSC
{
	Lint		m_result;//0-成功,1,失败
	RoomMsg		m_room;

	LMsgS2CQuickRoomResult() :LMsgSC(MSG_S_2_C_QUICK_ROOM_RESULT)
		, m_result(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room), m_room);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room), m_room);

		return true;
	}

	virtual LMsg* Clone() { return new LMsgS2CQuickRoomOpt(); }
};

struct LMsgC2SNoticeLogin :public LMsgSC
{
	Lint	m_user_id;

	LMsgC2SNoticeLogin() :LMsgSC(MSG_C_2_S_NOTICE_LOGIN)
		, m_user_id(0)
	{}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 2);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SNoticeLogin();
	}
};//LogicManager

//****************************************************************************

struct LMsgC2STest :public LMsgSC
{
	/*std::vector<RoomUser> m_users;


	LMsgC2STest() :LMsgSC(MSG_C_2_S_TEST)
	{
		m_users.clear();
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		//ReadMapData(obj, NAME_TO_STR(m_users), m_users);
		msgpack::object array;
		ReadMapData(obj, NAME_TO_STR(m_users), array);
		if (array.type == msgpack::v1::type::ARRAY)
		{
			Lint m_count = array.via.array.size;
			for (Lsize i = 0; i < array.via.array.size; ++i)
			{
				RoomUser v;
				msgpack::object& obj = *(array.via.array.ptr + i);
				obj.convert(v);
				m_users.push_back(v);
			}
		}


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_users), m_users);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2STest();
	}*/
};

struct LMsgS2CTest :public LMsgSC
{


	RoomMsg					m_desk;


	LMsgS2CTest() :LMsgSC(MSG_S_2_C_TEST)
	{
		
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		
		ReadMapData(obj, NAME_TO_STR(m_desk), m_desk);


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		
		WriteKeyValue(pack, NAME_TO_STR(m_desk), m_desk);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CTest();
	}
};
//*****************************************************************************LogicServer

struct LMsgC2SCreateDeskAsk :public LMsgSC
{
	Lint	m_room_id;
	Lint	m_opponent_id;   //对手id
	Lint	m_star;			 //竞技筹码星星数


	LMsgC2SCreateDeskAsk() :LMsgSC(MSG_C_2_S_CREATE_DESK_ASK)
	{
		m_room_id = 0;
		m_opponent_id = 0;
		m_star = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		//LLOG_ERROR("LMsgC2SCreateDeskAsk m_room_id = %d", m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_opponent_id), m_opponent_id);
		ReadMapData(obj, NAME_TO_STR(m_star), m_star);
	
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_opponent_id), m_opponent_id);
		WriteKeyValue(pack, NAME_TO_STR(m_star), m_star);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SCreateDeskAsk();
	}
};

struct LMsgS2CNoticeCreateDeskAsk :public LMsgSC
{
	Lint	m_room_id;
	Lint	m_opponent_id;   //对手id
	Lint	m_star;			 //竞技筹码星星数


	LMsgS2CNoticeCreateDeskAsk() :LMsgSC(MSG_S_2_C_NOTICE_CREATE_DESK_ASK)
	{
		m_room_id = 0;
		m_opponent_id = 0;
		m_star = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_opponent_id), m_opponent_id);
		ReadMapData(obj, NAME_TO_STR(m_star), m_star);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_opponent_id), m_opponent_id);
		WriteKeyValue(pack, NAME_TO_STR(m_star), m_star);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CNoticeCreateDeskAsk();
	}
};


struct LMsgC2SCreateDeskRespon :public LMsgSC
{
	Lint		m_result;		 // 0,同意 1，拒绝
	Lint		m_room_id;
	Lint		m_opponent_id;   //对手id
	Lint		m_star;			 //竞技筹码星星数


	enum 
	{
		RS_AGREE = 0,
		RS_REFUSE = 1,
	};


	LMsgC2SCreateDeskRespon() :LMsgSC(MSG_C_2_S_CREATE_DESK_RESPON)
	{
		m_result = 0;
		m_room_id = 0;
		m_opponent_id = 0;
		m_star = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_opponent_id), m_opponent_id);
		ReadMapData(obj, NAME_TO_STR(m_star), m_star);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_opponent_id), m_opponent_id);
		WriteKeyValue(pack, NAME_TO_STR(m_star), m_star);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SCreateDeskRespon();
	}
};



struct LMsgS2CNoticeCreateDeskResult :public LMsgSC
{
	Lint		m_result;		 // 0,同意 1，拒绝 2,星星不足，3，对方已经在竞赛中
	//MsgDesk		m_desk;


	enum
	{
		RS_AGREE = 0,
		RS_REFUSE = 1,

		RS_STAR_ERROR = 2,
		RS_IN_DESK = 3,
	};


	LMsgS2CNoticeCreateDeskResult() :LMsgSC(MSG_S_2_C_CREATE_DESK_RESULT)
	{
		m_result = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		//ReadMapData(obj, NAME_TO_STR(m_desk), m_desk);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		//WriteKeyValue(pack, NAME_TO_STR(m_desk), m_desk);
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CNoticeCreateDeskResult();
	}
};






struct LMsgC2SSelectCard :public LMsgSC
{
	Lint		m_operator_type;  //操作类型 ：0代表选择出的牌  1代表选择提示的牌
	Lint		m_room_id;
	Lint		m_desk_id;
	Lint		m_card;


	enum
	{
		OT_SELECT_CARD = 0,
		OT_SELECT_TIP_CARD,
	};


	LMsgC2SSelectCard() :LMsgSC(MSG_C_2_S_SELECT_CARD)
	{
		m_operator_type = 0;
		m_room_id = 0;
		m_desk_id = 0;
		m_card = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_operator_type), m_operator_type);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_desk_id), m_desk_id);
		ReadMapData(obj, NAME_TO_STR(m_card), m_card);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_operator_type), m_operator_type);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_desk_id), m_desk_id);
		WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SSelectCard();
	}
};

struct LMsgS2CSelectCard :public LMsgSC
{
	Lint			m_result;	
	Lint            m_operator_type;
	Lint			m_operator_user_id;  //操作的id
	Lint			m_room_id;
	Lint			m_desk_id;
	
	Lint			m_card;

	LMsgS2CSelectCard() :LMsgSC(MSG_S_2_C_SELECT_CARD)
	{
		m_result = 0;
		m_operator_type = 0;
		m_operator_user_id = 0;
		m_room_id = 0;
		m_desk_id = 0;
		m_card = 0;
	}



	enum
	{
		RT_SUCCEED,
		RT_USER_ERROR,
		RT_POS_ERROR,
		RT_CARD_ERROR,
	};

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_operator_type), m_operator_type);
		ReadMapData(obj, NAME_TO_STR(m_operator_user_id), m_operator_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_desk_id), m_desk_id);
		ReadMapData(obj, NAME_TO_STR(m_card), m_card);
		

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 8);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_operator_type), m_operator_type);

		WriteKeyValue(pack, NAME_TO_STR(m_operator_user_id), m_operator_user_id);

		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_desk_id), m_desk_id);
		WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CSelectCard();
	}
};


struct MsgUserResult
{
	int  m_user_id;	
	int  m_select_card;	//选择的牌
	int  m_stars;		//获得的星星数

	MsgUserResult()
	{
		m_user_id = 0;
		m_select_card = 0;
		m_stars = 0;
	}

	MSGPACK_DEFINE(m_user_id, m_select_card, m_stars);
};
struct LMsgS2CDeskOverResult :public LMsgSC
{
	Lint			m_desk_id;
	Lint			m_winner;		//0是平局 其他是赢家
	std::vector<MsgUserResult>	m_results;

	
	LMsgS2CDeskOverResult() :LMsgSC(MSG_S_2_C_DESK_OVER_RESULT)
	{
		m_winner = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_desk_id), m_desk_id);
		ReadMapData(obj, NAME_TO_STR(m_winner), m_winner);
		ReadMapData(obj, NAME_TO_STR(m_results), m_results);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_desk_id), m_desk_id);
		WriteKeyValue(pack, NAME_TO_STR(m_winner), m_winner);
		WriteKeyValue(pack, NAME_TO_STR(m_results), m_results);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CDeskOverResult();
	}
};


struct LMsgS2CTurnDeskPos:public LMsgSC
{
	Lint			m_desk_id;
	Lint			m_cur_pos;		//0是平局 其他是赢家
	Lint			m_left_select_time;

	LMsgS2CTurnDeskPos() :LMsgSC(MSG_S_2_C_TURN_DESK_POS)
	{
		m_desk_id = 0;
		m_cur_pos = 0;
		m_left_select_time = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_desk_id), m_desk_id);
		ReadMapData(obj, NAME_TO_STR(m_cur_pos), m_cur_pos);
		ReadMapData(obj, NAME_TO_STR(m_left_select_time), m_left_select_time);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_desk_id), m_desk_id);
		WriteKeyValue(pack, NAME_TO_STR(m_cur_pos), m_cur_pos);
		WriteKeyValue(pack, NAME_TO_STR(m_left_select_time), m_left_select_time);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CTurnDeskPos();
	}
};



struct LMsgS2CRoomUserModify :public LMsgSC
{
	int			m_type;
	int			m_room_id;
	int			m_modify_user_id;		
	int			m_stars;	//当前星星数
	int			m_desk_id;  //桌子id

	LMsgS2CRoomUserModify() :LMsgSC(MSG_S_2_C_ROOM_USER_MODIFY)
	{
		m_type = 0;
		m_room_id = 0;
		m_modify_user_id = 0;
		m_stars = 0;
		m_desk_id = 0;
	}
	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_modify_user_id), m_modify_user_id);
		ReadMapData(obj, NAME_TO_STR(m_stars), m_stars);
		ReadMapData(obj, NAME_TO_STR(m_desk_id), m_desk_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 7);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_modify_user_id), m_modify_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_stars), m_stars);
		WriteKeyValue(pack, NAME_TO_STR(m_desk_id), m_desk_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CRoomUserModify();
	}
};

struct LMsgC2SQuitRoom :public LMsgSC
{
	int			m_room_id;

	LMsgC2SQuitRoom() :LMsgSC(MSG_C_2_S_QUIT_ROOM)
	{
		m_room_id = 0;
	}
	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
	
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);

		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
	
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SQuitRoom();
	}
};


struct LMsgS2CQuitRoom :public LMsgSC
{
	int			m_result;
	int			m_room;

	LMsgS2CQuitRoom() :LMsgSC(MSG_S_2_C_QUIT_ROOM)
	{
		m_result = 0;
		m_room = 0;
	}
	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room), m_room);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);

		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room), m_room);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CQuitRoom();
	}
};



struct LMsgS2CRoomInfoWhenRelogin :public LMsgSC
{
	RoomMsg		m_room;

	LMsgS2CRoomInfoWhenRelogin() :LMsgSC(MSG_S_2_C_ROOM_INFO_WHEN_RELOGIN)
	{
	}
	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room), m_room);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);

		WriteKeyValue(pack, NAME_TO_STR(m_room), m_room);
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CRoomInfoWhenRelogin();
	}
};


//>>>*******************************************************************

////////////////////////////////////////////////////////////建桌子

struct LMsgC2SCreateXiangQiRoom :public LMsgSC
{
	

	LMsgC2SCreateXiangQiRoom() :LMsgSC(MSG_C_2_S_CREATE_XIANGQI_ROOM)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 2);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SCreateXiangQiRoom();
	}
};



struct LMsgS2CCreateXiangQiRoom :public LMsgSC
{
	int		m_result;
	int		m_room_id;

	LMsgS2CCreateXiangQiRoom() :LMsgSC(MSG_S_2_C_CREATE_XIANGQI_ROOM)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SCreateXiangQiRoom();
	}
};

struct LMsgS2CRoomInfoModify :public LMsgSC
{

	int				m_room_id;
	std::vector<S2CRoomUser>		m_users;


	LMsgS2CRoomInfoModify() :LMsgSC(MSG_S_2_C_ROOM_INFO_MODIFY)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_users), m_users);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_users), m_users);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CRoomInfoModify();
	}
};

struct LMsgC2SAddIntoRoom :public LMsgSC
{
	int		m_room_id;

	LMsgC2SAddIntoRoom() :LMsgSC(MSG_C_2_S_ADD_INTO_ROOM)
	{
		m_room_id = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SAddIntoRoom();
	}
};



struct LMsgS2CAddIntoRoom :public LMsgSC
{
	int		m_result;
	int		m_room_id;

	LMsgS2CAddIntoRoom() :LMsgSC(MSG_S_2_C_ADD_INTO_ROOM)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CAddIntoRoom();
	}
};


struct LMsgC2SReadyOpt :public LMsgSC
{
	int		m_room_id;
	int		m_type;		//0，准备  1，取消准备


	enum Opt
	{
		OPT_READY,
		OPT_CANCEL_READY,
	};

	LMsgC2SReadyOpt() :LMsgSC(MSG_C_2_S_READY_OPT)
	{
		m_room_id = 0;
		m_type = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SReadyOpt();
	}
};



struct LMsgS2CReadyOpt :public LMsgSC
{
	int		m_result;
	int		m_room_id;
	int		m_type;
	int		m_target_user;

	LMsgS2CReadyOpt() :LMsgSC(MSG_S_2_C_READY_OPT),m_result(0),m_room_id(0),m_type(0),m_target_user(0)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		ReadMapData(obj, NAME_TO_STR(m_target_user), m_target_user);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
		WriteKeyValue(pack, NAME_TO_STR(m_target_user), m_target_user);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CReadyOpt();
	}
};


struct LMsgS2CSendGameInfo :public LMsgSC
{
	GameInfo  m_game_info;

	LMsgS2CSendGameInfo() :LMsgSC(MSG_S_2_C_SEND_GAME_INFO)
	{
		
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_info), m_game_info);
		
	
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_game_info), m_game_info);
		
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CSendGameInfo();
	}
};

struct LMsgS2CQuickStartGame :public LMsgSC
{
	RoomMsg		m_room;

	LMsgS2CQuickStartGame() :LMsgSC(MSG_S_2_C_QUICK_START_GAME)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room), m_room);


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room), m_room);



		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CQuickStartGame();
	}
};



struct LMsgC2SAddBlockToChess :public LMsgSC
{
	int		m_room_id;
	int		m_block_id;	
	int		m_type;		//用来区分增加的是正常棋子还是石头或者炸弹
	int     m_pos_x;
	int		m_pos_y;


	LMsgC2SAddBlockToChess() :LMsgSC(MSG_C_2_S_ADD_BLOCK_TO_CHESS)
	{
		m_room_id = 0;
		m_block_id = 0;
		m_type = 0;
		m_pos_x = 0;
		m_pos_y = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		ReadMapData(obj, NAME_TO_STR(m_pos_x), m_pos_x);
		ReadMapData(obj, NAME_TO_STR(m_pos_y), m_pos_y);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 7);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_x), m_pos_x);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_y), m_pos_y);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SAddBlockToChess();
	}
};



struct LMsgS2CAddBlockToChess :public LMsgSC
{
	int		m_result;
	int		m_room_id;
	int		m_oper_user_id;
	GameBlock	m_block;

	LMsgS2CAddBlockToChess() :LMsgSC(MSG_S_2_C_ADD_BLOCK_TO_CHESS),m_result(0),m_room_id(0),m_oper_user_id(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		ReadMapData(obj, NAME_TO_STR(m_block), m_block);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block), m_block);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CAddBlockToChess();
	}
};


struct MsgZhuiSha
{
	int m_target_block;
	std::vector<int>	m_attack_blocks;


	MsgZhuiSha()
	{
		m_target_block = 0;
		m_attack_blocks.clear();
	}

	MSGPACK_DEFINE(m_target_block, m_attack_blocks);
};

struct LMsgS2CSendTurn :public LMsgSC
{
	
	int		m_room_id;
	int		m_cur_color;
	int		m_left_time;

	MsgZhuiSha	m_zhuisha;

	LMsgS2CSendTurn() :LMsgSC(MSG_S_2_C_SEND_TURN), m_room_id(0), m_cur_color(0), m_left_time(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);

		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_cur_color), m_cur_color);
		ReadMapData(obj, NAME_TO_STR(m_left_time), m_left_time);
		ReadMapData(obj, NAME_TO_STR(m_zhuisha), m_zhuisha);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
	
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_cur_color), m_cur_color);
		WriteKeyValue(pack, NAME_TO_STR(m_left_time), m_left_time);
		WriteKeyValue(pack, NAME_TO_STR(m_zhuisha), m_zhuisha);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CSendTurn();
	}
};

struct LMsgC2SChessBlockMove :public LMsgSC
{
	int		m_room_id;
	int		m_block_id;
	int		m_pos_x;
	int     m_pos_y;

	LMsgC2SChessBlockMove() :LMsgSC(MSG_C_2_S_CHESS_BLOCK_MOVE), 
		m_block_id(0), m_pos_x(0), m_pos_y(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_pos_x), m_pos_x);
		ReadMapData(obj, NAME_TO_STR(m_pos_y), m_pos_y);
	

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_x), m_pos_x);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_y), m_pos_y);
		

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SChessBlockMove();
	}
};


struct LMsgS2CChessBlockMove :public LMsgSC
{
	int		m_result;
	int		m_room_id;
	int		m_oper_user_id;
	int		m_block_id;
	int		m_pos_x;
	int     m_pos_y;

	LMsgS2CChessBlockMove() :LMsgSC(MSG_S_2_C_CHESS_BLOCK_MOVE), m_result(0),m_room_id(0), m_oper_user_id(0),
		m_block_id(0), m_pos_x(0), m_pos_y(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_pos_x), m_pos_x);
		ReadMapData(obj, NAME_TO_STR(m_pos_y), m_pos_y);


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 8);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_x), m_pos_x);
		WriteKeyValue(pack, NAME_TO_STR(m_pos_y), m_pos_y);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SChessBlockMove();
	}
};


struct LMsgC2SChessBlockAttack :public LMsgSC
{
	int		m_room_id;
	int		m_attack_id;
	int		m_get_damage_id;

	LMsgC2SChessBlockAttack() :LMsgSC(MSG_C_2_S_CHESS_BLOCK_ATTACK),
		m_room_id(0), m_attack_id(0), m_get_damage_id(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_attack_id), m_attack_id);
		ReadMapData(obj, NAME_TO_STR(m_get_damage_id), m_get_damage_id);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_attack_id), m_attack_id);
		WriteKeyValue(pack, NAME_TO_STR(m_get_damage_id), m_get_damage_id);
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SChessBlockAttack();
	}
};


struct LMsgS2CChessBlockAttack :public LMsgSC
{
	int		m_result;
	int		m_room_id;
	GameBlock	m_attack_block;
	GameBlock	m_get_damage_block;

	LMsgS2CChessBlockAttack() :LMsgSC(MSG_S_2_C_CHESS_BLOCK_ATTACK), m_result(0), m_room_id(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_attack_block), m_attack_block);
		ReadMapData(obj, NAME_TO_STR(m_get_damage_block), m_get_damage_block);



		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_attack_block), m_attack_block);
		WriteKeyValue(pack, NAME_TO_STR(m_get_damage_block), m_get_damage_block);
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessBlockAttack();
	}
};


struct LMsgS2CChessCoinModify :public LMsgSC
{
	int		m_target_user_id;
	int		m_coin;
	

	LMsgS2CChessCoinModify() :LMsgSC(MSG_S_2_C_CHESS_COIN_MODIFY), m_target_user_id(0),m_coin(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_target_user_id), m_target_user_id);
		ReadMapData(obj, NAME_TO_STR(m_coin), m_coin);
		
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_target_user_id), m_target_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_coin), m_coin);
	
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessCoinModify();
	}
};


struct LMsgC2SChessBlockUpgrade :public LMsgSC
{
	
	int		m_room_id;
	int		m_block_id;
	int		m_upgrade_type;   // 0,增加血量  1,增加攻击力


	LMsgC2SChessBlockUpgrade() :LMsgSC(MSG_C_2_S_CHESS_BLOCK_UPGRADE),
		m_room_id(0), m_block_id(0), m_upgrade_type(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_upgrade_type), m_upgrade_type);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_upgrade_type), m_upgrade_type);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SChessBlockUpgrade();
	}
};


struct LMsgS2CChessBlockUpgrade :public LMsgSC
{
	int		m_result;
	int		m_oper_user_id;
	int		m_room_id;
	int		m_block_id;
	int		m_upgrade_type;
	int		m_cur_value;

	LMsgS2CChessBlockUpgrade() :LMsgSC(MSG_S_2_C_CHESS_BLOCK_UPGRADE), m_result(0), m_room_id(0),m_block_id(0),m_upgrade_type(0),m_cur_value(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_upgrade_type), m_upgrade_type);
		ReadMapData(obj, NAME_TO_STR(m_cur_value), m_cur_value);


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 8);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_oper_user_id), m_oper_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_upgrade_type), m_upgrade_type);
		WriteKeyValue(pack, NAME_TO_STR(m_cur_value), m_cur_value);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessBlockUpgrade();
	}
};


struct LMsgS2CChessGameOver :public LMsgSC
{
	int m_room_id;
	int m_winner;

	LMsgS2CChessGameOver() :LMsgSC(MSG_S_2_C_CHESS_GAME_OVER), m_room_id(0), m_winner(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_winner), m_winner);
		


		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_winner), m_winner);
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessGameOver();
	}
};


struct LMsgC2SChessBlockZhuiSha :public LMsgSC
{
	int		m_room_id;
	int		m_attack_id;
	int		m_get_damage_id;

	LMsgC2SChessBlockZhuiSha() :LMsgSC(MSG_C_2_S_CHESS_BLOCK_ZHUI_SHA),
		m_room_id(0), m_attack_id(0), m_get_damage_id(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_attack_id), m_attack_id);
		ReadMapData(obj, NAME_TO_STR(m_get_damage_id), m_get_damage_id);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_attack_id), m_attack_id);
		WriteKeyValue(pack, NAME_TO_STR(m_get_damage_id), m_get_damage_id);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SChessBlockZhuiSha();
	}
};


struct LMsgS2CChessBlockZhuiSha :public LMsgSC
{
	int		m_result;
	int		m_room_id;
	GameBlock	m_attack_block;
	GameBlock	m_get_damage_block;

	LMsgS2CChessBlockZhuiSha() :LMsgSC(MSG_S_2_C_CHESS_BLOCK_ZHUI_SHA), m_result(0), m_room_id(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_room_id), m_room_id);
		ReadMapData(obj, NAME_TO_STR(m_attack_block), m_attack_block);
		ReadMapData(obj, NAME_TO_STR(m_get_damage_block), m_get_damage_block);



		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 6);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_room_id), m_room_id);
		WriteKeyValue(pack, NAME_TO_STR(m_attack_block), m_attack_block);
		WriteKeyValue(pack, NAME_TO_STR(m_get_damage_block), m_get_damage_block);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessBlockZhuiSha();
	}
};

struct LMsgS2CChessRewardUse :public LMsgSC
{
	int		m_target_user_id;
	int		m_reward_item;
	GameBlock	m_block;


	LMsgS2CChessRewardUse() :LMsgSC(MSG_S_2_C_CHESS_REWARD_USE), m_target_user_id(0), m_reward_item(0)
	{

	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_target_user_id), m_target_user_id);
		ReadMapData(obj, NAME_TO_STR(m_reward_item), m_reward_item);
		ReadMapData(obj, NAME_TO_STR(m_block), m_block);

		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_target_user_id), m_target_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_reward_item), m_reward_item);
		WriteKeyValue(pack, NAME_TO_STR(m_block), m_block);
		


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CChessRewardUse();
	}
};


struct LMsgC2SGetBagBlockList :public LMsgSC
{
	int m_tag;
	
	LMsgC2SGetBagBlockList() :LMsgSC(MSG_C_2_S_GET_BAG_BLOCK_LIST)
	{
		m_tag = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_tag), m_tag);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_tag), m_tag);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SGetBagBlockList();
	}
};


struct LMsgS2CGetBagBlockList :public LMsgSC
{
	std::vector<S2CBlock>	m_bag_blocks;

	LMsgS2CGetBagBlockList() :LMsgSC(MSG_S_2_C_GET_BAG_BLOCK_LIST)
	{
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_bag_blocks), m_bag_blocks);

	
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 3);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_bag_blocks), m_bag_blocks);
		


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CGetBagBlockList();
	}
};

struct LMsgC2SBagBlockUpgrade :public LMsgSC
{
	int m_block_id;
	int m_type;

	LMsgC2SBagBlockUpgrade() :LMsgSC(MSG_C_2_S_BAG_BLOCK_UPGRADE)
	{
		m_block_id = 0;
		m_type = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);
		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 4);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgC2SBagBlockUpgrade();
	}
};


struct LMsgS2CBagBlockUpgrade :public LMsgSC
{
	int m_result;
	int m_block_id;
	int m_type;

	LMsgS2CBagBlockUpgrade() :LMsgSC(MSG_S_2_C_BAG_BLOCK_UPGRADE)
	{
		m_result = 0;
		m_block_id = 0;
		m_type = 0;
	}

	virtual bool Read(msgpack::object& obj)
	{
		ReadMapData(obj, NAME_TO_STR(m_user_id), m_user_id);
		ReadMapData(obj, NAME_TO_STR(m_result), m_result);
		ReadMapData(obj, NAME_TO_STR(m_block_id), m_block_id);
		ReadMapData(obj, NAME_TO_STR(m_type), m_type);



		return true;
	}

	virtual bool Write(msgpack::packer<msgpack::sbuffer>& pack)
	{
		WriteMap(pack, 5);
		WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
		WriteKeyValue(pack, NAME_TO_STR(m_user_id), m_user_id);
		WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
		WriteKeyValue(pack, NAME_TO_STR(m_block_id), m_block_id);
		WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);


		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2CBagBlockUpgrade();
	}
};


//*******************************************************************


//*****************************************************************************

#endif