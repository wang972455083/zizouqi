
#include "ChessGame.h"
#include "ChessBlockFactory.h"
#include "ChessManager.h"
#include "Table/ChessBoardCfg.h"
#include "Table/ChessCoinCfg.h"
#include "Table/ChessRewardCfg.h"
#include "RoomManager.h"
#include "Work.h"



ChessGame::ChessGame()
{
	m_max_block_id = 0;
	m_left_oper_time = 0;
	m_Color = 0;
	m_game_over = false;
}


ChessGame::~ChessGame()
{
	
}

void ChessGame::Clear()
{
	for (int i = 0; i < POS_LIMIT_X; ++i)
	{
		for (int j = 0; j < POS_LIMIT_Y; j++)
		{
			m_Chess[i][j] = 0;
		}
	}

	m_map_blocks.clear();
	m_map_fighters.clear();
	m_max_block_id = 0;
	m_left_oper_time = 0;
	m_game_over = false;
	m_chess_zhui_sha.Clear();
}

void ChessGame::Start(Room* room)
{
	m_room = room;

	Clear();

	if (m_room)
	{
		Init();
	}
}


void ChessGame::Init()
{
	if (m_room == nullptr)
		return;

	std::vector<LUserPtr> vec_user = m_room->GetAllUsers();
	if (vec_user.size() != 2)
		return;

	
	m_map_fighters.clear();
	for (int i = 0; i < vec_user.size(); ++i)
	{
		LUserPtr room_user = vec_user[i];
		if (room_user == nullptr)
			continue;

		std::shared_ptr<ChessFighter> fighter = std::make_shared<ChessFighter>();
		fighter->m_user_id = room_user->GetUserId();
		CHESS_COLOR color = CHESS_COLOR(CHESS_RED + i);
		fighter->m_color = color;

		ChessCoinTbl* chess_coin_table = ChessCoinCfg::Instance().GetChessCoinTbl();
		if (chess_coin_table == nullptr)
			continue;

		fighter->m_money = chess_coin_table->m_init_coin;


		for (int j = CHESS_JIANG; j < CHESS_MAX; ++j)
		{
			ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(j);
			if (pTbl == nullptr)
				continue;

			/*for (int k = 0; k < pTbl->m_max_cnt; ++k)
			{
				int block_id = m_block_id++;

				std::shared_ptr<BagChessBlock> block = std::make_shared<BagChessBlock>();
				block->m_block_id = block_id;
				block->m_type = pTbl->m_type;
				block->m_hp = pTbl->m_init_hp;
				block->m_max_hp = pTbl->m_init_max_hp;
				block->m_attack = pTbl->m_init_attack;
				block->m_max_attack = pTbl->m_init_max_attack;

				fighter->m_blocks[block->m_block_id] = block;


				LChessBlock lBlock;
				lBlock.m_block_id = block_id;
				lBlock.m_type = pTbl->m_type;
				lBlock.m_hp = pTbl->m_init_hp;
				lBlock.m_max_hp = pTbl->m_init_max_hp;
				lBlock.m_attack = pTbl->m_init_attack;
				lBlock.m_max_attack = pTbl->m_init_max_attack;

				user[i]->m_bag_blocks.push_back(lBlock);

			}*/

			for (int i = 0; i < room_user->m_bag_blocks.size(); ++i)
			{
				LChessBlock& bag_block = room_user->m_bag_blocks[i];

				std::shared_ptr<BagChessBlock> block = std::make_shared<BagChessBlock>();
				block->m_block_id = bag_block.m_block_id;
				block->m_type = bag_block.m_type;
				block->m_hp = bag_block.m_hp;
				block->m_max_hp = bag_block.m_max_hp;
				block->m_attack = bag_block.m_attack;
				block->m_max_attack = bag_block.m_max_attack;

				fighter->m_blocks[block->m_block_id] = block;

				if (m_max_block_id < block->m_block_id)
				{
					m_max_block_id = block->m_block_id;
				}
			}
		}

		m_map_fighters[fighter->m_user_id] = fighter;
	}

	m_map_blocks.clear();

	m_Color = CHESS_RED;

	InitChessPos();

	//NextTurn();
	
}


