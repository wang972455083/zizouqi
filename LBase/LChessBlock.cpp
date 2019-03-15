#include "LChessBlock.h"



LChessBlock::LChessBlock()
{
	//m_id = 0;
	m_block_id = 0;
	m_type = 0;
	m_hp = 0;
	m_max_hp = 0;
	m_attack = 0;
	m_max_attack = 0;
	m_can_upgrade = false;
	m_in_battle = false;
}


LChessBlock::~LChessBlock()
{
}

void LChessBlock::Read(LBuff& buff)
{
	//buff.Read(m_id);
	buff.Read(m_block_id);
	buff.Read(m_type);

	buff.Read(m_hp);
	buff.Read(m_max_hp);
	buff.Read(m_attack);
	buff.Read(m_max_attack);
	buff.Read(m_can_upgrade);
	buff.Read(m_in_battle);
}
void LChessBlock::Write(LBuff& buff)
{
	//buff.Write(m_id);
	buff.Write(m_block_id);
	buff.Write(m_type);

	buff.Write(m_hp);
	buff.Write(m_max_hp);
	buff.Write(m_attack);
	buff.Write(m_max_attack);
	buff.Write(m_can_upgrade);
	buff.Write(m_in_battle);
}
