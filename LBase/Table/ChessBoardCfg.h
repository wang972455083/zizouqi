#pragma once
#include <map>
#include <vector>
#include "../LSingleton.h"

using namespace std;


class ChessBoardTbl
{
public:
	ChessBoardTbl();
	~ChessBoardTbl();

public:
	
	int		m_type;		//leixing
	int		m_cost_coin;	
	int		m_max_cnt;
	bool	m_can_upgrade;
	int		m_init_hp;
	int		m_init_max_hp;
	int		m_init_attack;
	int		m_init_max_attack;
	int		m_attacker_get_coin_for_one_damage;		//棋子没掉一滴血攻击方获得的金币奖励

};

class ChessBoardCfg : public LSingleton<ChessBoardCfg>
{
	
public:
	bool Init();
	bool Final();

	

	bool			LoadTbl();
	void			ClearTbl();

	ChessBoardTbl*	GetChessBoard(int type);

	
protected:
	static const char*		szChessBoardFile;
	map<int, ChessBoardTbl*> m_mapBoards;
};