//开局只把将、士和相给摆出来，其他的每局自己选择添加与否
void ChessGame::InitChessPos()
{
	
	//开局的时候士和将已经摆好了

	for (auto it = m_map_fighters.begin(); it != m_map_fighters.end(); ++it)
	{
		std::shared_ptr<ChessFighter> fighter = it->second;
		for (int j = CHESS_JIANG; j < CHESS_XIANG; j++)
		{
			int type = j;

			std::vector<std::shared_ptr<BagChessBlock> > bag_blocks;
			for (auto iter = fighter->m_blocks.begin(); iter != fighter->m_blocks.end(); ++iter)
			{
				if (iter->second->m_type == type)
				{
					bag_blocks.push_back(iter->second);
				}
			}

			std::vector<Point>	vecPos = ChessManager::getInstance().GetPoints(fighter->m_color, TYPE_CHESS(type));
			if(vecPos.size() != bag_blocks.size())
				continue;

			for (int i = 0; i < bag_blocks.size(); ++i)
			{



				std::shared_ptr<ChessBlock> pBlock = CreateBlocksFromBagList(fighter->m_color,bag_blocks[i]);
				if(nullptr == pBlock)
					continue;

				AddBlockToChess(pBlock, vecPos[i]);
			}
		}
	}
}

std::shared_ptr<ChessBlock> ChessGame::CreateBlocksFromBagList(int color,std::shared_ptr<BagChessBlock> bag_block)
{
	std::shared_ptr<ChessBlock> pBlock = nullptr;

	if (bag_block->m_use_cnt > 0)
	{
		pBlock = ChessBlockFactory::getInstance().CreateChessBlock(this, color, bag_block->m_block_id, bag_block->m_type, bag_block->m_hp, bag_block->m_max_hp, bag_block->m_attack, bag_block->m_max_attack);
	}
	else
	{
		pBlock = ChessBlockFactory::getInstance().CreateChessBlock(this, color, bag_block->m_block_id, bag_block->m_type, 1, bag_block->m_max_hp, 1, bag_block->m_max_attack);
	}

	bag_block->m_use_cnt++;

	
	return pBlock;
}

void ChessGame::AddBlockToChess(std::shared_ptr<ChessBlock> block, Point& to_point)
{
	if (nullptr == block)
		return;

	block->setPos(to_point);
	m_Chess[to_point.m_posX][to_point.m_posY] = block->getId();

	m_map_blocks[block->getId()] = block;
}

bool	ChessGame::IsChessPos(const Point& pt)
{
	if (pt.m_posX<0 || pt.m_posX>POS_LIMIT_X)
		return false;
	if (pt.m_posY<0 || pt.m_posY>POS_LIMIT_Y)
		return false;

	return true;
}



void  ChessGame::Move(std::shared_ptr<ChessBlock>& pBlock, const Point& pos)
{
	if (pBlock)
	{
		if (pBlock->getPos() == pos)
			return;
			
		m_Chess[pBlock->getPos().m_posX][pBlock->getPos().m_posY] = 0;
		m_Chess[pos.m_posX][pos.m_posY] = pBlock->getId();
		pBlock->setPos(pos);
	}
}



bool ChessGame::IsHaveOneBlock(const Point& from_point, const Point& to_point)
{
	//x轴相同，Y轴不同
	if (to_point.m_posX == from_point.m_posX && to_point.m_posY != from_point.m_posY)
	{
		int min = from_point.m_posY;
		int max = to_point.m_posY;
		if (max < min)
		{
			ChessManager::getInstance().Swap(min, max);
		}

		int nCnt = 0;
		int x = to_point.m_posX;
		for (int y = min+1; y < max, y <= POS_LIMIT_Y; ++y)
		{
			if (m_Chess[x][y])
			{
				nCnt++;
			}
		}

		if (nCnt == 1)
			return true;
	}
	else if (to_point.m_posY == from_point.m_posY && to_point.m_posX != from_point.m_posX)
	{
		int min = from_point.m_posX;
		int max = to_point.m_posX;
		if (max < min)
		{
			ChessManager::getInstance().Swap(min, max);
		}

		int nCnt = 0;
		int y = to_point.m_posY;
		for (int x= min+1; x < max, x <= POS_LIMIT_X; ++x)
		{
			if (m_Chess[x][y])
			{
				nCnt++;
			}
		}

		if (nCnt == 1)
			return true;
	}

	return false;
}

