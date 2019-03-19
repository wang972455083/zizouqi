#include "OutsideNet.h"
#include "LMsg.h"
#include "Config.h"
#include "Work.h"
#include "LLog.h"
#include "LMsgS2S.h"
#include "BaseCommonFunc.h"

bool OutsideNet::Init()
{
	SetMsgHeaderChecker(boost::bind(&OutsideNet::CheckMsgHeader, this, boost::placeholders::_1, boost::placeholders::_2));
	CreateMsgIdTable();
	LNet::Init("0.0.0.0", gConfig.GetOutsidePort(), 4, true, false);
	return true;
}

bool OutsideNet::Final()
{
	return true;
}

void OutsideNet::RecvMsgPack(LBuffPtr recv, LSocketPtr s)
{
	bool succeed = false;
	int msgid = MSG_ERROR_MSG;
	int user_id = 0;
	
	try{
		
		//msgpack::unpacked  unpack = msgpack::unpack( recv->Data() + recv->GetOffset(), Lsize(recv->Size() - recv->GetOffset()));
		msgpack::unpacked  unpack;
		msgpack::unpack(unpack,recv->Data() + recv->GetOffset(), Lsize(recv->Size() - recv->GetOffset()));
		msgpack::object obj = unpack.get();
		ReadMapData(obj, "m_msgId", msgid);

		/*if (msgid == 3001)
		{
			int roomid = 0;
			ReadMapData(obj, "m_room_id", roomid);

			LLOG_ERROR("m_room_id = %d",roomid);
		}*/
	
		if (msgid > MSG_ERROR_MSG && msgid < MSG_S_2_C_MAX)//�����Ϣ
		{
			LLOG_ERROR("First msgid = %d, body size = %d", msgid, recv->Size());

			if (msgid == MSG_C_2_S_HEART)
			{
				LMsg* msg_heart = LMsgFactory::Instance().CreateMsg(msgid);
				msg_heart->m_sp = s;
				if (msg_heart)
				{
					msg_heart->Read(obj);
				}
				gWork.Push(msg_heart);
				return;
			}
			
			//���͵�GameServer���
			if (msgid > 0 && msgid < MSG_TO_GAME_MAX_MSG)
			{
				LMsgG2GameUserMsg* msg = new LMsgG2GameUserMsg();
				msg->m_userMsgId = msgid;
				msg->m_dataBuff = recv;
				msg->m_ip = s->GetRemoteIp();
				msg->m_sp = s;
				gWork.Push(msg);
			}
			else if (msgid > MSG_TO_GAME_MAX_MSG && msgid < MSG_TO_LOGIC_MANAGER_MAX_MSG)
			{
				LMsgG2LMUserMsg* msg = new LMsgG2LMUserMsg();
				msg->m_userMsgId = msgid;
				msg->m_dataBuff = recv;
				msg->m_ip = s->GetRemoteIp();
				msg->m_sp = s;
				gWork.Push(msg);
			}
			else if (msgid > MSG_TO_LOGIC_MANAGER_MAX_MSG && msgid < MSG_TO_LOGIC_SERVER_MAX_MSG)
			{
				LMsgG2LUserMsg* msg = new LMsgG2LUserMsg();
				msg->m_userMsgId = msgid;
				msg->m_dataBuff = recv;
				msg->m_ip = s->GetRemoteIp();
				msg->m_sp = s;
				gWork.Push(msg);
			}
			succeed = true;
		}
		else
		{
			recv->Read(msgid);
			LLOG_ERROR(" Second msgid = %d, body size = %d", msgid, recv->Size());
			if(msgid == 0x7FFFFFFE || msgid == 0x7FFFFFFF)
			{
				LMsg* msg = LMsgFactory::Instance().CreateMsg(msgid);
				if(msg != NULL)
				{
					msg->Read(*recv.get());
					msg->m_sp = s;
					gWork.Push(msg);
					succeed = true;
				}
			}
		}

	}catch (...)
	{
		LLOG_ERROR("OutsideNet::RecvMsgPack error");
	}

	if(!succeed)
	{
		DisableIP(s->GetRemoteIp());
		s->Kick();
	}
}

bool OutsideNet::CheckMsgHeader(LSocketPtr s, const MsgHeader& header)
{
	bool succeed = false;
	if(header.bodySize >= 10 && header.bodySize <= 512)
	{
		Lint msgId = InverseTransfromMsgId(header.timestamp, header.msgId);
		succeed = IsValidMsgId(msgId);
	}
	return succeed;
}

void OutsideNet::CreateMsgIdTable()
{
	m_msgIdTable.insert(MSG_C_2_S_MSG);
	m_msgIdTable.insert(MSG_C_2_S_RE_LOGIN);
	m_msgIdTable.insert(MSG_C_2_S_LOGIN);
	m_msgIdTable.insert(MSG_C_2_S_LM_LOGIN);
	m_msgIdTable.insert(MSG_C_2_S_HEART);
	m_msgIdTable.insert(MSG_C_2_S_NOTICE_LOGIN);
	m_msgIdTable.insert(MSG_C_2_S_TEST);
	m_msgIdTable.insert(MSG_C_2_S_QUICK_ROOM_OPT);
	m_msgIdTable.insert(MSG_C_2_S_CREATE_DESK_ASK);
	m_msgIdTable.insert(MSG_C_2_S_CREATE_DESK_RESPON);
	m_msgIdTable.insert(MSG_C_2_S_SELECT_CARD);
	m_msgIdTable.insert(MSG_C_2_S_QUIT_ROOM);
	m_msgIdTable.insert(MSG_C_2_S_CREATE_XIANGQI_ROOM);
	m_msgIdTable.insert(MSG_C_2_S_ADD_INTO_ROOM);
	m_msgIdTable.insert(MSG_C_2_S_READY_OPT);
	m_msgIdTable.insert(MSG_C_2_S_ADD_BLOCK_TO_CHESS);

	m_msgIdTable.insert(MSG_C_2_S_CHESS_BLOCK_MOVE);
	m_msgIdTable.insert(MSG_C_2_S_CHESS_BLOCK_ATTACK);
	m_msgIdTable.insert(MSG_C_2_S_CHESS_BLOCK_UPGRADE);
	m_msgIdTable.insert(MSG_C_2_S_CHESS_BLOCK_ZHUI_SHA);

	m_msgIdTable.insert(MSG_C_2_S_GET_BAG_BLOCK_LIST);
	m_msgIdTable.insert(MSG_C_2_S_BAG_BLOCK_UPGRADE);
}

bool OutsideNet::IsValidMsgId(Lint id)
{
	auto iter = m_msgIdTable.find(id);
	return iter != m_msgIdTable.end();
}

