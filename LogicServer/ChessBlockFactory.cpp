
#include "ChessBlockFactory.h"
#include "Table/ChessBoardCfg.h"

void ChessBlockFactory::Init()
{
	
}

std::shared_ptr<ChessBlock> ChessBlockFactory::CreateChessBlock(ChessGame* pGame,int color,int block_id,int type,int hp,int max_hp,int attack,int max_attack)
{
	std::shared_ptr<ChessBlock> pBlock = nullptr;

	ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(type);
	if (pTbl == nullptr)
		return pBlock;
	switch (pTbl->m_type)
	{
	case CHESS_JIANG:
		pBlock = std::make_shared<ChessJiang>();
		break;
	case CHESS_SHI:
		pBlock = std::make_shared<ChessShi>();
		break;
	case CHESS_XIANG:
		pBlock = std::make_shared<ChessXiang>();
		break;
	case CHESS_MA:
		pBlock = std::make_shared<ChessMa>();
		break;
	case CHESS_CHE:
		pBlock = std::make_shared<ChessChe>();
		break;
	case CHESS_PAO:
		pBlock = std::make_shared<ChessPao>();
		break;
	case CHESS_BING:
		pBlock = std::make_shared<ChessBing>();
		break;
	}

	if (pBlock == nullptr)
		return nullptr;
	pBlock->setId(block_id);
	pBlock->setColor(color);
	
	pBlock->setHp(hp);
	pBlock->setAttack(attack);
	pBlock->setMaxHp(max_hp);
	pBlock->setMaxAttack(max_attack);

	pBlock->setGame(pGame);

	return pBlock;
}