bool ChessGame::DestoryBolck(std::shared_ptr<ChessBlock> pBlock)
{
	if (pBlock == nullptr)
		return false;

	auto it = m_map_blocks.find(pBlock->getId());
	if (it != m_map_blocks.end())
		m_map_blocks.erase(it);

	m_Chess[pBlock->getPos().m_posX][pBlock->getPos().m_posY] = 0;


	return true;
}

void ChessGame::GameOver(int winner)
{
	if (m_room == nullptr)
		return;

	if (winner == 0)
		return;

	LMsgS2CChessGameOver send;
	send.m_room_id = m_room->GetRoomId();
	send.m_winner = winner;
	
	m_room->Broadcast(send);

	m_room->GameOver();
	
}

bool ChessGame::IsHaveBlock(const Point& pos)
{
	return m_Chess[pos.m_posX][pos.m_posY];
}

void ChessGame::FillGameInfoMsg(GameInfo& game)
{
	if (nullptr == m_room)
		return;

	game.m_room_id = m_room->GetRoomId();
	game.m_cur_color = m_Color;

	for (auto it = m_map_fighters.begin(); it != m_map_fighters.end(); ++it)
	{
		std::shared_ptr<ChessFighter> fighter = it->second;

		Fighter msg_fighter;
		msg_fighter.m_user_id = fighter->m_user_id;
		msg_fighter.m_color = fighter->m_color;
		msg_fighter.m_money = fighter->m_money;

		for (auto iter = fighter->m_blocks.begin(); iter != fighter->m_blocks.end(); ++iter)
		{
			BlockUse msg_use;
			msg_use.m_block_id = iter->first;
			msg_use.m_use_cnt = iter->second->m_use_cnt;

			msg_fighter.m_use_data.push_back(msg_use);
		}


		game.m_fighters.push_back(msg_fighter);
	}


	for (auto it = m_map_blocks.begin(); it != m_map_blocks.end(); ++it)
	{
		GameBlock msg_block;
		std::shared_ptr<ChessBlock> block = it->second;
		block->FillBlockMsg(msg_block);

		game.m_game_blocks.push_back(msg_block);
	}
}

int ChessGame::GetUserColor(Lint user_id)
{
	auto it = m_map_fighters.find(user_id);
	if (it != m_map_fighters.end())
	{
		return  it->second->m_color;
	}

	return CHESS_ERROR_COLOR;
}

std::shared_ptr<ChessBlock> ChessGame::GetBlockById(Lint block_id)
{
	auto it = m_map_blocks.find(block_id);
	if (it != m_map_blocks.end())
		return it->second;

	return nullptr;
}

void ChessGame::AddCoin(int user_id,int count)
{
	if (count <= 0)
		return;

	auto it = m_map_fighters.find(user_id);
	if (it != m_map_fighters.end())
	{
		it->second->m_money = it->second->m_money + count;

		LMsgS2CChessCoinModify send;
		send.m_target_user_id = user_id;

		send.m_coin = it->second->m_money;

		if (m_room)
		{
			m_room->Broadcast(send);
		}
	}

}

bool ChessGame::CostCoin(int user_id, int cost)
{
	if (m_map_fighters[user_id]->m_money >= cost)
	{
		m_map_fighters[user_id]->m_money = m_map_fighters[user_id]->m_money - cost;

		LMsgS2CChessCoinModify send;
		send.m_target_user_id = user_id;
		send.m_coin = m_map_fighters[user_id]->m_money;

		if (m_room)
		{
			m_room->Broadcast(send);
		}
		return true;
	}

	return false;
}


