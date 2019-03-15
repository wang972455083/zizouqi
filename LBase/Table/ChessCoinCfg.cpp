#include "ChessCoinCfg.h"

#include "DBC.h"
#include "string-util.h"
#include "distribution.h"
#include <time.h>

const char* ChessCoinCfg::szChessCoinFile = "table/chesscoin.dat";


bool ChessCoinCfg::Init()
{
	return true;
}

bool ChessCoinCfg::Final()
{
	return true;
}


void ChessCoinCfg::Clear()
{
	if (m_table)
	{
		delete m_table;
		m_table = nullptr;
	}
}

bool ChessCoinCfg::LoadTbl()
{
	Clear();

	DBCFile fileDBC(0);
	fileDBC.OpenFromTXT(szChessCoinFile);
	int nRow = fileDBC.GetRecordsNum();
	for (int i = 0; i < nRow; i++)
	{
		m_table = new ChessCoinTbl();
		m_table->m_init_coin = fileDBC.Search_Posistion(i, 0)->iValue;
		m_table->m_add_coin_after_turn = fileDBC.Search_Posistion(i, 1)->iValue;
	}

	printf("Load %s cnt:%d\n", szChessCoinFile, nRow);

	return true;
}

ChessCoinTbl*	ChessCoinCfg::GetChessCoinTbl()
{
	return m_table;
}