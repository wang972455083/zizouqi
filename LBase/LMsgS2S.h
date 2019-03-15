#ifndef _LMSG_S2S_H_
#define _LMSG_S2S_H_

#include "LMsg.h"
#include "LUser.h"
#include "LCharge.h"
#include "GameDefine.h"
#include "TCPClient.h"
#include "LChessBlock.h"

//////////////////////////////////////////////////////////////////////////
//logic �� center 

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

struct LogicInfo
{
	Lint		m_id;
	Lint		m_deskCount;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;
	TCPClientPtr	m_client;
	Lint		m_flag;
	Lint		m_exclusive_play;	//���������淨, 0:��ʾû��,���ڰ�ĳ���淨�鵽������,�������淨����ʱ����(�Уߩ�)

public:
	LogicInfo()
		:m_id(0)
		,m_deskCount(0)
		,m_port(0)
		,m_flag(0)
		,m_exclusive_play(0)
	{

	}
};



struct LMsgLMG2GateLogicInfo :public LMsg
{
	Lint		m_ID;
	Lint		m_count;
	LogicInfo	m_logic[500];

	LMsgLMG2GateLogicInfo() :LMsg(MSG_LMG_2_G_SYNC_LOGIC)
		,m_ID(0)
		,m_count(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_ID);
		buff.Read(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Read(m_logic[i].m_id);
			buff.Read(m_logic[i].m_deskCount);
			buff.Read(m_logic[i].m_ip);
			buff.Read(m_logic[i].m_port);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_ID);
		buff.Write(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Write(m_logic[i].m_id);
			buff.Write(m_logic[i].m_deskCount);
			buff.Write(m_logic[i].m_ip);
			buff.Write(m_logic[i].m_port);
		} 
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLMG2GateLogicInfo();
	}
};






struct GateInfo
{
	Lint		m_id;
	Lint		m_userCount;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;

public:
	GateInfo()
		:m_id(0)
		,m_userCount(0)
		,m_port(0)
	{

	}
};



//////////////////////////////////////////////////////////////////////////
//center�������id��Ϣ��

//��һ�����Ϣ
struct UserIdInfo
{
	Lint		m_id;
	Lstring		m_openId;
	Lstring		m_nike;
	Lstring		m_headImageUrl;
	Lint		m_sex;
	Lint		m_blockEndTime;

public:
	UserIdInfo()
		:m_id(0)
		,m_sex(0)
		,m_blockEndTime(0)
	{

	}
};



struct Season
{
	Lint            m_level; //�û���ȡ�Ĳ���ȯ����
	Lint            m_time; //�û���ȡ����ȯʱ��
	Lint            m_playState; //�淨
	string          m_playType; //�淨ѡ��
};














//////////////////////////////////////////////////////////////////////////
//gate ���������Ϣ �� logicserver
struct LMsgG2LUserMsg :public LMsg
{
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2LUserMsg() :LMsg(MSG_G_2_L_USER_MSG), m_userMsg(NULL)
		,m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2LUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		try {
			
			msgpack::unpacked  unpack;
			msgpack::unpack(unpack,buff.Data() + buff.GetOffset(), Lsize(buff.Size() - buff.GetOffset()));
			msgpack::object obj = unpack.get();

			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("LMsgG2LUserMsg::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgL2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;
	
	LMsgL2GUserMsg() :LMsg(MSG_L_2_G_USER_MSG)
		, m_user_id(0),m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2GUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//gate ������ҵ�����Ϣ �� logic
struct LMsgG2LUserOutMsg :public LMsg
{
	Llong			m_userGateId;
	
	LMsgG2LUserOutMsg():LMsg(MSG_G_2_L_USER_OUT_MSG)
		,m_userGateId(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userGateId);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userGateId);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LUserOutMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgL2GUserOutMsg : public LMsg
{
	Llong			m_id;

	LMsgL2GUserOutMsg():LMsg(MSG_L_2_G_USER_OUT_MSG)
		,m_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2GUserOutMsg();
	}
};






struct LMsgS2SKeepAlive : public LMsg
{
	LMsgS2SKeepAlive() : LMsg(MSG_S_2_S_KEEP_ALIVE) {}

	virtual bool Read(LBuff& buff)
	{
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SKeepAlive();
	}
};

struct LMsgG2LLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgG2LLogin() :LMsg(MSG_G_2_L_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LLogin();
	}
};

struct LMsgL2LMGLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;
	Lint		m_exclusive_play;	//��������ר���淨

