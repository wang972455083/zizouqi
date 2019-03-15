#include "User.h"



User::User(int user_id, Lstring name, Lstring head_icon, int gate_server_id, std::vector<LChessBlock>& bag_blocks)
{
	m_user_id = user_id;
	m_name = name;
	m_head_icon = head_icon;

	m_status = 0;
	m_gate_id = gate_server_id;

	m_star = 10;

	m_robot = false;
	

	m_desk_id = 0;
	
	m_online = true;

	m_bag_blocks.clear();
	for (int i = 0; i < bag_blocks.size(); ++i)
	{
		m_bag_blocks.push_back(bag_blocks[i]);
	}

}

User::~User()
{
	Clear();
}

void User::Clear()
{
	m_user_id = 0;
	m_name = "";

	m_status = 0;
}

Lint User::GetUserId()
{
	return m_user_id;
}


void User::SetUserId(Lint user_id)
{
	m_user_id = user_id;
}



Lint User::GetStatus()
{
	return m_status;
}

void User::SetStatus(Lint status)
{
	m_status = status;
}


void User::Send(LMsgSC& msg)
{
	if (!m_online || m_robot)
		return;

	msg.m_user_id = m_user_id;

	GateInfo* info = gWork.GetGateInfoById(m_gate_id);
	if (info)
	{
		LMsgL2GUserMsg sendMsg;
		sendMsg.m_user_id = m_user_id;
		sendMsg.m_user_msg_id = msg.m_msgId;
		sendMsg.m_dataBuff = msg.GetSendBuff();

		info->m_sp->Send(sendMsg.GetSendBuff());
	}
}


bool User::IsRobot()
{
	return m_robot;
}

void User::SetOnline(bool online)
{
	m_online = online;
}

bool User::IsOnline()
{
	return m_online;
}

void User::FillRoomUserMsg(S2CRoomUser& user)
{
	user.m_user_id = m_user_id;
	user.m_name = m_name;
	user.m_head_icon = m_head_icon;
	user.m_status = m_status;
	for (int i = 0; i < m_bag_blocks.size(); ++i)
	{
		S2CBlock sblock;
		sblock.m_block_id = m_bag_blocks[i].m_block_id;
		sblock.m_type = m_bag_blocks[i].m_type;
		sblock.m_hp = m_bag_blocks[i].m_hp;
		sblock.m_max_hp = m_bag_blocks[i].m_max_hp;
		sblock.m_attack = m_bag_blocks[i].m_attack;
		sblock.m_max_attack = m_bag_blocks[i].m_max_attack;
		sblock.m_can_upgrade = m_bag_blocks[i].m_can_upgrade;

		user.m_battle_blocks.push_back(sblock);
	}
}







