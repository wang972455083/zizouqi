#ifndef _L_CHARGE_H_
#define _L_CHARGE_H_

#include "LBuff.h"



//enum CARDS_OPER_TYPE
//{
//	//CARDS_OPER_TYPE_INIT = 0,		        //��������-
//	//CARDS_OPER_TYPE_CHARGE = 1,		    //��ֵ�-
//	//CARDS_OPER_TYPE_FREE_GIVE = 2,	    //�������-
//	//CARDS_OPER_TYPE_CREATE_ROOM = 3,      //��������۳�-
//	//CARDS_OPER_TYPE_ACTIVE = 4,		    //��ͷ���-
//	//CARDS_OPER_TYPE_DAILY_LOGIN = 5,      //ÿ�յ�¼�ͷ���-
//	//CARDS_OPER_TYPE_LUCK_DRAW = 6,	    //�齱�ۿ�-
//	//CARDS_OPER_TYPE_PHONE_NUM = 7,	    //���ֻ����ͷ���-
//	//CARDS_OPER_TYPE_FEN_XIANG = 8,		//�����ͷ���-
//	//CARDS_OPER_TYPE_MATCH = 9,            //�μӱ����۳�-
//	//CARDS_OPER_TYPE_MATCH_REWARD=10,      //�μӱ�������-
//	//CARDS_OPER_TYPE_CHANGEPRICE  = 100,   //�����۸��������-
//};

class LCharge
{
public:
	LCharge();
	~LCharge();

	void	Read(LBuff& buff);
	void	Write(LBuff& buff);
public:
	Lint	m_userId;   //���ID
	Lint	m_time;     //�����䶯ʱ��
	Lint	m_cardNum;  //�����䶯����
	Lint	m_operType; //�����䶯����
	Lint    m_balance;  //��ҷ������
	Lstring	m_operator; //������(Ĭ��Ϊ��)
	Lstring m_remark;   //��ע(Ĭ��Ϊ��)
	Lint	m_nAppId;
};
#endif