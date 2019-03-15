#include "RoomManager.h"
#include "LMsgS2S.h"
#include "Work.h"
#include "LMsg.h"


Room::Room(int room_id)
{
	Clear();
	m_room_id = room_id;

	LTime t;
	m_create_time = t.GetSec();
	m_game = nullptr;
}

Room::~Room()
{
	Clear();
}

void Room::Clear()
{
	m_room_id = 0;
	
	m_room_status = 0;
	m_room_user.clear();
	m_room_status = RS_WAIT;
}

bool Room::AddUser(LUserPtr user, bool quick_start)
{
	if (nullptr == user)
		return false;

	if (IsFull())
		return false;

	m_room_user.push_back(user);

	if (!quick_start)
	{
		BrocastRoomModify();
	}
	

	return true;
}

bool Room::DelUser(Lint user_id)
{
	bool del = false;

	for (auto it = m_room_user.begin(); it != m_room_user.end();)
	{
		if ((*it)->GetUserId() == user_id)
		{
			it = m_room_user.erase(it);
			del = true;
			break;
		}
		else
		{
			++it;
		}
	}

	if (del)
	{
		BrocastRoomModify();
	}

	return del;
}


LUserPtr Room::GetRoomUser(Lint user_id)
{
	for (size_t i = 0; i < m_room_user.size(); i++)
	{
		if (m_room_user[i]->GetUserId() == user_id)
			return m_room_user[i];
	}

	return nullptr;
}

void Room::Tick()
{
	
}


void Room::CheckStart()
{
	if (!IsFull())
		return;

	for (int i = 0; i < m_room_user.size(); ++i)
	{
		if (m_room_user[i]->GetStatus() != ST_READY)
			return;
	}

	StartGame();
}

bool Room::StartGame()
{
	m_room_status = RS_GAME;

	m_game = std::make_shared<ChessGame>();
	if (m_game)
	{
		m_game->Start(this);

		return true;
	}

	return false;
}


void Room::Broadcast(LMsgSC& msg)
{
	for (int i = 0; i < m_room_user.size(); ++i)
	{
		std::shared_ptr<User> user = m_room_user[i];
		if (user)
		{
			user->Send(msg);
		}
	}	
}

void Room::SendToUser( int user_id,LMsgSC& msg)
{
	for (int i = 0; i < m_room_user.size(); ++i)
	{
		std::shared_ptr<User> user = m_room_user[i];
		if (user->GetUserId() == user_id)
		{
			user->Send(msg);
		}
	}
}


void Room::BrocastRoomModify()
{
	if (IsEmpty())
		return;

	LMsgS2CRoomInfoModify send;
	send.m_room_id = m_room_id;

	for (int i = 0; i < m_room_user.size(); ++i)
	{
		S2CRoomUser sUser;
		m_room_user[i]->FillRoomUserMsg(sUser);
	}

	
	Broadcast(send);
}

void Room::CaculStar()
{

}

void Room::HanderUserReadyOpt(LMsgC2SReadyOpt* msg)
{
	if (nullptr == msg)
		return;

	int status = ST_NULL;
	//zhuangbeo
	if (msg->m_type == LMsgC2SReadyOpt::OPT_READY)
	{
		status = ST_READY;
	}

	LUserPtr user = GetRoomUser(msg->m_user_id);
	if (nullptr == user)
		return;

	if (user->GetStatus() == status)
		return;

	LMsgS2CReadyOpt send;
	send.m_result = 0;
	send.m_room_id = m_room_id;
	send.m_type = msg->m_type;
	send.m_target_user = msg->m_user_id;
	
	Broadcast(send);

	if (msg->m_type == LMsgC2SReadyOpt::OPT_READY)
	{
		CheckStart();
	}
}

void Room::FillRoomMsg(RoomMsg& room)
{
	room.m_room_id = m_room_id;
	room.m_status = m_room_status;

	for (int i = 0; i < m_room_user.size(); ++i)
	{
		S2CRoomUser sUser;
		m_room_user[i]->FillRoomUserMsg(sUser);

		room.m_users.push_back(sUser);
	}

	if (m_game)
	{
		m_game->FillGameInfoMsg(room.m_game);
	}
}


void Room::HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg)
{
	if (m_game)
	{
		m_game->HanderAddBlockToChess(msg);
	}
}

void Room::HanderMoveChessBlock(LMsgC2SChessBlockMove* msg)
{
	if (m_game)
	{
		m_game->HanderMoveChessBlock(msg);
	}
}

void Room::HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg)
{
	if (m_game)
	{
		m_game->HanderChessBlockAttack(msg);
	}
}


void Room::HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg)
{
	if (m_game)
	{
		m_game->HanderChessBlockUpgrade(msg);
	}
}

void Room::HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg)
{
	if (m_game)
	{
		m_game->HanderChessBlockZhuiSha(msg);
	}
}


bool RoomManager::Init()
{
	return true;
}

bool RoomManager::Final()
{
	return true;
}

void RoomManager::Tick()
{
	for (auto iter = m_map_rooms.begin(); iter != m_map_rooms.end(); ++iter)
	{
		iter->second->Tick();
	}
}

std::shared_ptr<Room> RoomManager::GetRoomById(Lint room_id)
{
	auto it = m_map_rooms.find(room_id);
	if (it != m_map_rooms.end())
		return it->second;

	return nullptr;
}

std::shared_ptr<Room> RoomManager::CreateRoom(Lint room_id)
{
	std::shared_ptr<Room> room = std::make_shared<Room>(room_id);

	if (room == nullptr)
		return nullptr;

	m_map_rooms[room_id] = room;

	return room;
}

bool RoomManager::IsHaveRoom(int user_id)
{
	auto it = m_user_room.find(user_id);
	if (it != m_user_room.end())
		return true;

	return false;
}



bool RoomManager::DeleteRoom(Lint room_id)
{
	auto it = m_map_rooms.find(room_id);
	if (it != m_map_rooms.end())
	{
		m_map_rooms.erase(it);

		return true;
	}

	return false;
}


