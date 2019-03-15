#include "Work.h"
#include "LLog.h"
#include "Config.h"
#include "InsideNet.h"
#include "LMsgS2S.h"



#include <openssl/md5.h>
#include "table/TableManager.h"
#include "RoomManager.h"
#include "ChessManager.h"


using namespace boost::asio;  
using boost::asio::ip::tcp; 

//初始化
bool Work::Init()
{
	//设置log文件
	LLog::Instance().SetFileName("LogicServer");

	//初始化配置文件
	if(!gConfig.Init())
	{
		LLOG_ERROR("gConfig.Init error");
		return false;
	}

	LLog::Instance().SetLogLevel(gConfig.GetLogLevel());

	LMsgFactory::Instance().Init();

	if (!gInsideNet.Init())
	{
		LLOG_ERROR("gInsideNet.Init error");
		return false;
	}

	TableManager::Instance().Init();
	ChessManager::getInstance().Init();

	LTime cur;
	m_30MSceTick = cur.MSecs();
	m_200MSceTick = cur.MSecs();
	m_1SceTick = cur.MSecs();
	m_5SceTick = cur.MSecs();
	m_30SceTick = cur.MSecs();
	m_600SceTick = cur.MSecs();

	m_loginLMFirst = true;

	return true;
}

bool Work::Final()
{
	return true;
}

//启动
void Work::Start()
{
	gInsideNet.Start();

	LRunnable::Start();

	ConnectToLogicManager();

	m_gcService.Start();
}

//等待
void Work::Join()
{
	gInsideNet.Join();

	//等待逻辑线程终止
	LRunnable::Join();

}

//停止
void Work::Stop()
{
	m_logicManager.reset();

	m_gateInfo.clear();

	gInsideNet.Stop();
	
	LRunnable::Stop();
	
	m_gcService.Stop();
}

void Work::Clear()
{

}

void Work::Run()
{
	while(!GetStop())
	{
		//这里处理游戏主循环
		LTime cur;
		if(cur.MSecs() - m_tickTimer.MSecs() > 1)
		{
			m_tickTimer = cur;
			Tick(cur);	
		}

		LMsg* msg = (LMsg*)Pop();
		if(msg == NULL)
		{
			Sleep(1);
			continue;
		}
		else
		{
			HanderMsg(msg);
			//delete msg;
			m_gcService.Collect(msg);
		}
	}
}

void Work::Tick(LTime& cur)
{
	//30毫秒循环一次
	if (cur.MSecs() - m_30MSceTick > 30)
	{
		m_30MSceTick = cur.MSecs();
	}

	//200毫秒循环一次
	if (cur.MSecs() - m_200MSceTick > 200)
	{
		m_200MSceTick = cur.MSecs();
	}

	//1000毫秒循环一次
	if (cur.MSecs() - m_1SceTick > 1000)
	{
		gRoomManager.Tick();
		m_1SceTick = cur.MSecs();
	}

	//5秒循环一次
	if (cur.MSecs() - m_5SceTick > 5000)
	{
		m_5SceTick = cur.MSecs();
	}

	//30秒循环一次
	if (cur.MSecs() - m_30SceTick > 30*1000)
	{
		m_30SceTick = cur.MSecs();
	
	}

	//5分钟循环一次
	if (cur.MSecs() - m_600SceTick > 5*60 * 1000)
	{
		m_600SceTick = cur.MSecs();
	}

	if(m_logicManager)
		m_logicManager->OnTick(cur);
	
}

LTime& Work::GetCurTime()
{
	return m_tickTimer;
}

