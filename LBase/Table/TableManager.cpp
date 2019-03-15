#include "TableManager.h"
#include "ChessBoardCfg.h"
#include "ChessCoinCfg.h"
#include "ChessRewardCfg.h"

bool TableManager::Init()
{
	Load();
	return true;
}


bool TableManager::Final()
{
	return true;
}

void TableManager::Load()
{
	ChessBoardCfg::Instance().LoadTbl();
	ChessCoinCfg::Instance().LoadTbl();
	ChessRewardCfg::Instance().LoadTbl();
}
