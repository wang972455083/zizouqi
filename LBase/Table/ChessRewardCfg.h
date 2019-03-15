#pragma once
#include <map>
#include <vector>
#include "../LSingleton.h"

class RewardItemTbl
{
public:
	RewardItemTbl()
	{
		m_id = 0;
		m_value = 0;
		m_effect = 0;
		m_weight = 0;
	}

public:
	int	 m_id;
	int  m_value;  //�ٷֱ�
	int  m_effect;	//����Ч��
	int  m_weight;  //Ȩֵ
};

class ChessRewardCfg : public LSingleton<ChessRewardCfg>
{
public:
	bool Init();
	bool Final();



	bool			LoadTbl();
	void			ClearTbl();

	RewardItemTbl*	GetChessReward(int id);

	int				GetRandItem();

protected:
	static const char*		szChessRewardFile;
	std::map<int, RewardItemTbl*> m_mapRewardItem;
	int					m_total_weight;
};

