#pragma once
#include "SingleBase.h"
#include "ChessBlock.h"
#include "ChessGame.h"


class ChessBlockFactory : public SingleBase<ChessBlockFactory>
{
public:
	void Init();
	std::shared_ptr<ChessBlock> CreateChessBlock(ChessGame* pGame,int color, int block_id,int type, int hp, int max_hp, int attack, int max_attack);
};