	LMsgL2LMGLogin() :LMsg(MSG_L_2_LMG_LOGIN)
		, m_id(0)
		, m_port(0)
		, m_exclusive_play(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		buff.Read(m_exclusive_play);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		buff.Write(m_exclusive_play);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMGLogin();
	}
};

struct LMsgG2ServerLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgG2ServerLogin() :LMsg(MSG_G_2_SERVER_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2ServerLogin();
	}
};

struct LMsgS2SKeepAliveAck : public LMsg
{
	LMsgS2SKeepAliveAck() : LMsg(MSG_S_2_S_KEEP_ALIVE_ACK) {}

	virtual bool Read(LBuff& buff)
	{
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SKeepAliveAck();
	}
};


//�ָ���

struct LMsgGame2LMGLogin :public LMsg
{
	Lint		m_id;
	//Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgGame2LMGLogin() :LMsg(MSG_GAME_2_LM_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		//buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		//buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgGame2LMGLogin();
	}
};

struct GameServerInfo
{
	Lint		m_id;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;

	TCPClientPtr	m_client;
};

struct LMsgLMG2GateGameServerInfo :public LMsg
{
	Lint		m_ID;
	Lint		m_count;
	GameServerInfo	m_logic[500];

	LMsgLMG2GateGameServerInfo() :LMsg(MSG_LMG_2_G_SYNC_GAME_SERVER)
		, m_ID(0)
		, m_count(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_ID);
		buff.Read(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Read(m_logic[i].m_id);
			
			buff.Read(m_logic[i].m_ip);
			buff.Read(m_logic[i].m_port);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_ID);
		buff.Write(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Write(m_logic[i].m_id);
			
			buff.Write(m_logic[i].m_ip);
			buff.Write(m_logic[i].m_port);
		}
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLMG2GateGameServerInfo();
	}
}; 


struct LMsgG2GameUserMsg :public LMsg
{
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2GameUserMsg() :LMsg(MSG_G_2_GAME_USER_MSG), m_userMsg(NULL)
		, m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2GameUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		int msgid = MSG_ERROR_MSG;

		try {
			
			msgpack::unpacked  unpack;
			msgpack::unpack(unpack,buff.Data() + buff.GetOffset(), Lsize(buff.Size() - buff.GetOffset()));
			msgpack::object obj = unpack.get();

			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2GameUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("LMsgG2GameUserMsg::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2GameUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgGame2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;

	LMsgGame2GUserMsg() :LMsg(MSG_GAME_2_G_USER_MSG)
		, m_user_id(0),m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgGame2GUserMsg();
	}
};



//gate��logicmanager
struct LMsgG2LMUserMsg :public LMsg
{
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2LMUserMsg() :LMsg(MSG_G_2_LOGIC_MANAGER_USER_MSG), m_userMsg(NULL)
		, m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2LMUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		try {
			
			
			msgpack::unpacked  unpack = msgpack::unpack(buff.Data() + buff.GetOffset(), Lsize(buff.Size() - buff.GetOffset()));
			msgpack::object obj = unpack.get();

			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LMUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("LMsgG2LMUserMsg::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LMUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgLM2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;

	LMsgLM2GUserMsg() :LMsg(MSG_LOGIC_MANAGER_2_G_USER_MSG)
		, m_user_id(0), m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2GUserMsg();
	}
};

//����

struct MsgUserInfo
{
	int m_user_id;
	Lstring m_name;
	Lstring m_head_icon;
	Lint    m_gate_id;
	bool    m_robot;
};




struct LMsgLM2LQuickCreateRoomOpt : public LMsg
{
	Lint			m_room_id;
	
	std::vector<LUser>	m_users;
	int				m_user_cnt;


	LMsgLM2LQuickCreateRoomOpt() :LMsg(MSG_LM_2_L_QUICK_CREATE_ROOM_OPT)
		, m_room_id(0), m_user_cnt(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_room_id);
	
		buff.Read(m_user_cnt);

		for (int i = 0; i < m_user_cnt; ++i)
		{
			LUser user;
			user.Read(buff);

			m_users.push_back(user);
		}
		
		
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_room_id);
		
		m_user_cnt = m_users.size();
		buff.Write(m_user_cnt);

