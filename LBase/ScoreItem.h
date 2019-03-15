#ifndef _SCORE_H_
#define _SCORE_H_

#include <vector>

//�����
enum SCORE_ITEM
{
	SI_INVALID			= 0,	//��Ч��
	SI_XIAO_HU			= 1,	//ƽ��
	SI_QING_YI_SE		= 2,	//��һɫ
	SI_PENG_PENG_HU		= 3,	//������
	SI_QI_DUI			= 4,	//�߶�
	SI_HAO_HUA_QI_DUI	= 5,	//�����߶�
	SI_QING_PENG_PENG_HU= 6,	//��������
	SI_QING_QI_DUI		= 7,	//���߶�
	SI_QING_HAO_HUA_QI_DUI = 8,	//������߶�
	SI_JIE_PAO			= 9,	//����
	SI_DIAN_PAO			= 10,	//����
	SI_ZI_MO			= 11,	//����
	SI_TIAN_HU			= 12,	//���
	SI_DI_HU			= 13,	//�غ�
	SI_HUA_HU			= 14,	//����
	SI_CHAO_HAO_HUA_QI_DUI = 15,//�������߶�
	SI_CHAO_CHAO_HAO_HUA_QI_DUI = 16,//���������߶�


	/********����ռ��******/
	SI_XIAO_XUE    = 800,    //СѪ
	SI_DA_XUE      = 801,    //��Ѫ
	SI_RUAN_PI_HU  = 802,    //��ƨ��
	SI_YING_PI_HU  = 803,    //Ӳƨ�� 
	SI_QUANQIU_REN = 804,    //ȫ����
	/*********************/

	SI_MING_GANG		= 940,	//����(all)
	SI_AN_GANG			= 950,	//����(all)
	SI_DI_HUA           = 960,	//�׻�����
	SI_DI_BU_HUA        = 970,	//��������
	SI_SHOU_HUA         = 980,	//�ֻ�����
	SI_GA               = 990,	//�ܷ�(all)
	
	/********����ռ��******/
	SI_PIZIGANG      = 1000, //Ʀ�Ӹ�
	SI_LAIZIGANG     = 1001, //��Ӹ�

	SI_FENGDING      = 1010, //�ⶥ
	SI_JINDING       = 1011, //��

	//����
	SI_QIANG_GANG_HU_LOSE						= 1012,	//����ʧ��
	SI_QIANG_GANG_HU_WIN						= 1013,	//���ܳɹ�


	//SI_YING_QIANG_GANG_HU					= 1016,		//Ӳ���ܺ�
	//SI_RUAN_QIANG_GANG_HU					= 1017,		//�����ܺ�
	SI_ZUAN_SHI_DING						= 1018,		//��ʯ��
	SI_YANG_GUANG_DING						= 1019,		//���ⶥ
	
	SI_HONG_ZHONG_GANG						= 1020,		//���п���
	SI_KAI_KOU								= 1021,		//����
	SI_YING_PAI								= 1022,		//Ӳ��

	SI_YIN_DING								= 1023,		//����

	/**********************/

	SI_QIANG_GANG		= 17,	//����
	
	SI_GANG_SHANG_HUA	= 18,	//���ϻ�
	SI_HUA_SHANG_HUA	= 19,	//���ϻ�
	SI_LIAN_ZHUANG		= 20,	//��ׯ
	SI_SHI_SAN_YAO		= 21,	//13��

	SI_ZHUANG_XIAN		= 23,	//ׯ��
	SI_SHOU_ZHANG_BEI_GEN = 24,	//���ű���
	SI_SHOU_ZHANG_BEI_GANG= 25,	//���ű���	
	SI_3_DAO_BAO_PAI	= 26,	//3������
	SI_4_DAO_BAO_PAI	= 27,	//4������
	SI_QIANG_GANG_BAO_PAI = 28,	//���ܰ���
	SI_DIAN_GANG		= 29,	//��� 3#
	SI_SHUN_HUA_HU		= 30,	//˳����

