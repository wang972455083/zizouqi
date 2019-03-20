#include "Work.h"
#include "LLog.h"
#include "Config.h"
#include "DbServer.h"
#include "InsideNet.h"
#include "UserManager.h"
#include "ChecssBlockManager.h"

//#include "RemoteLogThread.h"

#include "Base64.h"
#include "BaseCommonFunc.h"
#include "Table/TableManager.h"


//初始化
bool Work::Init()
{
	//首先设置log文件，不然log没有开启，log文件没办法查看
	LLog::Instance().SetFileName("GameServer");

	//初始化配置文件
	if(!gConfig.Init())
	{
		LLOG_ERROR("gConfig.Init error");
		return false;
	}

	//设置log级别
	LLog::Instance().SetLogLevel(gConfig.GetLogLevel());

	//网络消息初始化
	LMsgFactory::Instance().Init();

	//数据库初始化
	if(!gDbServer.Init())
	{
		LLOG_ERROR("gDbServer.Init error");
		return false;
	}
	if(!m_dbsession.Init(gConfig.GetDbHost(),gConfig.GetDbUser(),gConfig.GetDbPass(),gConfig.GetDbName(),"utf8mb4",gConfig.GetDbPort()))
	{
		return false;
	}
	/*if(!gUserManager.Init())
	{
		LLOG_ERROR("gUserManager.Init error");
		return false;
	}*/

	LLOG_ERROR("Load users successfully");

	//外部端口初始化
	if (!gOutsideNet.Init())
	{
		LLOG_ERROR("gInsideNet.Init error");
		return false;
	}

	TableManager::Instance().Init();
	gUserManager.Init();
	gChessBlockManager.Init();

	LTime cur;
	m_30MSceTick = cur.MSecs();
	m_200MSceTick = cur.MSecs();
	m_1SceTick = cur.MSecs();
	m_30SceTick = cur.MSecs();
	m_600SceTick = cur.MSecs();

	

	return true;
}

bool Work::Final()
{
	return true;
}

//启动
void Work::Start()
{
	//主线程开启
	LRunnable::Start();

	//gRLT.start();

	//数据库线程开启
	gDbServer.Start();

	//外部网络开启
	gOutsideNet.Start();

	//gm网络开启
	gGmNet.Start();

	ConnectToLogicManager();

	m_gcService.Start();
}

//等待
void Work::Join()
{
	gGmNet.Join();

	//等待网络线程终止
	gOutsideNet.Join();

	//等待数据库线程终止
	gDbServer.Join();


	//等待逻辑线程终止
	LRunnable::Join();

}

//停止
void Work::Stop()
{
	m_gateInfo.clear();

	gGmNet.Stop();

	gOutsideNet.Stop();

	gDbServer.Stop();

	//gRLT.stop();

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
		m_1SceTick = cur.MSecs();
	}

	//30秒循环一次
	if (cur.MSecs() - m_30SceTick > 30*1000)
	{
		m_30SceTick = cur.MSecs();
		//SaveCurrentOnline();

		
	}

	//5分钟循环一次
	if (cur.MSecs() - m_600SceTick > 5*60 * 1000)
	{
		m_600SceTick = cur.MSecs();
	}

	if (m_logicManager)
		m_logicManager->OnTick(cur);
}

LTime& Work::GetCurTime()
{
	return m_tickTimer;
}

void Work::HanderMsg(LMsg* msg)
{
	if (msg == NULL)
	{
		return;
	}
	//玩家请求登录
	switch(msg->m_msgId)
	{
	case MSG_CLIENT_KICK:
		HanderUserKick((LMsgKick*)msg);
		break;
	case MSG_CLIENT_IN:
		HanderClientIn((LMsgIn*)msg);
		break;
	case MSG_G_2_SERVER_LOGIN:
		HanderGateLogin((LMsgG2ServerLogin*)msg);

	case MSG_S_2_S_KEEP_ALIVE:
		HanderKeepAlive((LMsgS2SKeepAlive*)msg);
		break;
	case MSG_S_2_S_KEEP_ALIVE_ACK:
		HanderKeepAliveAck((LMsgS2SKeepAliveAck*)msg);
		break;
	case MSG_G_2_GAME_USER_MSG:
		HanderGateUserMsg((LMsgG2GameUserMsg*)msg);
		break;
	case MSG_G_2_S_USER_OUT:
		HanderUserLogOut((LMsgG2SUserLogOut*)msg);
		break;
	default:
		LLOG_ERROR("Message Not Handle,Id=%d",msg->m_msgId);
		break;
	}
}