void Work::HanderMsg(LMsg* msg)
{
	//玩家请求登录
	switch(msg->m_msgId)
	{
	case MSG_CLIENT_KICK:
		HanderUserKick((LMsgKick*)msg);
		break;
	case MSG_CLIENT_IN:
		HanderClientIn((LMsgIn*)msg);
		break;
	case MSG_HTTP:
		HanderHttp((LMsgHttp*)msg);
		break;
		
	case MSG_G_2_SERVER_LOGIN:
		HanderGateLogin((LMsgG2ServerLogin*)msg);
		break;
	case MSG_G_2_L_USER_MSG:
		{
			GateInfo* gate = GetGateInfoBySp(msg->m_sp);
			if (gate)
			{
				HanderGateUserMsg((LMsgG2LUserMsg*)msg,gate);
			}
			else
			{
				LLOG_ERROR("Work::HanderMsg gate not exiext");
			}
		}
		break;
	case MSG_G_2_L_USER_OUT_MSG:
		{
			LMsgG2LUserOutMsg* gmsg = (LMsgG2LUserOutMsg*)msg;
			GateInfo* gate = GetGateInfoBySp(msg->m_sp);
			if (gate)
			{
				//HanderUserLogout(gate->m_id,gmsg->m_userGateId);
			}
			else
			{
				LLOG_ERROR("Work::HanderMsg gate not exiext");
			}
		}
		break;
	
	case MSG_S_2_S_KEEP_ALIVE:
		HanderKeepAlive((LMsgS2SKeepAlive*)msg);
		break;
	case MSG_S_2_S_KEEP_ALIVE_ACK:
		HanderKeepAliveAck((LMsgS2SKeepAliveAck*)msg);
		break;
	//case MSG_LM_2_L_DESK_OPT:
		//HanderDeskOpt((LMsgLM2LDeskOpt*)msg);
	case MSG_LM_2_L_QUICK_CREATE_ROOM_OPT:
		HanderQuickCreateRoomOpt((LMsgLM2LQuickCreateRoomOpt*)msg);
		break;
	case MSG_L_2_L_AUTO_SELECT_CARD:
		//HanderAutoSelectCard((LMsgL2LAutoSelectCard*)msg);
		break;
	case MSG_LM_2_L_USER_LOGIN:
		HanderLM2LUserLogin((LMsgLM2LUserLogin*)msg);
		break;
	case MSG_LM_2_L_USER_OUT:
		HanderLM2LUserLogout((LMsgLM2LUserLogOut*)msg);
		break;
		//***********************
	case MSG_S_2_S_CREATE_XIANGQI_ROOM:
		break;

	default:
		break;
	}
}


/*void Work::HanderAutoSelectCard(LMsgL2LAutoSelectCard* msg)
{
	LRoomPtr room = gRoomManager.GetRoomById(msg->m_room_id);
	if (room)
	{


		room->HanderAutoSelectCard(msg);

	}
}*/


void Work::HanderUserKick(LMsgKick* msg)
{
	if (m_logicManager == msg->m_sp)
	{
		LLOG_ERROR("LogicManager DisConnect!");
		return;
	}

	HanderGateLogout(msg);
}



void Work::HanderClientIn(LMsgIn* msg)
{
	if(m_logicManager == msg->m_sp)
	{
		LLOG_ERROR("Connecting logicmanager on %s:%d succeed", m_logicManager->GetServerIP().c_str(), m_logicManager->GetServerPort());
		SendLogicLoginToLM();
	}
}

void Work::HanderHttp(LMsgHttp* msg)
{	

}

void Work::ConnectToLogicManager()
{
	KeepAliveSetting setting = {true, 15000, 10000};
	m_logicManager.reset(new TCPClient(gInsideNet, gConfig.GetLogicManagerIp().c_str(), gConfig.GetLogicManagerPort(), setting));
	LLOG_ERROR("Work::ConnectToLogicManager begin %s:%d", gConfig.GetLogicManagerIp().c_str(), gConfig.GetLogicManagerPort());
	m_logicManager->Connect();
	LLOG_ERROR("Work::ConnectToLogicManager end %s:%d", gConfig.GetLogicManagerIp().c_str(), gConfig.GetLogicManagerPort());
}