		for (int i = 0; i < m_users.size(); ++i)
		{
			LUser user = m_users[i];
			user.Write(buff);
		}
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2LQuickCreateRoomOpt();
	}
};


struct LMsgL2LMQuickCreateRoomOpt: public LMsg
{
	Lint			m_result; 
	Lint			m_room_id;

	std::vector<int>	m_users;
	int				m_user_cnt;


	LMsgL2LMQuickCreateRoomOpt() :LMsg(MSG_L_2_LM_QUICK_CREATE_ROOM_OPT)
		, m_room_id(0), m_user_cnt(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_result);
		buff.Read(m_room_id);

		buff.Read(m_user_cnt);
		for (int i = 0; i < m_user_cnt; ++i)
		{
			int user_id = 0;
			buff.Read(user_id);
			m_users.push_back(user_id);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_result);
		buff.Write(m_room_id);
		m_user_cnt = m_users.size();
		buff.Write(m_user_cnt);

		for (int i = 0; i < m_users.size(); ++i)
		{
			buff.Write(m_users[i]);
		}

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMQuickCreateRoomOpt();
	}
};



struct LMsgLM2GUserStatusModify : public LMsg
{
	Lint			m_user_id;	
	Lint			m_status;		//״̬0������  1������
	Lint			m_logic_server_id;  


	LMsgLM2GUserStatusModify() :LMsg(MSG_LM_2_G_USER_STATUS_MODIFY)
		, m_user_id(0), m_status(0), m_logic_server_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_status);
		buff.Read(m_logic_server_id);
		

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_status);
		buff.Write(m_logic_server_id);
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2GUserStatusModify();
	}
};


struct LMsgLM2LMQuckCreateRoom : public LMsg
{

	std::vector<LUser>	m_users;
	LMsgLM2LMQuckCreateRoom() :LMsg(MSG_LM_2_LM_QUICK_CREATE_ROOM)
	{
		m_users.clear();
	}

	virtual bool Read(LBuff& buff)
	{
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2LMQuckCreateRoom();
	}
};



struct LMsgL2LMQuitRoom :public LMsg
{
	int							m_room_id;
	std::vector<int>			m_users;
	int							m_count;
	
	
	LMsgL2LMQuitRoom() :LMsg(MSG_L_2_LM_QUIT_ROOM)
	{
		m_room_id = 0;
		m_users.clear();
		
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_room_id);
		
		buff.Read(m_count);

		for (int i = 0; i < m_count; ++i)
		{
			int user_id = 0;
			buff.Read(user_id);
			m_users.push_back(user_id);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_room_id);
		m_count = m_users.size();
		buff.Write(m_count);

		for (int i = 0; i < m_users.size(); ++i)
		{
			buff.Write(m_users[i]);
		}
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMQuitRoom();
	}
};


struct LMsgG2SUserLogOut :public LMsg
{
	int			m_user_id;

	LMsgG2SUserLogOut() :LMsg(MSG_G_2_S_USER_OUT)
	{
		m_user_id = 0;

	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2SUserLogOut();
	}
};

struct LMsgLM2LUserLogOut :public LMsg
{
	int			m_room_id;
	int			m_user_id;

	LMsgLM2LUserLogOut() :LMsg(MSG_LM_2_L_USER_OUT)
	{
		m_room_id = 0;
		m_user_id = 0;

	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_room_id);
		buff.Read(m_user_id);


		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_room_id);
		buff.Write(m_user_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2LUserLogOut();
	}
};

struct LMsgLM2LUserLogin :public LMsg
{
	int			m_room_id;
	int			m_user_id;
	int			m_gate_id;

	LMsgLM2LUserLogin() :LMsg(MSG_LM_2_L_USER_LOGIN)
	{
		m_room_id = 0;
		m_user_id = 0;
		m_gate_id = 0;

	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_room_id);
		buff.Read(m_user_id);
		buff.Read(m_gate_id);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_room_id);
		buff.Write(m_user_id);
		buff.Write(m_gate_id);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2LUserLogin();
	}
};

struct LMsgL2LMRecyleRoom : public LMsg
{
	Lint			m_room_id;

	LMsgL2LMRecyleRoom() :LMsg(MSG_L_2_LM_RECYLE_ROOM)
		, m_room_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_room_id);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_room_id);

	
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMRecyleRoom();
	}
};