void ChessGame::AddAttackData(int attack_block_id,int get_damage_id)
{
	ClearAttackData();

	std::shared_ptr<ChessBlock>	attack_block = GetChessBlock(attack_block_id);
	if (attack_block == nullptr)
		return;

	std::shared_ptr<ChessBlock>	get_damage_block = GetChessBlock(get_damage_id);
	if (attack_block == nullptr)
		return;


	for (auto it = m_map_blocks.begin(); it != m_map_blocks.end(); ++it)
	{
		if (it->second->getColor() != get_damage_block->getColor())
			continue;
		if (it->second->getId() == get_damage_block->getId())
			continue;
		

		if (it->second->IsCanProtectBlock(get_damage_block))
		{
			m_chess_zhui_sha.m_attack_blocks.push_back(it->second->getId());
		}
	}

	m_chess_zhui_sha.m_target_block = attack_block_id;
}

bool ChessGame::IsCanZhuiSha(int attack_block_id, int get_damage_id)
{
	if (m_chess_zhui_sha.m_target_block != get_damage_id)
		return false;

	for (int i = 0; i < m_chess_zhui_sha.m_attack_blocks.size(); ++i)
	{
		if (m_chess_zhui_sha.m_attack_blocks[i] == attack_block_id)
		{
			return true;
		}
	}

	return false;
}

void ChessGame::ClearAttackData()
{
	m_chess_zhui_sha.Clear();
}


std::shared_ptr<ChessBlock> ChessGame::GetChessBlock(int block_id)
{
	auto it = m_map_blocks.find(block_id);
	if (it != m_map_blocks.end())
	{
		return it->second;
	}

	return nullptr;
}

//
void ChessGame::OperateBeforeTurn()
{
	ClearAttackData();
}


void ChessGame::NextTurn()
{
	if (m_room == nullptr)
		return;

	//每一轮结束后张金币
	for (auto it = m_map_fighters.begin(); it != m_map_fighters.end(); ++it)
	{
		ChessCoinTbl* chess_coin_table = ChessCoinCfg::Instance().GetChessCoinTbl();
		if (chess_coin_table == nullptr)
			continue;

		AddCoin(it->first, chess_coin_table->m_add_coin_after_turn);
	}
	

	if (m_Color == CHESS_RED)
	{
		m_Color = CHESS_BLACK;
	}
	else
	{
		m_Color = CHESS_RED;
	}
	

	m_left_oper_time = MAX_OPER_TIME;

	

	LMsgS2CSendTurn send;
	send.m_room_id = m_room->GetRoomId();
	send.m_cur_color = m_Color;
	send.m_left_time = m_left_oper_time;

	m_room->Broadcast(send);


	int rand_reward = ChessRewardCfg::Instance().GetRandItem();
	UseRewardItem(rand_reward);
}

void ChessGame::UseRewardItem(int item_id)
{
	if (m_reward_effect == nullptr)
	{
		m_reward_effect = std::make_shared<ItemEffect>();
	}

	m_reward_effect->Clear();

	item_id = 1002;

	RewardItemTbl* pTbl = ChessRewardCfg::Instance().GetChessReward(item_id);
	if (pTbl == nullptr)
		return;

	if (pTbl->m_value == 0)
		return;

	
	std::shared_ptr<ChessFighter> cur_user = nullptr;
	for (auto it = m_map_fighters.begin(); it != m_map_fighters.end(); ++it)
	{
		if (it->second->m_color == m_Color)
		{
			cur_user = it->second;
		}
	}

	if (cur_user == nullptr)
		return;


	LMsgS2CChessRewardUse send;
	send.m_target_user_id = cur_user->m_user_id;
	send.m_reward_item = pTbl->m_id;

	float value = float(pTbl->m_value) / float(100);

	switch (pTbl->m_effect)
	{
	case EF_ADD_COIN:
	{
		AddCoin(cur_user->m_user_id, value);
	}
	break;
	case EF_ADD_STONE:
	{
		cur_user->m_stone_cnt++;
	}
	break;
	case EF_ADD_ATTACK:
	{
		m_reward_effect->m_attack_addition += value;
	}
	break;
	case EF_ADD_ATTACK_GET_COIN:
	{
		m_reward_effect->m_coin_addition_for_attack_once_hp += value;
	}
	break;
	case EF_DESC_COIN:
	{
		CostCoin(cur_user->m_user_id, value);
	}
	break;
	case EF_DESC_HP:  //除了将士外随机选取一名棋子
	{
		std::vector<int> self_blocks;
		for (auto it = m_map_blocks.begin(); it != m_map_blocks.end(); ++it)
		{
			if (it->second->getColor() != m_Color)
				continue;

			if (it->second->getType() == CHESS_JIANG)
				continue;

			if (it->second->getType() == CHESS_SHI)
				continue;

			self_blocks.push_back(it->first);
		}

		if (self_blocks.empty())
			break;

		int index = rand() % self_blocks.size();
		int block_id = self_blocks[index];

		std::shared_ptr<ChessBlock> block = GetBlockById(block_id);
		if (block == nullptr)
			break;

		block->GetDamage(value);

		block->FillBlockMsg(send.m_block);

		if (block->getHp() <= 0)
		{
			DestoryBolck(block);
		}
	}
	break;
	default:
		break;
	}

	if (m_room)
		m_room->Broadcast(send);
}


