#pragma once

enum
{
	EF_NULL,
	EF_ADD_COIN,
	EF_ADD_STONE,
	EF_ADD_ATTACK,
	EF_ADD_ATTACK_GET_COIN,
	EF_DESC_COIN,
	EF_DESC_HP,
};


struct ItemEffect
{
public:
	ItemEffect()
	{
		Clear();
	}
	
	void Clear()
	{
		
		m_attack_addition = 0;
		m_coin_addition_for_attack_once_hp = 0.0f;
	}

	int		m_attack_addition;
	float	m_coin_addition_for_attack_once_hp;	//对对方造成没一点伤害的获得金币加成
};

