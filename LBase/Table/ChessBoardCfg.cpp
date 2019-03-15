#include "ChessBoardCfg.h"

#include "DBC.h"
#include "string-util.h"
#include "distribution.h"
#include <time.h>

const char* ChessBoardCfg::szChessBoardFile = "table/chessblock.dat";




ChessBoardTbl::ChessBoardTbl()
{
	
	m_type = 0;
	m_cost_coin = 0;
	m_max_cnt = 0;

	m_can_upgrade = false;
	m_init_hp = 0;
	m_init_max_hp = 0;
	m_init_attack = 0;
	m_init_max_attack = 0;
	m_attacker_get_coin_for_one_damage = 0;

}


ChessBoardTbl::~ChessBoardTbl()
{
}

bool ChessBoardCfg::Init()
{
	return true;
}

bool ChessBoardCfg::Final()
{
	ClearTbl();
	return true;
}

bool ChessBoardCfg::LoadTbl()
{
	ClearTbl();

	DBCFile fileDBC(0);
	fileDBC.OpenFromTXT(szChessBoardFile);
	int nRow = fileDBC.GetRecordsNum();
	for (int i = 0; i < nRow; i++)
	{

		//pTbl->m_tBeginTime = GetDayTimeByStr(fileDBC.Search_Posistion(i, E_ACTIVE_SHOP_TIME_BG_D)->pString
		//	, fileDBC.Search_Posistion(i, E_ACTIVE_SHOP_TIME_BG_T)->pString);
		//bool bUsing = (fileDBC.Search_Posistion(i, E_ACTIVE_SHOP_USEING)->iValue == 0) ? false : true;
		ChessBoardTbl* pTbl = new ChessBoardTbl();
		pTbl->m_type = fileDBC.Search_Posistion(i, 0)->iValue;

		pTbl->m_cost_coin = fileDBC.Search_Posistion(i, 1)->iValue;
		pTbl->m_max_cnt = fileDBC.Search_Posistion(i, 2)->iValue;
	    pTbl->m_can_upgrade = fileDBC.Search_Posistion(i, 3)->iValue;
		pTbl->m_init_hp = fileDBC.Search_Posistion(i, 4)->iValue;
		pTbl->m_init_max_hp = fileDBC.Search_Posistion(i, 5)->iValue;
		pTbl->m_init_attack = fileDBC.Search_Posistion(i, 6)->iValue;
		pTbl->m_init_max_attack = fileDBC.Search_Posistion(i, 7)->iValue;
		pTbl->m_attacker_get_coin_for_one_damage = fileDBC.Search_Posistion(i, 8)->iValue;

	
		m_mapBoards[pTbl->m_type] = pTbl;
	}
	printf("Load %s cnt:%d\n", szChessBoardFile, nRow);
	return true;

}

void ChessBoardCfg::ClearTbl()
{
	
	for (auto iter = m_mapBoards.begin(); iter != m_mapBoards.end(); iter++)
	{
		delete iter->second;
	}
	m_mapBoards.clear();
}


ChessBoardTbl* ChessBoardCfg::GetChessBoard(int type)
{
	auto it = m_mapBoards.find(type);
	if (it != m_mapBoards.end())
	{
		return it->second;
	}

	return nullptr;
}