void Work::SendToLogicManager(LMsg& msg)
{
	if(m_logicManager)
	{
		m_logicManager->Send(msg.GetSendBuff());
	}
}

void Work::HanderGateLogin(LMsgG2ServerLogin* msg)
{
	LLOG_ERROR("Work::HanderGateLogin Gate Login: %d", msg->m_id);

	if (msg->m_key.empty())
	{
		msg->m_sp->Stop();
		LLOG_ERROR("Work::HanderGateLogin key error %d %s",msg->m_id, msg->m_key.c_str());
		return;
	}

	auto iter = m_gateInfo.find(msg->m_id);
	if (iter != m_gateInfo.end())
	{
		if(iter->second.m_sp)
		{
			if(iter->second.m_sp != msg->m_sp)
			{
				iter->second.m_sp->Stop();
				DelGateInfo(msg->m_id);
				LLOG_ERROR("Work::HanderGateLogin: delete already existed gate info %d", msg->m_id);
			}
			else
				return;
		}

	}

	GateInfo info;
	info.m_id = msg->m_id;
	info.m_ip = msg->m_ip;
	info.m_port = msg->m_port;
	info.m_userCount = 0;
	info.m_sp = msg->m_sp;
	m_gateInfo[info.m_id] = info;

	//SendGateInfoToCenter();
}

void Work::HanderGateLogout(LMsgKick* msg)
{
	GateInfo* info = GetGateInfoBySp(msg->m_sp);
	if (info)
	{
		LLOG_ERROR(" Work::HanderGateLogout: %d", info->m_id);
		DelGateInfo(info->m_id);
	}
}

GateInfo* Work::GetGateInfoBySp(LSocketPtr sp)
{
	std::map<Lint, GateInfo>::iterator it = m_gateInfo.begin();
	for (; it != m_gateInfo.end(); ++it)
	{
		if (sp == it->second.m_sp)
			return &it->second;
	}
	return NULL;
}

GateInfo* Work::GetGateInfoById(Lint id)
{
	if (m_gateInfo.count(id))
		return &m_gateInfo[id];

	return NULL;
}

void Work::DelGateInfo(Lint id)
{
	auto iter = m_gateInfo.find(id);
	if (iter != m_gateInfo.end())
	{
		m_gateInfo.erase(iter);
	}
}

void Work::HanderGateUserMsg(LMsgG2LUserMsg* msg,GateInfo* gate)
{
	int msgId = msg->m_userMsgId;
	if (msg->m_userMsg)
	{
		switch (msgId)
		{
		
		case MSG_C_2_S_QUIT_ROOM:
			HanderQuitRoom(gate->m_id, (LMsgC2SQuitRoom*)msg->m_userMsg);
			break;
		case MSG_C_2_S_ADD_BLOCK_TO_CHESS:
			HanderAddBlockToChess((LMsgC2SAddBlockToChess*)msg->m_userMsg);
			break;
		case MSG_C_2_S_CHESS_BLOCK_MOVE:
			HanderMoveChessBlock((LMsgC2SChessBlockMove*)msg->m_userMsg);
			break;
		case MSG_C_2_S_CHESS_BLOCK_ATTACK:
			HanderChessBlockAttack((LMsgC2SChessBlockAttack*)msg->m_userMsg);
			break;
		case MSG_C_2_S_CHESS_BLOCK_UPGRADE:
			HanderChessBlockUpgrade((LMsgC2SChessBlockUpgrade*)msg->m_userMsg);
			break;
		case MSG_C_2_S_CHESS_BLOCK_ZHUI_SHA:
			HanderChessBlockZhuiSha((LMsgC2SChessBlockZhuiSha*)msg->m_userMsg);
			break;
		}
	}
	else
	{
		//LLOG_ERROR("Work::HanderGateUserMsg msg error %d:%ll:%d", gateId, gateUserId, msg->m_userMsgId);
	}
}

