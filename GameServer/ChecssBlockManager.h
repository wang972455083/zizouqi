#pragma once

#include "LSingleton.h"
#include "DChessBlock.h"
#include "LBase.h"

class ChecssBlockManager :public LSingleton<ChecssBlockManager>
{
public:

	virtual	bool		Init();
	virtual	bool		Final();

	BlockPtr				GetChessBlockById(Lint block_id);

	void				DelChessBlockById(Lint block_id);


	//BlockPtr				CreateBlock(int user_id, Lstring name);

	void				Save(BlockPtr block);

	void				LoadBloackInfo();

	BlockPtr			CreateBlock(int user_id,int type, int hp, int max_hp, int attack, int max_attack, bool can_upgrade = true,bool in_battle = false);
	void				AddBlock(BlockPtr block);
	Lint				GetRandInsertIDFromDB();
	Lstring				GetTableName();
	
	std::list<int>			GetBattleListByUserId(int user_id);
	std::list<int>			GetBagListByUserId(int user_id);

private:
	std::map<Lint, BlockPtr >	m_map_blocks;

	std::map<int, std::list<int> >	m_user_blocks_bag_list;
	std::map<int, std::list<int> >	m_user_blocks_battle_list;
};

#define gChessBlockManager ChecssBlockManager::Instance()

