#pragma once

#include "LTool.h"


struct S2CBlock
{
	int		m_block_id;
	int		m_type;
	int		m_hp;
	int		m_max_hp;
	int		m_attack;
	int		m_max_attack;
	bool	m_can_upgrade;

	S2CBlock()
	{
		m_block_id = 0;
		m_type = 0;
		m_hp = 0;
		m_max_hp = 0;
		m_attack = 0;
		m_max_attack = 0;
		m_can_upgrade = false;
	}

	MSGPACK_DEFINE(m_block_id, m_type, m_hp, m_max_hp, m_attack, m_max_attack, m_can_upgrade);
};

struct S2CRoomUser
{
	int   m_user_id;
	Lstring m_name;
	Lstring m_head_icon;
	int		m_status;
	std::vector<S2CBlock>	m_battle_blocks;		//shangzhendeqizi

	MSGPACK_DEFINE(m_user_id, m_name, m_head_icon, m_status,m_battle_blocks);

public:
	S2CRoomUser()
	{
		m_user_id = 0;
	}

	S2CRoomUser& operator = (const S2CRoomUser& other)
	{
		if (&other == this)
			return *this;

		m_user_id = other.m_user_id;
		m_name = other.m_name;
		m_head_icon = other.m_head_icon;

		return *this;
	}
};


//游戏中棋子使用信息
struct BlockUse
{
	int m_block_id;
	int m_use_cnt;  //使用次数
	MSGPACK_DEFINE(m_block_id, m_use_cnt);
};
struct Fighter
{
	int m_user_id;
	int m_color;
	int m_money;
	std::vector<BlockUse>	m_use_data;

	MSGPACK_DEFINE(m_user_id, m_color, m_money, m_use_data);
};

struct GameBlock
{
	int m_color;
	int m_pos_x;
	int m_pos_y;
	S2CBlock	m_block;
	

	GameBlock()
	{
		m_pos_x = 0;
		m_pos_y = 0;
		m_color = 0;
	}

	MSGPACK_DEFINE(m_color, m_pos_x,m_pos_y,m_block);
};


struct GameInfo
{
	int		m_room_id;
	int		m_cur_color;
	std::vector<Fighter>	m_fighters;
	std::vector<GameBlock>	m_game_blocks;  //桌子上的棋子

	MSGPACK_DEFINE(m_room_id, m_cur_color, m_fighters, m_game_blocks);
};

struct RoomMsg
{
	Lint	m_room_id;
	int		m_status;
	std::vector<S2CRoomUser>	m_users;
	
	GameInfo		m_game;

	MSGPACK_DEFINE(m_room_id,m_status,m_users, m_game);

public:
	RoomMsg() :m_room_id(0)
	{

		m_room_id = 0;
		m_status = 0;
	}
};


