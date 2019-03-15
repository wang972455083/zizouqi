#ifndef _AI_H_
#define _AI_H_

#include "Card.h"


class AI
{
public:
	AI();
	~AI();

	//������
	void				CheckCard(CardVector& card);

	//����
	Card*				CheckOutCard(CardVector& handcard, CardVector& eatcard, CardVector& bombCard, CardVector& abombCard,Lint& flag);

	//˼��
	Lint				ThinkCard(CardVector& handcard, Card* outCard);

private:
	CardVector		m_card;
	CardVector		m_fourCard;
	CardVector		m_threeCard;
	CardVector		m_twoCard;
	CardVector		m_oneCard;
	CardVector		m_checkCard[14];
	CardVector		m_okCard;
};

#endif