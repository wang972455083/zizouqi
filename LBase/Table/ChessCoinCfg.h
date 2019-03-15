#pragma once
#include <map>
#include <vector>
#include "../LSingleton.h"

using namespace std;


class ChessCoinTbl
{
public:
	ChessCoinTbl()
	{
		m_init_coin = 0;
		m_add_coin_after_turn = 0;
	}

public:
	int		m_init_coin;
	int		m_add_coin_after_turn;
};

class ChessCoinCfg:  public LSingleton<ChessCoinCfg>
{
public:
	bool Init();
	bool Final();



	bool			LoadTbl();
	void			Clear();
	ChessCoinTbl*			GetChessCoinTbl();

private:
	ChessCoinTbl*			m_table;
	static const char*		szChessCoinFile;
};

