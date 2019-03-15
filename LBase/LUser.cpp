#include "LUser.h"

LUser::LUser()
{
	m_id = 0;
	m_user_id = 0;
	m_sex = 1;
	m_money = 0;
	m_scores = 0;
	m_bag_block_cnt = 0;
	m_gate_id = 0;
}

LUser::~LUser()
{

}

LUser& LUser::operator = (const LUser& user)
{
	m_id = user.m_id;
	m_user_id = user.m_user_id;
	m_name = user.m_name;
	m_sex = user.m_sex;
	m_headImageUrl = user.m_headImageUrl;
	m_money = user.m_money;
	m_scores = user.m_scores;
	m_bag_block_cnt = user.m_bag_block_cnt;

	m_bag_blocks.clear();
	for (int i = 0; i < user.m_bag_blocks.size(); ++i)
	{
		m_bag_blocks.push_back(user.m_bag_blocks[i]);
	}

	m_gate_id = user.m_gate_id;

	return *this;
}

void LUser::Read(LBuff& buff)
{
	buff.Read(m_id);
	buff.Read(m_user_id);
	buff.Read(m_name);

	buff.Read(m_sex);
	buff.Read(m_headImageUrl);
	buff.Read(m_money);
	buff.Read(m_scores);

	buff.Read(m_bag_block_cnt);
	for (int i = 0; i < m_bag_block_cnt; ++i)
	{
		LChessBlock block;
		block.Read(buff);

		m_bag_blocks.push_back(block);
	}

	buff.Read(m_gate_id);
}

void LUser::Write(LBuff& buff)
{
	buff.Write(m_id);
	buff.Write(m_user_id);
	buff.Write(m_name);

	buff.Write(m_sex);
	
	buff.Write(m_headImageUrl);
	buff.Write(m_money);
	buff.Write(m_scores);

	m_bag_block_cnt = m_bag_blocks.size();
	buff.Write(m_bag_block_cnt);

	for (int i = 0; i < m_bag_blocks.size(); ++i)
	{
		m_bag_blocks[i].Write(buff);
	}

	buff.Write(m_gate_id);
}