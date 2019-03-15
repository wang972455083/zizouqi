#ifndef _L_CARDS_OPER_TYPE_H_
#define _L_CARDS_OPER_TYPE_H_

enum CARDS_OPER_TYPE
{
	CARDS_OPER_TYPE_INIT								= 0,        // ��������
	CARDS_OPER_TYPE_RECHARGE							= 1,        // ��ֵ
	CARDS_OPER_TYPE_FREE_GIVEN							= 2,        // �������        
	CARDS_OPER_TYPE_ROOM_CHARGE_BY_OWNER				= 3,        // ����֧������
	CARDS_OPER_TYPE_ROOM_CHARGE_BY_AA					= 4,        // AA��֧������
	CARDS_OPER_TYPE_ROOM_CHARGE_BY_WINNER				= 5,        // ��Ӯ��֧������
	CARDS_OPER_TYPE_ACTIVITY							= 6,        // ��ͷ���
	CARDS_OPER_TYPE_PURCHASE_GIVEN						= 7,        // �����������
	CARDS_OPER_TYPE_VIP_GIVE							= 8,        // �������VIP��������
	CARDS_OPER_TYPE_INVITING							= 9,        // ��������ͷ���
	CARDS_OPER_TYPE_ACCEPT_INVITING						= 10,       // ���������ͷ���
	CARDS_OPER_TYPE_PLAYING								= 11,       // ����ƾ��ͷ���������ƾֿ���������÷���
	CARDS_OPER_TYPE_SHARE								= 12,       // �����ͷ���
	CARDS_OPER_TYPE_SIGNIN								= 13,       // ǩ���ͷ���
	CARDS_OPER_TYPE_REPLENISH_SIGNIN					= 14,       // ��ǩ���۷���
	CARDS_OPER_TYPE_TASK								= 15,       // ��������ͷ���
	CARDS_OPER_TYPE_RANKING								= 16,       // ���а��ͷ���
	CARDS_OPER_TYPE_RECRUIT_NEW_PLAYER					= 17,       // �����ͷ���
	CARDS_OPER_TYPE_OLD_PLAYER_BACK						= 18,       // ����һ����ͷ���
	CARDS_OPER_TYPE_VOTE								= 19,       // ����ͶƱ�ͷ���
	CARDS_OPER_TYPE_BIND_MOBILE							= 20,       // ���ֻ����ͷ���
	CARDS_OPER_TYPE_BIND_AGENT							= 21,       // ���ƹ�Ա�ͷ���
	CARDS_OPER_TYPE_SPECIAL_CARD_STYLE					= 22,       // ���������ͷ���
	CARDS_OPER_TYPE_EXCHANGE_LUCKY_DRAW_QUALIFICATION   = 23,       // �齱����ת������
	CARDS_OPER_TYPE_LUCKY_DRAW                          = 24,       // �齱����ת�̻�÷���
	CARDS_OPER_TYPE_EXCHANGE_GOLDEN_COIN                = 25,       // �һ����
	CARDS_OPER_TYPE_EXCHANGE_BY_GOLDEN_COIN             = 26,       // ��Ҷһ�����
	CARDS_OPER_TYPE_EXCHANGE_BY_GIFT_CERTIFICATE        = 27,       // ��ȯ������
	CARDS_OPER_TYPE_MATCH_ENTRY                         = 28,       // �����������ۿ�
	CARDS_OPER_TYPE_WIN_MATCH                           = 29,       // ������Ӯ����
	CARDS_OPER_TYPE_ENTER_MATCH_GIVEN                   = 30,       // ����������ͷ���
	CARDS_OPER_TYPE_QUIT_MATCH_RETURN                   = 31,       // �����������������˻�
	CARDS_OPER_TYPE_RELIEF_IN_MATCH                     = 32,       // ��������������ȼ�����
	CARDS_OPER_TYPE_PRICE_ADJUSTMENT_COMPENSATION       = 100,      // �۸��������
	CARDS_OPER_TYPE_ACCIDENT_COMPENSATION               = 101,      // �¹ʲ���
	CARDS_OPER_TYPE_REWARD_BY_GM                        = 102,      // ƽ̨����
	CARDS_OPER_TYPE_DEDUCTION_BY_GM                     = 103,      // ƽ̨�ۿ�
};

enum TEAM_CARDS_OPER_TYPE
{
	TCARDS_OPER_TYPE_RECHARGE_BY_CREATOR = 0,      // ���ᴴ���߳俨
	TCARDS_OPER_TYPE_ROOM_CHARGE_BY_OWNER = 1,      // �����Ա��������ۿ�
};

#endif  // _L_CARDS_OPER_TYPE_H_