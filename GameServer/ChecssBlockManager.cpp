#include "ChecssBlockManager.h"

#include "LLog.h"
#include "LMsgS2S.h"
#include "Work.h"
#include "BaseCommonFunc.h"


bool ChecssBlockManager::Init()
{
	LoadBloackInfo();

	return true;
}

void ChecssBlockManager::LoadBloackInfo()
{
	//加载所有的qizi
	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::LoadBloackInfo mysql null");
		return;
	}

	Lint limitfrom = 0;
	Lint count = 5000000;

	
	while (true)
	{
		std::stringstream ss;
		ss << "select Id, Type, Hp, MaxHp, Attack, MaxAttack, CanUpgrade, InBattle, UserId FROM chessblock ORDER BY Id DESC LIMIT ";
		ss << limitfrom << "," << count;

		if (mysql_real_query(m, ss.str().c_str(), ss.str().size()))
		{
			LLOG_ERROR("UserManager::LoadBloackInfo sql error %s", mysql_error(m));
			break;
		}

		MYSQL_RES* res = mysql_store_result(m);
		MYSQL_ROW row = mysql_fetch_row(res);
		if (!row)
		{
			break;
		}

		while (row)
		{
			++limitfrom;
			BlockPtr block = std::make_shared<DChessBlock>();
			block->m_block.m_block_id = atoi(*row++);
			block->m_block.m_type = atoi(*row++);
			block->m_block.m_hp = atoi(*row++);
			block->m_block.m_max_hp = atoi(*row++);
			block->m_block.m_attack = atoi(*row++);
			block->m_block.m_max_attack = atoi(*row++);
			block->m_block.m_can_upgrade = atoi(*row++);
			block->m_block.m_in_battle = atoi(*row++);
			block->m_user_id = atoi(*row++);
			AddBlock(block);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);
	}
	
	LLOG_ERROR("UserManager::LoadBloackInfo user count %d", limitfrom);
}

bool ChecssBlockManager::Final()
{
	return true;
}



void ChecssBlockManager::Save(BlockPtr block)
{
	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::SaveUser mysql null");
		return;
	}

	
	//查询数据库
	std::stringstream ss;
	ss << "UPDATE chessBlock SET ";
	ss << "Hp='" << block->m_block.m_hp << "',";
	ss << "MaxHp='" << block->m_block.m_max_hp << "',";
	ss << "Attack='" << block->m_block.m_attack << "',";
	ss << "MaxAttack='" << block->m_block.m_max_attack << "',";
	ss << "InBattle='" << block->m_block.m_in_battle << "',";
	ss << "CanUpgrade='" << block->m_block.m_can_upgrade << "' WHERE Id='";
	ss << block->m_block.m_block_id << "'";

	LLOG_DEBUG("ChecssBlockManager::Save sql =%s", ss.str().c_str());

	Lstring* sql = new Lstring(ss.str());
	gDbServer.Push(sql);
}

BlockPtr ChecssBlockManager::GetChessBlockById(Lint block_id)
{
	auto it = m_map_blocks.find(block_id);
	if (it != m_map_blocks.end())
	{
		return it->second;
	}

	return nullptr;
}

void ChecssBlockManager::DelChessBlockById(Lint block_id)
{
	auto it = m_map_blocks.find(block_id);
	if (it != m_map_blocks.end())
	{
		m_map_blocks.erase(it);
	}
}

BlockPtr ChecssBlockManager::CreateBlock(int user_id,int type, int hp, int max_hp, int attack, int max_attack, bool can_upgrade,bool in_battle)
{
	BlockPtr block = std::make_shared<DChessBlock>();
	
	block->m_block.m_type = type;
	block->m_block.m_hp = hp;
	block->m_block.m_max_hp = max_hp;
	block->m_block.m_attack = attack;
	block->m_block.m_max_attack = max_attack;
	block->m_block.m_can_upgrade = can_upgrade;
	block->m_block.m_in_battle = in_battle;
	block->m_user_id = user_id;
	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::CreateUser mysql null");
		return 0;
	}


	std::stringstream ss;
	ss << "INSERT INTO chessBlock (Type,Hp,MaxHp,Attack,MaxAttack,CanUpgrade,InBattle,UserId) VALUES (";
	ss << "'" << block->m_block.m_type << "',";
	ss << "'" << block->m_block.m_hp << "',";
	ss << "'" << block->m_block.m_max_hp << "',";
	ss << "'" << block->m_block.m_attack << "',";
	ss << "'" << block->m_block.m_max_attack << "',";
	ss << "'" << block->m_block.m_can_upgrade << "',";
	ss << "'" << block->m_block.m_in_battle << "',";
	ss << "'" << block->m_user_id << "'";
	ss << ")";

	if (mysql_real_query(m, ss.str().c_str(), ss.str().size()))
	{
		LLOG_ERROR("ChecssBlockManager::CreateBlock sql error %s", ss.str().c_str());
		return 0;
	}

	block->m_block.m_block_id = (Lint)mysql_insert_id(m);

	AddBlock(block);

	return block;
}

void ChecssBlockManager::AddBlock(BlockPtr block)
{
	m_map_blocks[block->m_block.m_block_id] = block;

	if (block->m_block.m_in_battle)
	{
		m_user_blocks_battle_list[block->m_user_id].push_back(block->m_block.m_block_id);
	}
	else
	{ 
		m_user_blocks_bag_list[block->m_user_id].push_back(block->m_block.m_block_id);
	}
	
}


Lint ChecssBlockManager::GetRandInsertIDFromDB()
{
	return 100;
}


std::list<int>		ChecssBlockManager::GetBattleListByUserId(int user_id)
{

	auto it= m_user_blocks_battle_list.find(user_id);
	if (it != m_user_blocks_battle_list.end())
	{
		return it->second;
	}

	std::list<int>	null_list;
	return null_list;
}

std::list<int>		ChecssBlockManager::GetBagListByUserId(int user_id)
{

	auto it = m_user_blocks_bag_list.find(user_id);
	if (it != m_user_blocks_bag_list.end())
	{
		return it->second;
	}

	std::list<int>	null_list;
	return null_list;
}