void Work::HanderGateUserMsg(LMsgG2GameUserMsg*msg)
{
	if (nullptr == msg)
		return;

	switch (msg->m_userMsgId)
	{
	case MSG_C_2_S_LOGIN:
		HanderUserLogin(msg->m_sp,(LMsgC2SLogin*)msg->m_userMsg);
	break;
	case MSG_C_2_S_CREATE_XIANGQI_ROOM:
		HanderUserCreateXiangQiRoom((LMsgC2SCreateXiangQiRoom*)msg->m_userMsg);
	break;
	case MSG_C_2_S_QUICK_ROOM_OPT:
		HanderUserQuickStart((LMsgC2SQuickRoomOpt*)msg->m_userMsg);
		break;

	case MSG_C_2_S_GET_BAG_BLOCK_LIST:
		HanderUserGetBagBlockList(msg->m_sp,(LMsgC2SGetBagBlockList*)msg->m_userMsg);
		break;

	case MSG_C_2_S_BAG_BLOCK_UPGRADE:
		HanderUserbagBlockUpgrade(msg->m_sp, (LMsgC2SBagBlockUpgrade*)msg->m_userMsg);
		break;
	default:
		break;
	}
}

void Work::HanderUserLogin(LSocketPtr sp,LMsgC2SLogin* msg)
{
	int user_id = msg->m_user_id;

	UserPtr user = gUserManager.GetUserById(user_id);
	if (nullptr == user)
	{
		/*user = std::make_shared<DUser>();

		user->m_usert.m_user_id = user_id;
		user->m_usert.m_name = msg->m_location;
		user->m_usert.m_sex = 1;
		user->m_usert.m_money = 100;

		gUserManager.SaveUser(user);*/

		user = gUserManager.CreateUser(msg->m_user_id, msg->m_location);
	}


	user->SetOnline(true);
	user->SetSp(sp);

	LMsgS2CLogin send;
	send.m_errorCode = 0;
	send.m_user_id = user->m_usert.m_user_id;;
	send.m_name = user->m_usert.m_name;
	send.m_head_image = user->m_usert.m_headImageUrl;
	send.m_sex = user->m_usert.m_sex;
	send.m_money = user->m_usert.m_money;

	SendUserMsg(sp, user_id, send);
	
}

void Work::HanderUserLogOut(LMsgG2SUserLogOut* msg)
{
	if (nullptr == msg)
		return;

	UserPtr user = gUserManager.GetUserById(msg->m_user_id);
	if (user)
	{
		user->SetOnline(false);
		user->SetSp(nullptr);
	}
}

void Work::SendUserMsg(LSocketPtr sp,int user_id,LMsg& msg)
{
	
	if (sp)
	{
		LMsgGame2GUserMsg send;
		send.m_user_id = user_id;
		send.m_dataBuff = msg.GetSendBuff();
		sp->Send(send.GetSendBuff());
	}
}


