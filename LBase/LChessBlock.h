#pragma once

#include "LBuff.h"


class LChessBlock
{
public:
	LChessBlock();
	~LChessBlock();
	void	Read(LBuff& buff);
	void	Write(LBuff& buff);


public:
	//Lint	m_id;
	int		m_block_id;
	int		m_type;
	int		m_hp;
	int     m_max_hp;
	int     m_attack;
	int		m_max_attack;
	bool	m_can_upgrade;
	bool	m_in_battle;
};





