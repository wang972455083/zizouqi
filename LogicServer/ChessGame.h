#pragma once


#include "LBase.h"
#include "LMsg.h"
#include "Point.h"
#include "ItemEffect.h"
class Room;
class ChessBlock;


//追杀功能
struct ChessZhuiSha
{
	ChessZhuiSha()
	{
		Clear();

	}

	Lint	m_target_block;		//攻击的棋子id
	
	std::vector<int>	m_attack_blocks;

	void Clear()
	{
		m_target_block = 0;
	
		m_attack_blocks.clear();
	}
};

struct BagChessBlock
{
	int			m_block_id;
	//LChessBlock	m_block;
	int			m_type;
	int			m_hp;
	int			m_max_hp;
	int			m_attack;
	int			m_max_attack;
	int			m_use_cnt;

	BagChessBlock()
	{
		m_block_id = 0;
		m_type = 0;
		m_hp = 0;
		m_max_hp = 0;
		m_attack = 0;
		m_max_attack = 0;
		m_use_cnt = 0;
	}
};

class ChessFighter
{
public:
	ChessFighter()
	{
		m_user_id = 0;
		m_money = 0;
		m_shou = 0;
		m_color = 0;
		m_stone_cnt = 0;
	}

public:
	Lint		m_user_id;
	Lint		m_money;
	Lint		m_shou;
	int			m_color;

	std::map<int, std::shared_ptr<BagChessBlock> > m_blocks;
	int			m_stone_cnt;

};

class ChessGame
{
public:
	ChessGame();
	~ChessGame();

	void Clear();

public:
	void Start(Room* room);
	void Init();
	
	void InitChessPos();
	std::shared_ptr<ChessBlock> CreateBlocksFromBagList(int color,std::shared_ptr<BagChessBlock> bag_block);  //出站自己还没有出战过的棋子
	
	//void CheckMove(std::shared_ptr<ChessBlock>& pBlock, const Point& to_point);
	void Attack(const Point& from_point, const Point& to_point);
	bool IsHaveBlock(const Point& pos);
	void FillGameInfoMsg(GameInfo& game);
	void AddBlockToChess(std::shared_ptr<ChessBlock> block, Point& to_point);
	void UseRewardItem(int item_id);
public:
	std::shared_ptr<ChessBlock> GetChessBlock(int block_id);
	

private:
	bool IsChessPos(const Point& pt);
	
	bool DestoryBolck(std::shared_ptr<ChessBlock> pBlock);
	
	void Move(std::shared_ptr<ChessBlock>& pBlock, const Point& pos);
	//这个就是限定在垂直和水平上
	bool IsHaveOneBlock(const Point& from_point, const Point& to_point);
	void GameOver(int winner);

	int GetUserColor(Lint user_id);
	std::shared_ptr<ChessBlock> GetBlockById(Lint block_id);
	void AddCoin(int user_id,int count);
	bool CostCoin(int user_id, int ccount);
	void AddAttackData(int attack_block_id,int get_damage_id);
	bool IsCanZhuiSha(int attack_id, int get_damage_id);
	void ClearAttackData();

private:
	void NextTurn();
	void OperateBeforeTurn();  //在进行跳转到下一玩家的操作前的清理工作

public:
	void HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg);
	void HanderMoveChessBlock(LMsgC2SChessBlockMove* msg);
	void HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg);
	void HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg);
	void HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg);

	bool IsNorlmalBlock(int type);
private:
	int m_Chess[POS_LIMIT_X][POS_LIMIT_Y];
	int														m_Color;   
	std::map<int, std::shared_ptr<ChessBlock>>				m_map_blocks;
	std::map<int, std::shared_ptr<ChessFighter> >			m_map_fighters;
	Room*													m_room;
	ChessZhuiSha											m_chess_zhui_sha;

	int														m_max_block_id;
	int														m_left_oper_time;		//本回合剩余操作时间
	bool													m_game_over;
	std::shared_ptr<ItemEffect>								m_reward_effect;
};