void ChessGame::HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg)
{
	if (!IsChessPos(Point(msg->m_pos_x, msg->m_pos_y)))
		return;

	int user_id = msg->m_user_id;
	int result = 0;
	std::shared_ptr<ChessBlock> pBlock = nullptr;

	do 
	{


		if (m_Chess[msg->m_pos_x][msg->m_pos_y])
		{
			result = 1;
			break;
		}

	
		auto it = m_map_fighters.find(user_id);
		if (it == m_map_fighters.end())
		{
			result = 2;
			break;
		}

		std::shared_ptr<ChessFighter> fighter = it->second;

		if (m_Color != it->second->m_color)
		{
			result = 3;
			break;
		}

		int block_id = msg->m_block_id;

		//在棋盘上的不能增加了
		auto iter = m_map_blocks.find(block_id);
		if (iter != m_map_blocks.end())
		{
			result = 4;
			break;
		}


		auto itt = fighter->m_blocks.find(block_id);
		if (itt == fighter->m_blocks.end())
		{
			result = 5;
			break;
		}

		std::shared_ptr<BagChessBlock> bag_block = itt->second;



		if (bag_block->m_use_cnt > 0)  //花钱建造，但是出事血量为1
		{
			ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(bag_block->m_type);
			if (pTbl == nullptr)
			{
				result = 7;
				break;
			}

			int cost = 2 * pTbl->m_cost_coin;
			if(!CostCoin(user_id,cost))
			{
				result = 6;
				break;
			}
		}
		

		pBlock = CreateBlocksFromBagList(fighter->m_color, bag_block);
		
		if (nullptr == pBlock)
			continue;

		//ClearAttackData();

		AddBlockToChess(pBlock, Point(msg->m_pos_x,msg->m_pos_y));


	} while (0);


	LMsgS2CAddBlockToChess send;
	send.m_result = result;
	send.m_room_id = m_room->GetRoomId();
	send.m_oper_user_id = user_id;
	if (result == 0)
	{
		if (pBlock)
		{
			pBlock->FillBlockMsg(send.m_block);
		}
	
		if (m_room)
		{
			m_room->Broadcast(send);
		}

		NextTurn();
	}
	else
	{
		m_room->SendToUser(user_id, send);
	}
}

void ChessGame::HanderMoveChessBlock(LMsgC2SChessBlockMove* msg)
{

	Point to_point(msg->m_pos_x, msg->m_pos_y);
	if (!IsChessPos(to_point))
		return;


	int user_id = msg->m_user_id;
	int result = 0;

	do 
	{

		if (m_Chess[msg->m_pos_x][msg->m_pos_y])
		{
			result = 1;
			break;
		}


		auto it = m_map_fighters.find(user_id);
		if (it == m_map_fighters.end())
		{
			result = 2;
			break;
		}

		if (m_Color != it->second->m_color)
		{
			result = 3;
			break;
		}

		int block_id = msg->m_block_id;

		//在棋盘上meiyou
		auto iter = m_map_blocks.find(block_id);
		if (iter == m_map_blocks.end())
		{
			result = 4;
			break;
		}

		std::shared_ptr<ChessBlock> block = iter->second;

		if (m_Color != block->getColor())
		{
			result = 5;
			break;
		}

	
		if (!block->IsCanMove(to_point))
		{
			result = 6;
			break;
		}

		OperateBeforeTurn();

		Move(block, to_point);

	} while (0);

	LMsgS2CChessBlockMove send;
	send.m_result = result;

	if (result == 0)
	{
		send.m_room_id = m_room->GetRoomId();
		send.m_block_id = msg->m_block_id;
		send.m_pos_x = msg->m_pos_x;
		send.m_pos_y = msg->m_pos_y;

		m_room->Broadcast(send);

		NextTurn();

	}
	else
	{
		m_room->SendToUser(user_id, send);
	}
}