struct LMsgS2SCreateXiangQiRoom : public LMsg
{
	int		m_user_id;
	int		m_room_id;
	int		m_count;
	vector<LChessBlock>		m_blocks;
	MsgUserInfo				m_self_info;


	LMsgS2SCreateXiangQiRoom() :LMsg(MSG_S_2_S_CREATE_XIANGQI_ROOM)
		, m_user_id(0), m_room_id(0), m_count(0)
	{
		m_blocks.clear();
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_room_id);
		buff.Read(m_count);

		for (int i = 0; i < m_count; ++i)
		{
			LChessBlock block;
			block.Read(buff);
			m_blocks.push_back(block);
		}

		buff.Read(m_self_info.m_user_id);
		buff.Read(m_self_info.m_head_icon);
		buff.Read(m_self_info.m_name);
		buff.Read(m_self_info.m_gate_id);
		buff.Read(m_self_info.m_robot);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_room_id);

		m_count = m_blocks.size();
		buff.Write(m_count);
		for (int i = 0; i < m_blocks.size(); ++i)
		{
			m_blocks[i].Write(buff);
		}


		buff.Write(m_self_info.m_user_id);
		buff.Write(m_self_info.m_head_icon);
		buff.Write(m_self_info.m_name);
		buff.Write(m_self_info.m_gate_id);
		buff.Write(m_self_info.m_robot);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SCreateXiangQiRoom();
	}
};


	
struct LMsgL2LMCreateRoomResult : public LMsg
{
	int				m_result;
	Lint			m_user_id;
	Lint			m_room_id;

	LMsgL2LMCreateRoomResult() :LMsg(MSG_L_2_LM_CREATE_XIANGQI_ROOM_RESULT)
		, m_user_id(0),m_room_id(0),m_result(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_result);
		buff.Read(m_user_id);
		buff.Read(m_room_id);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{

		buff.Write(m_result);
		buff.Write(m_user_id);
		buff.Write(m_room_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMCreateRoomResult();
	}
};

struct LMsgS2SAddIntoXiangQiRoom : public LMsg
{
	int		m_user_id;
	int		m_room_id;
	int		m_count;
	vector<LChessBlock>		m_blocks;
	MsgUserInfo				m_self_info;


	LMsgS2SAddIntoXiangQiRoom() :LMsg(MSG_S_2_S_ADD_INTO_XIANGQI_ROOM)
		, m_user_id(0), m_room_id(0), m_count(0)
	{
		m_blocks.clear();
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_room_id);
		buff.Read(m_count);

		for (int i = 0; i < m_count; ++i)
		{
			LChessBlock block;
			block.Read(buff);
			m_blocks.push_back(block);
		}

		buff.Read(m_self_info.m_user_id);
		buff.Read(m_self_info.m_head_icon);
		buff.Read(m_self_info.m_name);
		buff.Read(m_self_info.m_gate_id);
		buff.Read(m_self_info.m_robot);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_room_id);

		m_count = m_blocks.size();
		buff.Write(m_count);
		for (int i = 0; i < m_blocks.size(); ++i)
		{
			m_blocks[i].Write(buff);
		}


		buff.Write(m_self_info.m_user_id);
		buff.Write(m_self_info.m_head_icon);
		buff.Write(m_self_info.m_name);
		buff.Write(m_self_info.m_gate_id);
		buff.Write(m_self_info.m_robot);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SAddIntoXiangQiRoom();
	}
};

struct LMsgL2LMAddIntoRoomResult : public LMsg
{
	int				m_result;
	int				m_user_id;
	int				m_room_id;

	LMsgL2LMAddIntoRoomResult() :LMsg(MSG_L_2_LM_ADD_INTO_XIANGQI_ROOM_RESULT)
		, m_user_id(0), m_room_id(0), m_result(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_result);
		buff.Read(m_user_id);
		buff.Read(m_room_id);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{

		buff.Write(m_result);
		buff.Write(m_user_id);
		buff.Write(m_room_id);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMAddIntoRoomResult();
	}
};


struct LMsgGame2LMQuickStartOpt : public LMsg
{
	int		m_type;
	LUser	m_user;


	LMsgGame2LMQuickStartOpt() :LMsg(MSG_GAME_2_LM_QUICK_START_ROOM_OPT)
	{
		
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_type);
		m_user.Read(buff);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_type);
		m_user.Write(buff);

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgGame2LMQuickStartOpt();
	}
};

#endif