#pragma once
#include "LBase.h"
#include "LSingleton.h"
#include "LMsg.h"

#include <vector>

#include "ChessGame.h"
#include "User.h"


class Room
{
public:
	Room(int room_id);
	~Room();
	void Clear();

	bool AddUser(LUserPtr user,bool quick_start = false);
	bool DelUser(Lint user_id);

	LUserPtr GetRoomUser(Lint user_id);

	void Tick();

	int GetRoomId()
	{
		return m_room_id;
	}

	bool IsInGame()
	{
		return m_room_status == RS_GAME;
	}

	bool IsFull()
	{
		return m_room_user.size() == ROOM_USER_CNT;
	}

	bool IsEmpty()
	{
		return m_room_user.size() == 0;
	}

	int GetRoomStatus()
	{
		return m_room_status;
	}

	std::vector<LUserPtr> GetAllUsers()
	{
		return m_room_user;
	}

	void GameOver()
	{
		m_room_status = RS_WAIT;
	}

public:
	void CheckStart();
	bool StartGame();

	
	void Broadcast(LMsgSC& msg);

	void SendToUser(int user_id,LMsgSC& msg);

	void CaculStar();

public:
	//void	SendRoomUserModify(LUserPtr user, ROOMUSER_MODIFY type);
	void BrocastRoomModify();
public:
	void HanderUserReadyOpt(LMsgC2SReadyOpt* msg);
	void FillRoomMsg(RoomMsg& room);
	void HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg);
	void HanderMoveChessBlock(LMsgC2SChessBlockMove* msg);
	void HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg);
	void HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg);
	void HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg);
private:
	Lint m_room_id;
	std::vector<LUserPtr>				m_room_user;
	Lint							m_room_status;
	Lint							m_create_time; //创建房间的时间
	std::shared_ptr<ChessGame>		m_game;
};


class RoomManager : public LSingleton<RoomManager>
{
public:
	virtual	bool	Init();
	virtual	bool	Final();

public:
	void Tick();

public:
	std::shared_ptr<Room>		GetRoomById(Lint room_id);
	std::shared_ptr<Room>		CreateRoom(Lint room_id);

	bool			IsHaveRoom(int user_id);

	bool			DeleteRoom(Lint room_id);

private:
	std::map<Lint, std::shared_ptr<Room>>	m_map_rooms;

	//玩家对应的桌子id
	std::map<Lint, Lint>		m_user_room;
};


#define gRoomManager   RoomManager::Instance()