void ChessGame::HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg)
{
	int user_id = msg->m_user_id;
	int result = 0;
	
	do
	{
		auto it = m_map_fighters.find(user_id);
		if (it == m_map_fighters.end())
		{
			result = 2;
			break;
		}

		if (m_Color != it->second->m_color)
		{
			result = 3;
			break;
		}

		std::shared_ptr<ChessBlock> attack_block = GetBlockById(msg->m_attack_id);
		if (attack_block == nullptr)
		{
			result = 4;
			break;
		}

		if (m_Color != attack_block->getColor())
		{
			result = 5;
			break;
		}

		std::shared_ptr<ChessBlock> get_damage_block = GetBlockById(msg->m_get_damage_id);
		if (get_damage_block == nullptr)
		{
			result = 4;
			break;
		}

		if (attack_block->getColor()== get_damage_block->getColor())
		{
			result = 5;
			break;
		}



		if (!attack_block->IsCanAttack(get_damage_block))
		{
			result = 6;
			break;
		}

		OperateBeforeTurn();

		ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(get_damage_block->getType());
		if (pTbl == nullptr)
			return;

		
		
		int get_coin = 0;
		int damage = attack_block->getAttack();
		damage += m_reward_effect->m_attack_addition;


		int once_coin = pTbl->m_attacker_get_coin_for_one_damage + pTbl->m_attacker_get_coin_for_one_damage*m_reward_effect->m_coin_addition_for_attack_once_hp;
		if (get_damage_block->getHp() > damage)
		{
			get_coin = damage * pTbl->m_attacker_get_coin_for_one_damage;
		}
		else
		{
			get_coin = pTbl->m_attacker_get_coin_for_one_damage*(get_damage_block->getHp() + get_damage_block->getAttack());
		}


		get_damage_block->GetDamage(damage);

		//如果吃掉，就销毁被吃的棋子，如果没有，则两个棋子都不动
		if (get_damage_block->getHp() == 0)
		{
			if (get_damage_block->getType() == CHESS_JIANG)
			{
				GameOver(user_id);
			}

			DestoryBolck(get_damage_block);

			Move(attack_block, get_damage_block->getPos());
		}
		else
		{
			AddAttackData(msg->m_attack_id, msg->m_get_damage_id);
		}
		
		AddCoin(msg->m_user_id, get_coin);
		
		LMsgS2CChessBlockAttack send;
		send.m_result = 0;
		send.m_room_id = m_room->GetRoomId();
		attack_block->FillBlockMsg(send.m_attack_block);
		get_damage_block->FillBlockMsg(send.m_get_damage_block);
		m_room->Broadcast(send);

		NextTurn();

		return;

	} while (0);

	

	if (result != 0)
	{
		
		LMsgS2CChessBlockMove send;
		send.m_result = result;
		m_room->SendToUser(user_id, send);

	}
}