Lstring Work::Md5Hex(const void* src ,Lsize len)
{
	unsigned char sign[17];
	memset(sign,0,sizeof(sign));

	MD5((const unsigned char*)src,len,sign);

	//转换成16进制
	char signHex[33];
	memset(signHex,0,sizeof(signHex));
	for(int i = 0 ; i < 16; ++i)
	{
		sprintf(signHex + i*2,"%02x", sign[i]);
	}

	return std::string(signHex);
}



void Work::SendLogicLoginToLM()
{
	LMsgL2LMGLogin login;
	login.m_id = gConfig.GetServerID();
	login.m_ip = gConfig.GetInsideIp();
	login.m_key = gConfig.GetCenterKey();
	login.m_port = gConfig.GetInsidePort();
	login.m_exclusive_play = gConfig.GetExclusivePlay();
	m_logicManager->Send(login.GetSendBuff());
	LLOG_ERROR("Login logicmanager succeed.");
	m_loginLMFirst = false;
}



void Work::HanderKeepAlive(LMsgS2SKeepAlive* msg)
{
	if(msg == NULL || !msg->m_sp)
		return;
	LLOG_DEBUG("KeepAlive from %s:%d received.", msg->m_sp->GetRemoteIp().c_str(), msg->m_sp->GetRemotePort());

	LMsgS2SKeepAliveAck ack;
	msg->m_sp->Send(ack.GetSendBuff());
}

void Work::HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg)
{
	if(msg == NULL)
		return;

	if(m_logicManager == msg->m_sp)
		m_logicManager->OnKeepAliveAck();
	
}

void Work::HanderQuitRoom(int gate_id,LMsgC2SQuitRoom* msg)
{
	if (msg == nullptr)
		return;

	std::shared_ptr<Room> room = gRoomManager.GetRoomById(msg->m_room_id);
	if (room)
	{
		LUserPtr user = room->GetRoomUser(msg->m_user_id);
		if (nullptr == user)
			return;

		if (room->DelUser(msg->m_user_id))
		{
		
			LMsgL2LMQuitRoom send;
			send.m_users.push_back(msg->m_user_id);
			send.m_room_id = msg->m_room_id;

			SendToLogicManager(send);

			if (room->IsEmpty())
			{
				gRoomManager.DeleteRoom(room->GetRoomId());
			}


			LMsgS2CQuitRoom quit_msg;
			quit_msg.m_result = 0;
			quit_msg.m_room = msg->m_room_id;

			user->Send(quit_msg);
		}

	}
}



void Work::HanderQuickCreateRoomOpt(LMsgLM2LQuickCreateRoomOpt* msg)
{
	LMsgL2LMQuickCreateRoomOpt send;
	int result = 0;
	do
	{
		int room_id = msg->m_room_id;
		if (gRoomManager.GetRoomById(room_id))
		{
			result = 2;
			break;
		}

		std::shared_ptr<Room> room = gRoomManager.CreateRoom(room_id);
		

		for (int i = 0; i < msg->m_users.size(); ++i)
		{
			LUser& msgUser = msg->m_users[i];
			LUserPtr user = std::make_shared<User>(msgUser.m_user_id, msgUser.m_name, msgUser.m_headImageUrl, msgUser.m_gate_id, msgUser.m_bag_blocks);
			
			room->AddUser(user,true);

			send.m_users.push_back(msgUser.m_user_id);
		}

		if (room->StartGame())
		{
			LMsgS2CQuickRoomResult msg;
			room->FillRoomMsg(msg.m_room);
			room->Broadcast(msg);

		}

	} while (0);

	//chu cuo
	//if (result)
	{
		send.m_result = result;
		msg->m_sp->Send(send.GetSendBuff());
	}
	
}