	//§��
	SI_ZI_YI_SE			= 31,	//��һɫ
	SI_JIANG_YI_SE		= 32,	//��һɫ
	SI_LOU_BAO			= 33,	//§��
	SI_BAO_ZHONG_BAO	= 34,	//���б�
	SI_BAO_MING_LOU 	= 35,	//��¥
	SI_BAO_HAIDILAO 	= 36,	//������
	SI_BAO_SHUAIPAI 	= 37,	//˦��
	SI_BAO_DADIAOCHE 	= 38,	//�����
	SI_BAO_JIAO_ZI      = 39,   //������
	SI_BU_GANG  		= 40,	//����§��
	//�ൺ
	SI_DAN_DIAO_JIANG = 41,          //������
	SI_HUI_JIANG = 42,          //�Ὣ
	SI_SHEN_JIANG = 43,          //��
	SI_HU_HUA = 44,          //����
	SI_JIU_LIAN_BAO_DENG = 45,	//��������
	SI_DA_SAN_YUAN = 46,	//����Ԫ
	SI_DA_SI_XI = 47,	//����ϲ
	SI_BAO_SAN_JIA = 48,	//������
	//�����齫
	SI_BAO_ZUI	= 49,		//����
	SI_MAI_ZHA_DAN = 50,	//��ը��
	SI_MAI_PAO_ZI  = 51,	//������
	SI_DAI_CAI	   = 52,	//����
	SI_CHI_SAN_KOU	   = 53,	//������
	SI_BUY_QUE_MEN = 54,	//��ȱ��

	//��̨�齫
	SI_MAI_AN_GANG_WIN = 55, //�򰵸�Ӯ
	SI_MAI_AN_GANG_LOSE = 56, //�򰵸���
	SI_GANG_PAO = 57,         //����
	//����
	SI_258JIANGYISE = 151,       //��һɫ
	SI_HUN_YI_SE = 152,		     //��һɫ
	SI_HAI_DI_LAO = 153,         //������
	SI_DA_DIAO_CHE = 154,        //�����
	SI_YI_TIAO_LONG = 155,       //һ����
	SI_SHI_SAN_BU_KAO = 156,     //ʮ������
	SI_MING_LOU = 157,		     //��¥
	SI_MENQING = 158,		     //����

	SI_TOTAL_HUA = 162,          //�ܻ�������
	SI_TOTAL_FAN = 163,          //�ܷ�������
	SI_HUANG_ZHUANG_COUNT = 200, //��ׯ��������

	SI_PU=201,					//���� ��
	SI_MING_DA = 202,			//����
	SI_DAN_DIAO = 203,				//����
	SI_BIAN = 204,					//�ߺ�
	SI_MAO = 205,					//ë�ĸ���

	SI_DAI_QUE_MEN = 206,			//��ȱ��
	SI_DIAN_PENG = 209,				//����
	SI_SHEN_CARD = 208,				//����
	SI_YU_SCORE = 207,				//���
	SI_SHUAI_HUN = 210,				//˦����
	SI_MAN_HUI = 211,				//������

						
	SI_END = 10000,
};

struct MatchScoreNotice
{
	/////////////////matchʹ��/////////////////
	Lint		m_id;  //���id
	Lstring		m_nike;//����ǳ�
	Lint        m_score;//��һ���
	Lint        m_mc;   //����
	Lstring		m_headImageUrl;//���ͷ��
	/////////////////////////////////
	MSGPACK_DEFINE(m_id, m_nike, m_score, m_mc, m_headImageUrl);
public:
	MatchScoreNotice()
		: m_id(0)
		, m_score(0)
		, m_mc(0)
	{}
};

struct ScoreNotice
{
	int m_score_item;	//�����
	int m_count;		//����
	MSGPACK_DEFINE(m_score_item,m_count);
public:
	ScoreNotice()
		:m_score_item(SI_INVALID)
		,m_count(0)
	{}
};

typedef std::vector<ScoreNotice> ScoreNoticeVec; 

#endif