void ChessGame::HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg)
{

	LMsgS2CChessBlockUpgrade send;
	send.m_oper_user_id = msg->m_user_id;
	send.m_room_id = msg->m_room_id;
	send.m_block_id = msg->m_block_id;
	send.m_upgrade_type = msg->m_upgrade_type;

	int user_id = msg->m_user_id;
	int result = 0;
	do
	{
		auto it = m_map_fighters.find(user_id);
		if (it == m_map_fighters.end())
		{
			result = 2;
			break;
		}

		if (m_Color != it->second->m_color)
		{
			result = 3;
			break;
		}

		int block_id = msg->m_block_id;

		//在棋盘上meiyou
		auto iter = m_map_blocks.find(block_id);
		if (iter == m_map_blocks.end())
		{
			result = 4;
			break;
		}

		std::shared_ptr<ChessBlock> block = iter->second;

		if (m_Color != block->getColor())
		{
			result = 5;
			break;
		}

		ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(block->getType());
		if (pTbl == nullptr)
		{
			result = 6;
			break;
		}

		if(block)

		if (!CostCoin(user_id,pTbl->m_cost_coin))
		{
			result = 7;
			break;
		}

		switch (msg->m_upgrade_type)
		{
		case UPGRADE_HP:
		{
			if (block->getHp() == block->getMaxHp())
			{
				result = 8;
				break;
			}

			block->setHp(block->getHp() + 1);
			send.m_cur_value = block->getHp();
		}
		break;
		case UPGRADE_ATTACK:
		{
			if (block->getAttack() == block->getMaxAttack())
			{
				result = 8;
				break;
			}

			block->setAttack(block->getAttack() + 1);
			send.m_cur_value = block->getAttack();
		}
		break;
		default:
			result = 9;
			break;
		}

	} while (0);

	send.m_result = result;
	if (result == 0)
	{
		m_room->Broadcast(send);
	}
	else
	{
		m_room->SendToUser(user_id, send);
	}

}
//追杀的时候只造成伤害，但是棋子不移动
void ChessGame::HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg)
{
	int user_id = msg->m_user_id;
	int result = 0;

	do
	{
		auto it = m_map_fighters.find(user_id);
		if (it == m_map_fighters.end())
		{
			result = 2;
			break;
		}

		if (m_Color != it->second->m_color)
		{
			result = 3;
			break;
		}

		std::shared_ptr<ChessBlock> attack_block = GetBlockById(msg->m_attack_id);
		if (attack_block == nullptr)
		{
			result = 4;
			break;
		}

		if (m_Color != attack_block->getColor())
		{
			result = 5;
			break;
		}

		std::shared_ptr<ChessBlock> get_damage_block = GetBlockById(msg->m_get_damage_id);
		if (get_damage_block == nullptr)
		{
			result = 4;
			break;
		}

		if (attack_block->getColor() == get_damage_block->getColor())
		{
			result = 5;
			break;
		}



		if (!IsCanZhuiSha(msg->m_attack_id, msg->m_get_damage_id))
		{
			result = 6;
			break;
		}

		OperateBeforeTurn();


		ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(get_damage_block->getId());
		if (pTbl == nullptr)
			return;



		int get_coin = 0;
		int damage = attack_block->getAttack();
		if (get_damage_block->getHp() > damage)
		{
			get_coin = damage * pTbl->m_attacker_get_coin_for_one_damage;
		}
		else
		{
			get_coin = pTbl->m_attacker_get_coin_for_one_damage*(get_damage_block->getHp() + get_damage_block->getAttack());
		}


		
		get_damage_block->GetDamage(damage);

		//如果吃掉，就销毁被吃的棋子，如果没有，则两个棋子都不动
		if (get_damage_block->getHp() == 0)
		{
			if (get_damage_block->getType() == CHESS_JIANG)
			{
				GameOver(user_id);
			}

			DestoryBolck(get_damage_block);

			Move(attack_block, get_damage_block->getPos());
		}
		else
		{
			AddAttackData(msg->m_attack_id, msg->m_get_damage_id);
		}

		AddCoin(user_id,get_coin);


		LMsgS2CChessBlockZhuiSha send;
		send.m_result = 0;
		send.m_room_id = m_room->GetRoomId();
		attack_block->FillBlockMsg(send.m_attack_block);
		get_damage_block->FillBlockMsg(send.m_get_damage_block);
		m_room->Broadcast(send);

		NextTurn();

		return;

	} while (0);



	if (result != 0)
	{

		LMsgS2CChessBlockMove send;
		send.m_result = result;
		m_room->SendToUser(user_id, send);

	}
}


bool ChessGame::IsNorlmalBlock(int type)
{
	if (type > CHESS_NULL && type < CHESS_MAX)
		return true;

	return false;
}






