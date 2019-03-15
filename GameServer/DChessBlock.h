#pragma once
#include "LChessBlock.h"


class DChessBlock
{
public:
	DChessBlock();
	~DChessBlock();

public:
	LChessBlock		m_block;
	int				m_user_id;
};

typedef std::shared_ptr<DChessBlock> BlockPtr;