void Work::HanderGateLogin(LMsgG2ServerLogin* msg)
{
	LLOG_ERROR("Work::HanderGateLogin Gate Login: %d", msg->m_id);

	if (msg->m_key.empty())
	{
		msg->m_sp->Stop();
		LLOG_ERROR("Work::HanderGateLogin key error %d %s", msg->m_id, msg->m_key.c_str());
		return;
	}

	auto iter = m_gateInfo.find(msg->m_id);
	if (iter != m_gateInfo.end())
	{
		if (iter->second.m_sp)
		{
			if (iter->second.m_sp != msg->m_sp)
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

	
}

void Work::DelGateInfo(Lint id)
{
	auto iter = m_gateInfo.find(id);
	if (iter != m_gateInfo.end())
	{
		m_gateInfo.erase(iter);
	}
}

void Work::AddSp(LSocketPtr sp)
{
	//m_spMap[sp] = GetCurTime().Secs();
}

void Work::DelSp(LSocketPtr sp)
{
	
	
}

void Work::HanderUserKick(LMsgKick* msg)
{
	if (m_logicManager == msg->m_sp)
	{
		LLOG_ERROR("LogicManager DisConnect!");
		return;
	}

	HanderGateLogout(msg);
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


void Work::HanderClientIn(LMsgIn* msg)
{
	if (msg == NULL)
	{
		return;
	}

	if (msg->m_sp == m_logicManager)
	{
		SendGameLoginToLM();
	}

}



bool Work::HanderCheckMd5(std::map<Lstring, Lstring>& param)
{
	return true;
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
	if (msg == NULL)
		return;

	if (m_logicManager == msg->m_sp)
		m_logicManager->OnKeepAliveAck();

}

DBSession& Work::GetDbSession()
{
	return m_dbsession;
}











template<typename T> string Work::toString(const T& t)
{
	ostringstream oss;  //创建一个格式化输出流
	oss << t;             //把值传递如流中
	return oss.str();   
}


void Work::ConnectToLogicManager()
{
	KeepAliveSetting setting = { true, 15000, 5000 };
	MsgPackReceiver receiver = [this](LBuffPtr recv, LSocketPtr s)
	{
		LMsg* msg = ::ParseInsideNetPacket(recv, s);
		if (msg)
			this->Push(msg);
	};

	m_logicManager.reset(new TCPClient(gOutsideNet, gConfig.GetLogicManagerIp(), gConfig.GetLogicManagerPort(), setting, receiver));
	bool succeed = m_logicManager->Connect();
	if (!succeed)
		LLOG_ERROR("Failed to connect with logicmanager.");
}

void Work::SendGameLoginToLM()
{
	LMsgGame2LMGLogin login;
	login.m_id = gConfig.GetId();
	login.m_ip = gConfig.GetOutsideIp();
	login.m_port = gConfig.GetOutsidePort();
	m_logicManager->Send(login.GetSendBuff());
	LLOG_ERROR("Login LogicManager succeed.");
}
//获取上阵棋子的信息
void Work::HanderUserCreateXiangQiRoom(LMsgC2SCreateXiangQiRoom* msg)
{
	if (nullptr == msg)
		return;

	UserPtr user = gUserManager.GetUserById(msg->m_user_id);
	if (nullptr == user)
		return;
	LMsgS2SCreateXiangQiRoom send;
	send.m_user_id = msg->m_user_id;

	send.m_self_info.m_user_id = msg->m_user_id;
	send.m_self_info.m_name = user->m_usert.m_name;
	send.m_self_info.m_head_icon = user->m_usert.m_headImageUrl;

	list<int> battle_list = gChessBlockManager.GetBattleListByUserId(msg->m_user_id);
	for (auto it = battle_list.begin();it != battle_list.end(); ++it)
	{
		BlockPtr block = gChessBlockManager.GetChessBlockById(*it);
		if(block == nullptr)
			continue;

		send.m_blocks.push_back(block->m_block);
	}




	m_logicManager->Send(send.GetSendBuff());
}


void Work::HanderUserAddIntoXiangQiRoom(LMsgC2SAddIntoRoom* msg)
{
	if (nullptr == msg)
		return;

	UserPtr user = gUserManager.GetUserById(msg->m_user_id);
	if (nullptr == user)
		return;
	LMsgS2SAddIntoXiangQiRoom send;
	send.m_user_id = msg->m_user_id;
	send.m_room_id = msg->m_room_id;
	send.m_self_info.m_user_id = msg->m_user_id;
	send.m_self_info.m_name = user->m_usert.m_name;
	send.m_self_info.m_head_icon = user->m_usert.m_headImageUrl;

	list<int> battle_list = gChessBlockManager.GetBattleListByUserId(msg->m_user_id);
	for (auto it = battle_list.begin(); it != battle_list.end(); ++it)
	{
		BlockPtr block = gChessBlockManager.GetChessBlockById(*it);
		if (block == nullptr)
			continue;

		send.m_blocks.push_back(block->m_block);
	}




	m_logicManager->Send(send.GetSendBuff());
}

void Work::HanderUserQuickStart(LMsgC2SQuickRoomOpt* msg)
{
	if (nullptr == msg)
		return;

	UserPtr user = gUserManager.GetUserById(msg->m_user_id);
	if (nullptr == user)
		return;
	LMsgGame2LMQuickStartOpt send;

	send.m_type = msg->m_type;
	send.m_user.m_user_id = msg->m_user_id;
	//快速匹配
	if (msg->m_type == 0)
	{
		
		send.m_user.m_name = user->m_usert.m_name;
		send.m_user.m_headImageUrl = user->m_usert.m_headImageUrl;

		list<int> battle_list = gChessBlockManager.GetBattleListByUserId(msg->m_user_id);
		for (auto it = battle_list.begin(); it != battle_list.end(); ++it)
		{
			BlockPtr block = gChessBlockManager.GetChessBlockById(*it);
			if (block == nullptr)
				continue;

			send.m_user.m_bag_blocks.push_back(block->m_block);
		}

	}

	m_logicManager->Send(send.GetSendBuff());
}

void Work::HanderUserGetBagBlockList(LSocketPtr sp,LMsgC2SGetBagBlockList* msg)
{
	if (nullptr == msg)
		return;

	UserPtr user = gUserManager.GetUserById(msg->m_user_id);
	if (nullptr == user)
		return;

	LMsgS2CGetBagBlockList send;
	

	list<int> battle_list = gChessBlockManager.GetBattleListByUserId(msg->m_user_id);
	for (auto it = battle_list.begin(); it != battle_list.end(); ++it)
	{
		BlockPtr block = gChessBlockManager.GetChessBlockById(*it);
		if (block == nullptr)
			continue;

		S2CBlock msg_block;
		msg_block.m_block_id = block->m_block.m_block_id;
		msg_block.m_type= block->m_block.m_type;
		msg_block.m_hp = block->m_block.m_hp;
		msg_block.m_max_hp = block->m_block.m_max_hp;
		msg_block.m_attack = block->m_block.m_attack;
		msg_block.m_max_attack = block->m_block.m_max_attack;
		msg_block.m_can_upgrade = block->m_block.m_can_upgrade;

		send.m_bag_blocks.push_back(msg_block);
	}

	SendUserMsg(sp, msg->m_user_id, send);
}

void Work::HanderUserbagBlockUpgrade(LSocketPtr sp, LMsgC2SBagBlockUpgrade* msg)
{
	if (nullptr == msg)
		return;

	LMsgS2CBagBlockUpgrade send;
	send.m_block_id = msg->m_block_id;
	send.m_type = msg->m_type;

	bool exist = false;
	list<int> battle_list = gChessBlockManager.GetBattleListByUserId(msg->m_user_id);
	for (auto it = battle_list.begin(); it != battle_list.end(); ++it)
	{

		if (*it == msg->m_block_id)
		{
			exist = true;
			break;
		}
	}


	int result = 1;
	if (exist)
	{
		BlockPtr block = gChessBlockManager.GetChessBlockById(msg->m_block_id);
		if (block == nullptr)
			return;

		switch (msg->m_type)
		{
		case BP_HP:
		{
			if (block->m_block.m_hp < block->m_block.m_max_hp)
			{
				block->m_block.m_hp++;
				gChessBlockManager.Save(block);
				result = 0;
				break;
			}
		}
		break;
		case BP_MAX_HP:
		{
		}
		break;
		case BP_ATTACK:
		{
			if (block->m_block.m_attack < block->m_block.m_max_attack)
			{
				block->m_block.m_attack++;
				gChessBlockManager.Save(block);
				result = 0;
				break;
			}
		}
		break;
		case BP_MAX_ATTACK:
		{
		}
		break;
		}
	}

	send.m_result = result;
	SendUserMsg(sp, msg->m_user_id, send);
}