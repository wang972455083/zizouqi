
#include "ChessRewardCfg.h"

#include "DBC.h"
#include "string-util.h"
#include "distribution.h"
#include <time.h>
#include "../LTool.h"

const char* ChessRewardCfg::szChessRewardFile = "table/chessReward.dat";


bool ChessRewardCfg::Init()
{
	return true;
}

bool ChessRewardCfg::Final()
{
	ClearTbl();
	return true;
}

bool ChessRewardCfg::LoadTbl()
{
	ClearTbl();

	DBCFile fileDBC(0);
	fileDBC.OpenFromTXT(szChessRewardFile);
	int nRow = fileDBC.GetRecordsNum();
	for (int i = 0; i < nRow; i++)
	{

		RewardItemTbl* pTbl = new RewardItemTbl();
		pTbl->m_id = fileDBC.Search_Posistion(i, 0)->iValue;

		pTbl->m_value = fileDBC.Search_Posistion(i, 1)->iValue;
		pTbl->m_effect = fileDBC.Search_Posistion(i, 2)->iValue;
		pTbl->m_weight = fileDBC.Search_Posistion(i, 3)->iValue;
		
		m_total_weight += pTbl->m_weight;
		m_mapRewardItem[pTbl->m_id] = pTbl;
	}
	printf("Load %s cnt:%d\n", szChessRewardFile, nRow);
	return true;

}

void ChessRewardCfg::ClearTbl()
{

	for (auto iter = m_mapRewardItem.begin(); iter != m_mapRewardItem.end(); iter++)
	{
		delete iter->second;
	}
	m_mapRewardItem.clear();
}


RewardItemTbl* ChessRewardCfg::GetChessReward(int id)
{
	auto it = m_mapRewardItem.find(id);
	if (it != m_mapRewardItem.end())
	{
		return it->second;
	}

	return nullptr;
}

int ChessRewardCfg::GetRandItem()
{
	
	int key = L_Rand(0,m_total_weight);

	int weight = 0;

	for (auto it = m_mapRewardItem.begin(); it != m_mapRewardItem.end(); ++it)
	{
		weight += it->second->m_weight;
		if (weight >= key)
			return it->first;
	}

	return 0;
}
