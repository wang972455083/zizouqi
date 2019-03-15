#ifndef _ROOM_MANAGER_H
#define _ROOM_MANAGER_H

#include "LBase.h"
#include "LSingleton.h"
#include "LMsgS2S.h"



//ֻ����ID��logicserverid
class Room
{
public:
	Room()
	{
		Init();
	}

	void Init()
	{
		m_room_id = 0;

		m_logic_server_id = 0;
	}

public:
	void AddUser(int user_id)
	{
		m_users.push_back(user_id);
	}

	void DelUser(int user_id)
	{
		for (auto it = m_users.begin(); it != m_users.end();)
		{
			if (*it == user_id)
			{
				it = m_users.erase(it);
			}
			else
			{
				++it;
			}
		}
		
	}

	bool IsFull()
	{
		return m_users.size() >= 2;
	}

	bool IsEmpty()
	{
		return m_users.size() == 0;
	}
	
public:
	Lint m_room_id;
	
	Lint m_logic_server_id;

	std::vector<Lint>	m_users;
};

typedef std::shared_ptr<Room>  RoomPtr;


class RoomManager:public LSingleton<RoomManager>
{
public:
	virtual	bool	Init();
	virtual	bool	Final();

	//void			RecycleRoomId(LMsgL2LMGRecyleRoomID* msg);

	Lint			GetFreeRoomId();

	Lint			getServerIDbyRoomID(Lint nRoomID);

	void			RecycleDumpServer(Lint nLogicID);

	RoomPtr			CreateRoom(Lint Room_id,int logic_server_id);
	bool			DeleteRoom(int Room_id);
	RoomPtr			GetRoom(int Room_id);
	void			RecycleRoomID(Lint Room_id);  //����Room_id

	
private:
	std::queue<Lint>	m_FreeRoomList;
	
	std::queue<Lint>	m_sharevideoId;
	//���д���������
	std::map<Lint, RoomPtr>		m_Rooms;
	
};
#define gRoomManager RoomManager::Instance()

#endif