void Work::HanderLM2LUserLogout(LMsgLM2LUserLogOut* msg)
{
	if (nullptr == msg)
		return;
	std::shared_ptr<Room> room = gRoomManager.GetRoomById(msg->m_room_id);
	if (room)
	{
		LUserPtr user = room->GetRoomUser(msg->m_user_id);
		if (user)
		{
			user->SetOnline(false);
			user->SetGateServerId(0);
		}
	}

}

void Work::HanderLM2LUserLogin(LMsgLM2LUserLogin* msg)
{
	if (nullptr == msg)
		return;
	std::shared_ptr<Room> room = gRoomManager.GetRoomById(msg->m_room_id);
	if (room)
	{
		LUserPtr user = room->GetRoomUser(msg->m_user_id);
		if (user)
		{
			user->SetOnline(true);
			user->SetGateServerId(msg->m_gate_id);

			LMsgS2CRoomInfoWhenRelogin send;
			//room->FillRoomInfo(user->GetUserId(), send.m_room);
			user->Send(send);
		}
	}
}


//**********************************************
void	Work::HanderCreateXiangQiRoom(LMsgS2SCreateXiangQiRoom* msg)
{
	/*if (nullptr == msg)
		return;

	std::shared_ptr<Room> room = gRoomManager.GetRoomById(msg->m_room_id);
	if (room != nullptr)
		return;

	room = gRoomManager.CreateRoom(msg->m_room_id);

	LUserPtr user  = std::make_shared<User>(msg->m_self_info.m_user_id, msg->m_self_info.m_name, msg->m_self_info.m_head_icon, msg->m_self_info.m_gate_id, msg->m_self_info.m_robot);
	for (int i = 0; i < msg->m_blocks.size(); ++i)
	{
		LChessBlock block = msg->m_blocks[i];
		user->m_bag_blocks.push_back(block);
	}


	room->AddUser(user);

	LMsgL2LMCreateRoomResult send;
	send.m_user_id = msg->m_user_id;
	send.m_room_id = msg->m_room_id;
	
	SendToLogicManager(send);*/
}

void Work::HanderAddInToXiangQiRoom(LMsgS2SAddIntoXiangQiRoom* msg)
{
	/*int result = 0;
	int user_id = msg->m_self_info.m_user_id;
	do 
	{
		if (nullptr == msg)
			return;

		std::shared_ptr<Room> room = gRoomManager.GetRoomById(msg->m_room_id);
		if (room == nullptr)
		{
			result = 1;
			break;
		}
			

		if (room->IsFull())
		{
			result = 2;
			break;
		}

		LUserPtr user = room->GetRoomUser(user_id);
		if (nullptr == user)
		{
			result = 3;
			break;
		}

		user = std::make_shared<User>(msg->m_self_info.m_user_id, msg->m_self_info.m_name, msg->m_self_info.m_head_icon, msg->m_self_info.m_gate_id, msg->m_self_info.m_robot);
		for (int i = 0; i < msg->m_blocks.size(); ++i)
		{
			LChessBlock block = msg->m_blocks[i];
			user->m_battle_blocks.push_back(block);
		}


		room->AddUser(user);

		

	} while (0);

	LMsgL2LMAddIntoRoomResult send;
	send.m_result = result;
	send.m_user_id = user_id;
	send.m_room_id = msg->m_room_id;

	SendToLogicManager(send);*/
}

void Work::HanderReadyOpt(LMsgC2SReadyOpt* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	
}


void Work::HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	room->HanderAddBlockToChess(msg);
}


void Work::HanderMoveChessBlock(LMsgC2SChessBlockMove* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	room->HanderMoveChessBlock(msg);
}


void Work::HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	room->HanderChessBlockAttack(msg);
}

void Work::HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	room->HanderChessBlockUpgrade(msg);
}

void Work::HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg)
{
	if (nullptr == msg)
		return;

	std::shared_ptr<Room>	room = gRoomManager.GetRoomById(msg->m_room_id);
	if (nullptr == room)
		return;

	room->HanderChessBlockZhuiSha(msg);
}



