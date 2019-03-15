#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#define CARD_COUNT 144
#define REN_MAN_MATCH_TYPE 9999; //�������ı��

#define FEN_BAO_COST_CARD_16_CIRCLE_TYPE  5  //�ְ�16Ȧ�۳���������

enum CARD_COLOR
{
	CARD_COLOR_WAN = 1,			//��
	CARD_COLOR_TUO = 2,			//Ͳ
	CARD_COLOR_SUO = 3,			//��
	CARD_COLOR_FENG = 4,		//����������
	CARD_COLOR_JIAN = 5,		//�з���
	CARD_COLOR_FLOWER = 6,		//�����ﶬ ÷�����
};

//���˼������
enum THINK_OPERATOR
{
	THINK_OPERATOR_NULL = 0, 
	THINK_OPERATOR_OUT = 1,             //����
	THINK_OPERATOR_BOMB      = 2,		//����
	THINK_OPERATOR_AGANG     = 3,		//����
	THINK_OPERATOR_MGANG     = 4,		//����
	THINK_OPERATOR_PENG      = 5,		//��
	THINK_OPERATOR_CHI       = 6,		//��
	THINK_OPERATOR_ABU       = 7,		//����
	THINK_OPERATOR_MBU       = 8,		//����
	THINK_OPERATOR_BU_HUA    = 9,		//����
	THINK_OPERATOR_ZHI_MGANG = 10,		//ֱ��
	THINK_OPERATOR_WAN_MGANG = 11,		//���
	THINK_OPERATOR_TING      = 12,		//����
	THINK_OPERATOR_AN_LOU    = 13,      //��¥
	THINK_OPERATOR_MING_LOU  = 14,      //��¥
	THINK_OPERATOR_SHUAI     = 15,		//˦��
	THINK_OPERATOR_CANCLE_TING=16,      //���Ƶ��²�����
	THINK_OPERATOR_BAO_ZUI	  =17,      //�����齫����
	THINK_OPERATOR_LAST_CARD_GUO	  =18,      //�����齫��������Ʋ��ܴ��,������ǹ����뷨���������Լ�������ƣ�Ȼ��ֱ���¼�����
	THINK_OPERATOR_NIU_CARD   =19,      //Ť��
	THINK_OPERATOR_QISHOU_TING = 20,	//Ϋ����������
	THINK_OPERATOR_MAI_AN_GANG = 21,    //��̨�򰵸�
	THINK_OPERATOR_QI_SHOU_MING_LOU=22,	//�޳�������¥
	THINK_OPERATOR_XU_NI_5         = 23,
	THINK_OPERATOR_ZHANG_MAO		= 24, //������ë
	THINK_OPERATOR_LUAN_MAO			= 25, //������ë
	THINK_OPERATOR_CHU_MAO			= 26, //��ë��
	THINK_OPERATOR_FANG_FENG		= 27,  //�ŷ�
	THINK_OPERATOR_LAIZI_GANG       = 28,  //��Ӹ�
	THINK_OPERATOR_PIZI_GANG        = 29,  //Ʀ�Ӹ�
	THINK_OPERATOR_HONG_ZHONG_GANG        = 30,  //���и�
	THINK_OPERATOR_QIANG_GANG_HU	= 31,  //���ܺ�
	};
#define DESK_USER_COUNT 4	//һ���������
#define DESK_USER_COUNT_TRIO 3	//3���淨������

#define INVAILD_POS		-1  //��Ч��λ��
#define EFFECTIVE_POS(pos, max_pos)	((pos) >= 0 && (pos) < (max_pos))

#define	PLAY_CARD_TIME		10  //����ʱ��
#define WAIT_READY_TIME		15  //׼��ʱ��

#define RESERVE_CARD_COUNT	15	//Ԥ��������7�װ���,15��
#define RESERVE_CARD_COUNT_JINAN	12	//Ԥ��������6���ƻ�ׯ �����齫
#define BASE_SCORE	1			        //�׷�

//�������䵱ǰ״̬
enum ROOM_STATE
{
	ROOM_FREE = 0,	//���ӿ���״̬
	ROOM_WAIT,			//���ӵȴ�״̬
	ROOM_PLAY,			//������Ϸ��
	ROOM_CONTINUE,      //һ�ֽ�����������Ϸ
	ROOM_END,           //��������
						//	DESK_COUNT_RESULT,	//���ڽ�����
						//	DESK_RESET_ROOM		//���������ɢ������
};

enum MATCH_RULES
{
	MATCH_CARD     = 1,           //����
};

enum MATCH_LEVEL
{
	MATCH_RULES_V0 = 0,           //V0�ʸ� ��ұ���̭ �����ʸ񱻿۳�
	MATCH_RULES_V1 = 1,           //V1�ʸ�
	MATCH_RULES_V2 = 2,           //V2�ʸ�
	MATCH_RULES_V3 = 3,           //V3�ʸ�
	MATCH_RULES_V4 = 4,           //V4�ʸ� 
	MATCH_RULES_V5 = 5,           //V5�ʸ�
	MATCH_RULES_V6 = 6,           //V6�ʸ�
};

enum MATCH_USER_STATE
{
	MATCH_USER_FREE = 0,	//���ӿ���״̬
	MATCH_USER_WAIT,			//���ӵȴ�״̬
	MATCH_USER_M_PLAY,			//������Ϸ��
	MATCH_USER_CONTINUE,      //һ�ֽ�����������Ϸ
	MATCH_USER_END,           //��������
	MATCH_USER_ERROR,         //����״̬
};

//���ӵ�ǰ״̬
enum DESK_STATE
{
	DESK_FREE =		0,	//���ӿ���״̬
	DESK_WAIT,			//���ӵȴ�״̬
	DESK_PLAY,			//������Ϸ��
};

//�ƾֵ�ǰ״̬
enum DESK_PLAY_STATE
{
	DESK_PLAY_START_HU,		//���ֺ�״̬
	DESK_PLAY_GET_CARD,		//����
	DESK_PLAY_THINK_CARD,	//˼��
	DESK_PLAY_END_CARD,		//������
	DESK_PLAY_SET_GA,		//����
	DESK_PLAY_BUY_ITEM,		//����
	DESK_PLAY_XUNI_5,       //��̨����5
	DESK_PLAY_BUY_AN_GANG,	//�򰵸�
};

//�Һ��������0-û����1-������2-���ڣ�3-����
enum WIN_TYPE
{
	WIN_ZIMO,		//���� 0
	WIN_BOMB,		//���� 1
	WIN_NONE,		//��ׯ 2
	WIN_MAIANGANG_WIN, //�򰵸�Ӯ 3
	WIN_MAIANGANG_LOSE,//�򰵸��� 4
};

//���Ʒ�ʽ
enum WIN_TYPE_SUB
{
	WIN_SUB_NONE,
	WIN_SUB_ZIMO,		//����
	WIN_SUB_BOMB,		//����
	WIN_SUB_ABOMB,		//����

	WIN_SUB_DZIMO,		//�������
	WIN_SUB_DBOMB,		//�������
	WIN_SUB_DABOMB,		//�������
};

//���ֺ�������
enum START_HU_INFO
{
	START_HU_NULL = 0,
	START_HU_QUEYISE=1,   	//ȱһɫ
	START_HU_BANBANHU,		//����
	START_HU_DASIXI,		//����ϲ
	START_HU_LIULIUSHUN,	//����˳
	START_HU_TIANHU,		//���
};
//�齫����
enum GameType
{
	HaiNanTrio = -11,
	ZhuanZhuan = -10,
	GuangDongJiPingHuMaJing = -9,
	HuNanMaJing = -8,
	ChangShaMaJing = -7,
	ShaoYangMaJiang = -6,
	HUNAMPAODEKUAI = -5,
	GuangDongCustomTuiDaoHu = -4,
	HeYuanJokerChickenHu = -3,
	GuangDongOneHundred = -2,
	GuangDongTuiDaoHuMaJing = -1,

	JiNanMaJiang       = 0,      //����
	LouBaoMaJiang      = 1,	     //ɽ��§��
	QingDaoMaJiang	   = 2,	     //ɽ���ൺ
	LinYiMajiang       = 3,      //�����齫
	HeZeMaJiang        = 4,		 //�����齫
	YanTaiMajiang      = 5,      //��̨�齫
	JiNingMaJiang      = 6,		 //�����齫
	TaiAnMaJiang       = 7,      //̩���齫
	WeiFangMaJiang     = 8,      //Ϋ���齫
	ZouChengMaJiang    = 9,	     //�޳��齫
	JiaXiangMaJiang    = 10,	 //�����齫
	JinXiangMaJiang    = 11,	 //�����齫
	LiaoChengMaJiang   = 12,	 //�ĳ��齫
	TengZhouMaJiang    = 13,	 //�����齫
	ZaoZhuangMaJiang   = 14,	 //��ׯ�齫
	WeiHaiMaJiang      = 15,	 //�����齫
	MouPingMajiang     = 16,	 //Ĳƽ�淨
	YanZhouMaJiang     = 17,     //�����齫
	LinQuMaJiang       = 18,     //�����齫
	HuaShuiMaJiang	   = 19,	 //��ˮ�齫

	CaoXianMaJiang		=20,	 //�����淨
	LongKouMajiang		= 21,	 //�����齫
	HaiNanMaJiang = 100,  //����

	
	
	/*********200--250����ռ��*****************/
	HuBeiYiPiLaiMaJiang = 201,   //����һƦ���
	HuBeiEnShiYingMaJiang = 202, //������ʩӲ�齫
	XianNingMaJiang			= 203, //����������Ӹ�
	TongShanMaJiang			= 204,  //ͨɽ�齫
	/****************************************/

	//1000����Ϊ�������淨��1000����Ϊ�齫�淨
	LBase_Dou_Di_Zhu = 1000,			// ������
	LBase_Jing_Dian_Dou_Di_Zhu = 1001,	//���䶷����,��������LBaseDouDiZhu��ĺ��ֵ��ͬ
	LBase_Wei_Fang_Pu_Ke = 2001,
};
//��������
enum HU_PAI_LEIXING
{
	HU_INVALID = 0,				//û��
	HU_XIAOHU = 1,				//С��
	HU_ZIMO = 7,					//����
	HU_GANGSHANGKAIHUA = 8,		//���Ͽ���
	HU_GANGSHANGPAO = 9,			//������
	HU_HAIDILAO = 10,				//������
	HU_HAIDIPAO = 11,				//������
	HU_QIANGGANGHU = 12,			//������
	HU_QISHOUFOURLAN = 13,		//�����ĸ�����
	HU_MENQING = 14,				//����
	HU_SANYUANPAIONE = 15,		//һ���з��׿�
	HU_SANYUANPAITWO = 16,		//�����з��׿�
	HU_FENGPAIKEQUAN = 17,		//���ƿ���Ȧ��ͬ
	HU_FENGPAIKEWEI = 18,			//���ƿ���λ��ͬ

	HU_JIHU = 101,				//����
	HU_PING = 102,				//ƽ��
	HU_PENGPENGHU = 103,		//������
	HU_HUNYISE = 104,			//��һɫ
	HU_BAOHU = 104,
	HU_HUNPENG = 105,			//����
	HU_QINGYISE = 106,			//��һɫ
	HU_QINGPENG = 107,			//����
	HU_HUNYAOJIU = 108,			//���۾�
	HU_XIAOSANYUAN = 109,		//С��Ԫ
	HU_XIAOSIXI = 110,			//С��ϲ
	HU_ZIYISE = 111,			//��һɫ
	HU_QINGYAOJIU = 112,		//���۾�
	HU_DASANYUAN = 113,			//����Ԫ
	HU_DASIXI = 114,			//����ϲ
	HU_SHISANYAO = 115,			//ʮ����
	HU_HAOHUAQIDUI = 116,		//�����߶�
	HU_QIDUI = 117,				//С�߶�
	HU_RENHU = 118,				//�˺�
	HU_DIHU = 119,				//�غ�
	HU_TIANHU = 120,			//���
	HU_JIULIANBAODENG = 121,	//��������

	HU_CHAOHAOHUAQIDUI = 122,   //�������߶�
	//HU_FOURGHOST = 122,			//�Ĺ��
	HU_YAOJIU = 123,			//�۾�
	HU_NOGHOST2TIMES = 124,		//�޹�ӷ�

	HU_FLOWERMATCHSINGLE = 125,	//������
	HU_FLOWERMATCHFLOWER = 126, //������
	HU_GENGZHUANGSUCC = 127,	//��ׯ�ɹ�

	HU_QING_QIDUI = 128,		//���߶�
	HU_QING_HAOHUAQIDUI = 129,	//������߶�
	HU_QING_PENGPENGHU = 130,	//����
	HU_JIANGYISE = 131,			//��һɫ
	HU_258JIANGYISE_JINAN = 150, //��һɫ
	HU_13BUKAO = 151,            //ʮ������
	HU_YITIAOLONG = 152,         //һ����
	HU_DADIAOCHE = 153,          //�����

	HU_DANDIAOJIANG = 154,          //������
	HU_HUIJIANG = 155,           //�Ὣ
	HU_SHENJIANG = 156,          //��
	HU_HUHUA = 157,              //����
	HU_8HUAHU = 158,             //8����
	HU_DANDIAO = 159,             //������
	HU_BIAN    = 160,
	HU_BIANKADIAO = 161,		  //���߿���
	HU_CHAOCHAOHAOHUAQIDUI = 162, //���������߶�
	HU_SHUAIHUN = 163,				//˦��
	HU_MAN_HUI_HU = 164,	//�����
	HU_YING_HU	  = 165,	//Ӳ��
	HU_QUAN_QIU_REN = 166,	//ȫ����
	HU_DUO_HUI_HU = 167,	//����

	///////////////////��������--����ռ��///////////////////////
	HU_RUANPIHU = 500,                   //��ƨ��
	HU_YINGPIHU = 501,                   //Ӳƨ��
	HU_RUANXIAOXUE = 502,                //��СѪ
	HU_YINHXIAOXUE = 503,                //ӲСѪ
	HU_RUANQUANQIUREN  = 504,            //��ȫ����
	HU_YINGQUANQIUREN  = 505,            //Ӳȫ����
	HU_RUANQINGYISE = 506,               //����һɫ
	HU_YINGQINGYISE = 507,               //Ӳ��һɫ
	HU_RUANJIANGYISE = 508,              //��һɫ
	HU_YINGJIANGYISE = 509,              //Ӳ��һɫ
	HU_RUANPENGPENGHU = 510,             //��������
	HU_YINGPENGPENGHU = 511,             //Ӳ������
	HU_RUANQIDUI      = 512,             //���߶�
	HU_YINGQIDUI      = 513,             //Ӳ�߶�
	HU_RUANHAOHUAQIDUI = 514,            //������߶�
	HU_YINGHAOHUAQIDUI = 515,            //Ӳ�����߶�
	HU_RUANCHAOHAOHUAQIDUI = 516,        //�������߶�
	HU_YINGCHAOHAOHUAQIDUI = 517,        //Ӳ�������߶�
	HU_SILAIZIHU           = 518,        //����Ӻ�
};

//�����ɢ����
enum RESET_ROOM_TYPE
{
	RESET_ROOM_TYPE_WAIT,
	RESET_ROOM_TYPE_AGREE,
	RESET_ROOM_TYPE_REFLUSE,
};

enum  PLAYTYPE
{
	PT_UNDEFINE = -1,
	PT_ZHANIAO0 = 0,
	PT_QIXIAODUI = 5,
	PT_ZHANIAO2 = 2,
	PT_ZHANIAO4 = 4,
	PT_ZHANIAO6 = 6,

	PT_GHOSTCARDNO = 7,
	PT_GHOSTCARDBAIBAN = 8,
	PT_GHOSTCARDFlop = 9,
	PT_REMOVEWINDCARDS = 10,

	PT_ZIMOHU = 11,
	PT_DIANPAOHU = 12,
	PT_KEQIANGGANG = 13,
	PT_HONGZHONG = 14,
	PT_ZHUANGXIAN = 15,			//ׯ��

	PT_QIXIAOFUIDOUBLESCORE = 16,
	PT_QIANGGANGHORSERSCORE = 17,
	PT_QIANGGANGHU = 18,

	PT_NOGHOST2TIMESSCORE = 19,
	PT_GANGSHANGKAIHUAQUANBAO = 20,
	PT_FOURGHOSTHU = 21,
	PT_FOURGHOSTHU2TIMESSCORE = 22,
	PT_DUIDUIHU2TIMESSCORE = 23,
	PT_QINGYISE4TIMESSCORE = 24,
	PT_YAOJIU6TIMESSCORE = 25,
	PT_ALLWIND8TIMES = 26,
	PT_SHISANYAO8TIMES = 27,
	PT_GHOSTCARDFLOPTWO = 28,

	PT_GHOSTCARDFLOWER = 29,		//��������
	PT_MINGGANGKEQIANG = 30,
	PT_YAOJIU1OR9 = 31,
	PT_QIXIAODUI4TIMESSCORE = 32,
	PT_GENGZHUANG = 33,
	PT_JIANGMA = 34,
	PT_MAGENGDIFEN = 35,
	PT_HONGZHONGZUOMA = 36,
	PT_HONGZHONGJIAFEN = 37,

	//�����齫
	PT_HAVE_FAN = 101,			//�з�
	PT_WU_FAN = 102,			//�޷�
	PT_LIAN_ZHUANG = 103,		//��ׯ
	PT_HAVE_GA = 104,			//�и�
	PT_LIU_JU_SUAN_GANG = 105,	//�������(��ܡ��������׸ܡ��׸�)
	PT_DIAN_PAO_ZI_FU = 106,	//���ڵ���Ը�
	PT_FENG_YAN = 107,			//���ƿ��Ե���
	PT_SHUN_HUA_HU = 108,		//˳����
	PT_3_NOT_FENG = 109,		//3���淨�޷���
	PT_3_NOT_EAT = 110,			//3���淨���ɳ�
	PT_FREEDOM_GA = 111,		//�����ϸ�

	//ɽ��§��
	PT_258_JIANG = 112,			//258����
	PT_CAN_EAT = 113,			//�ɳ�
	PT_CAN_JIE_PAO = 114,		//�ɽ���
	PT_CAN_QIANG_GANG = 115,	//������
	PT_13_YAO = 116,			//13�ۿɺ�
	PT_DIAN_GANG_KAI_HUA = 117,	//��ܿ����������ӣ�
	PT_MAI_3_PIAO = 118,		//��3Ʊ
	PT_MAI_5_PIAO = 119,		//��5Ʊ
	PT_FAN_XIANG_Multi = 120,	//�����
	PT_FAN_XIANG_ADD = 121,		//�����
	PT_DA_DIAO_CHE = 122,		//�����
	PT_AN_LOU = 123,	    	//���


	//�����齫
	//PT_FengDing32_JiNan = 200,		        //�ⶥ32
	//PT_FengDing64_JiNan = 201,               //�ⶥ64
	//PT_WuFengDing_JiNan = 202,               //���ⶥ
	PT_258_Jiang_JiNan = 203,		        //258����
	//PT_Luan_Jiang_JiNan = 204,               //�ҽ�
	//PT_CanChiPai_JiNan = 205,               //���Գ���
	//PT_WuFengPai_JiNan = 206,               //������
	//PT_MenQing_JiNan = 207,		            //����
	//PT_DaDiaoChe_JiNan = 208,			    //�����
	//PT_ShiSanYao_JiNan = 209,               //ʮ����
	//PT_HunYiSe_JiNan = 210,                 //��һɫ
	//PT_HaiDiLao_JiNan = 211,			    //��������
	//PT_GangBaoBaoPai_JiNan = 212,           //�ܱ�ȫ��
	PT_TINGCanHu_JiNan = 213,				//�����ſɺ�
	PT_3_REN_JiNan       = 214,                 //3���淨
	PT_4_REN_JiNan       = 215,                 //4���淨
	PT_MO_DAO_DI_JiNan = 216,                 //������
	PT_MING_LOU_JI_NAN		= 217,				//��¥
	PT_BU_DAI_FENG_JI_NAN   = 218,				//������
	PT_BU_DAI_HUA_JI_NAN	= 219,				//������
	
	   //�����齫
	PT_258_Jiang_LinYi     = 300,		        //258����
	PT_19_Jiang_LinYi      = 301,               //19����
	PT_Luan_Jiang_LinYi    = 302,               //�ҽ�
	PT_Only_ZiMoHu_LinYi   = 303,               //ֻ��������
	PT_CanChiPai_LinYi     = 304,               //���Գ���

	//��̨�齫
	PT_258_Jiang_YanTai = 350,                 //258����
	PT_Luan_Jiang_YanTai = 351,                //�ҽ�
	PT_Mai_An_Gang = 352,                      //�򰵸�
	PT_Can_Not_Chi_YanTai = 353,               //�����Գ�
	//PT_Yi_Tiao_Long_YanTai = 354,              //һ����
	//PT_Da_Diao_Che_YanTai = 355,               //�����
	//PT_Qing_YiSe_YanTai = 356,                 //��һɫ
	PT_3_REN_YanTai     = 357,                 //3���淨
	PT_4_REN_YanTai     = 358,                 //4���淨
	PT_MINGLOU_YanTai   = 359,                 //��¥
	PT_XUNI5_YanTai     = 360,                 //����5
	PT_DAIHUI_YanTai    = 361,                 //�������
	PT_ZIMOHU_YanTai    = 362,                 //������
	PT_Fan_Xiang_Multi_YanTai = 363,	        //�����
	PT_Fan_Xiang_Add_YanTai = 364,		        //�����
	PT_MAI_3_DAI_PIAO_YanTai = 365,	            //��̨ ��Ư
	PT_BUDAIFENG_YanTai      = 366,             //������
	//�ൺ�齫
	PT_Have_Hua_QingDao            = 400,		        //�л�
	PT_Can_Eat_QingDao             = 401,		        //�ɳ�
	PT_Have_Hui_QingDao            = 402,		        //�л���
	PT_Dan_Diao_Jiang_QingDao      = 403,		        //������
	PT_Yi_Tiao_Long_QingDao        = 404,		        //һ����
	PT_Hun_Yi_Se_QingDao           = 405,		        //��һɫ
	PT_Fan_Xiang_Multi_QingDao     = 406,	            //�����
	PT_Fan_Xiang_Add1_QingDao      = 407,		        //�����1��
	//PT_Fan_Xiang_Add10_QingDao     = 408,		        //�����10��
	PT_Pao_3_Zhang_QingDao         = 409,		        //��3Ʊ
	PT_Pao_5_Zhang_QingDao         = 410,		        //��5Ʊ
	PT_Yi_Pao_Duo_Xiang            = 411,		        //һ�ڶ���
	PT_NIU_PAI                     = 412,		        //Ť��

	//�����齫
	
	PT_He_Ze_Bao_Zui_1				= 500,  //����
	PT_He_Ze_Bao_Zui_2				= 501,
	PT_He_Ze_Bao_Zui_3				= 502,
	PT_He_Ze_Bao_Zui_4				= 503,  //�⼸��ֻ�ܳ���һ��

	PT_He_Ze_Mai_Zha_Dan			= 504,   //��ը��
	PT_He_Ze_Mai_Pao_Zi				= 505,   //������
	PT_He_Ze_Dai_Cai				= 506,   //����
	PT_He_Ze_Mai_Que_Men			= 507,	 //��ȱ��
	PT_He_Ze_Dai_Que_Men			= 508,	 //��ȱ��
	PT_He_Ze_Bu_Dai_Feng				= 509,	 //������
	PT_Bian_Ka_Diao					= 510,	 //�߿���
	PT_He_Ze_3_Peo					= 511,
	PT_He_Ze_4_Peo					= 512,
	PT_He_Ze_ZI_MO_HU				= 513,	//������
	
	//�����齫
	PT_3_Pu_Bao_Card_Ji_Ning		= 600, //���˰���
	PT_3_Ren_Ji_Ning				= 601, //�����淨
	PT_4_Ren_Ji_Ning				= 602, //�����淨

	//̩���齫
	PT_TAIAN_258_JIANG  = 650,            //258��
	PT_TAIAN_QING_YI_SE = 651,            //��һɫ
	PT_TAIAN_YI_TIAO_LONG = 652,          //һ����
	PT_TAIAN_QIDUI        = 653,          //�߶�
	PT_TAIAN_ZIMOHU       = 654,          //������
	PT_TIAN_CAN_NOT_EAT	  = 655,		  //��ѡ��
	PT_TIAN_XIA_PAO		  = 656,			//̩�����ܹ���

	//Ϋ���齫
	PT_WEIFANG_FENGDING_8FAN = 700,		           //�ⶥ8��
	PT_WEIFANG_FENGDING_16FAN = 701,               //�ⶥ16��
	PT_WEIFANG_FENGDING_NULL = 702,                //���ⶥ
	PT_WEIFANG_258_JIANG = 703,                   //258��
	PT_WEIFANG_HAVE_HUA  = 704,                   //�л���
	PT_WEIFANG_ZHUANG_DOUBLE_DI = 705,            //ׯ�ҵͷַ���
	PT_WEIFANG_CAN_NOT_EAT = 706,		          //���ɳ�
	//PT_WEI_FANG_ANLOUHUANPAI  = 707,             //��¥�ɻ���
	PT_WEIFANG_GANGBAO         = 708,             //�ܱ�ȫ���������ӣ�
	PT_WEIFANG_HAVE_HUI        = 709,             //�л���
	PT_WEIFANG_DAIFENG         = 710,             //����
	PT_WEIFANG_3_REN           = 711,             //3���淨
	PT_WEIFANG_BAOTING         = 722,             //�����ɺ�
	//�޳��淨
	PT_3_Ren_Zou_Cheng				= 801, //�����淨
	PT_4_Ren_Zou_Cheng				= 802, //�����淨
	PT_Zou_Cheng_Ming_Lou_Hu		= 803, //������¥��

	//�����淨
	PT_3_Ren_Jia_Xiang				= 901, //�����淨
	PT_4_Ren_Jia_Xiang				= 902, //�����淨

	//�����淨
	PT_3_Ren_Jin_Xiang				= 1001, //�����淨
	PT_4_Ren_Jin_Xiang				= 1002, //�����淨

	//�ĳ��淨
	PT_LIAO_CHENG_MING_DA			= 1101,	//����
	//�����淨
	PT_3_Ren_Teng_Zhou				= 1201,	//���˾�
	PT_4_Ren_Teng_Zhou				= 1202,	//���˾�

	//��ׯ�淨
	PT_3_Ren_Zao_Zhuang				= 1301, //���˾�
	PT_4_Ren_Zao_Zhuang				= 1302, //���˾�

	//�����淨
	PT_3_Ren_Wei_Hai				= 1401, //���˾�
	PT_4_Ren_Wei_Hai				= 1402, //���˾�

	PT_Pao_Fen_Wei_Hai				= 1403, //�ܷ�
	PT_Ming_Lou_Wei_Hai				= 1404, //��¥
	PT_Chang_Mao_Wei_Hai			= 1405, //��ë
	PT_Luan_Mao_Wei_Hai				= 1406, //��ë
	PT_Jiang_258_Wei_Hai			= 1407, //258����
	PT_Can_Not_Chi_Wei_Hai			= 1408, //���ɳ�
	PT_Zi_Mo_Hu_Wei_Hai				= 1409, //������

	//Ĳƽ�齫
	
	PT_Can_Not_Chi_MuPing = 1501,                 //���ɳ�          
	PT_3_REN_MouPing     = 1502,                 //3���淨
	PT_4_REN_MouPing     = 1503,                 //4���淨
	     
	PT_Pao_Fen_MuPing   = 1505,				    //���ܷ�
	PT_DAIHUI_MuPing    = 1506,                 //�������
	PT_ZIMOHU_MuPing    = 1507,                 //������
	PT_XiangCheng_MuPing    = 1508,                 //�������
	PT_XiangJia_MuPing    = 1509,                 //������
	PT_BUDaiFeng_MuPing     = 1510,					//����ѡ��

	//�����齫
	PT_3_Ren_Yan_Zhou = 1601, //�����淨
	PT_4_Ren_Yan_Zhou = 1602, //�����淨
	PT_DAI_HUA_Yan_Zhou = 1603, //����
	PT_BU_KE_CHI_Yan_Zhou = 1604, //���ɳ�
	PT_MING_LOU_Yan_Zhou = 1605, //��¥
	PT_ShiSanBuKao_Yan_Zhou = 1606, //ʮ������

	//�����齫
	PT_LINQU_FENGDING_8FAN = 1700,		           //�ⶥ8��
	PT_LINQU_FENGDING_16FAN = 1701,                  //�ⶥ16��
	PT_LINQU_FENGDING_NULL = 1702,                   //���ⶥ
	PT_LINQU_258_JIANG = 1703,                       //258��
	//PT_WEIFANG_HAVE_HUA = 704,                         //�л���
	PT_LINQU_ZHUANG_DOUBLE_DI = 1705,                 //ׯ�ҵͷַ���
	PT_LINQU_CAN_NOT_EAT = 1706,		               //���ɳ�
	//PT_WEI_FANG_ANLOUHUANPAI  = 707,                 //��¥�ɻ���
	PT_LINQU_GANGBAO = 1708,                          //�ܱ�ȫ���������ӣ�
	PT_LINQU_HAVE_HUI = 1709,                         //�л���
	PT_LINQU_DAIFENG = 1710,                          //����
	PT_LINQU_3_REN = 1711,                            //3���淨
	PT_LINQU_BAOTING = 1722,                          //�����ɺ�

	//��ˮ�齫
	//�����0-10
	PT_HUA_SHUI_Yu_Score_0	= 1800,
	PT_HUA_SHUI_Yu_Score_1	= 1801,
	PT_HUA_SHUI_Yu_Score_2	= 1802,
	PT_HUA_SHUI_Yu_Score_3	= 1803,
	PT_HUA_SHUI_Yu_Score_4	= 1804,
	PT_HUA_SHUI_Yu_Score_5	= 1805,
	PT_HUA_SHUI_Yu_Score_6	= 1806,
	PT_HUA_SHUI_Yu_Score_7	= 1807,
	PT_HUA_SHUI_Yu_Score_8	= 1808,
	PT_HUA_SHUI_Yu_Score_9	= 1809,
	PT_HUA_SHUI_Yu_Score_10 = 1810,

	PT_HUA_SHUI_258_Jiang	= 1811,		//258��
	PT_HUA_SHUI_Dai_Feng	= 1812,		//����
	PT_HUA_SHUI_Hong_Zhong_Lai_Zi	= 1813,		//��������
	PT_HUA_SHUI_Zi_Mo_Hu	= 1814,				//��ģ��
	PT_HUA_SHUI_Dan_Yu		= 1815,			//����
	PT_HUA_SHUI_Yu_Chi_Yu	= 1816,			//�����

	//�����淨

	PT_CAO_XIAN_BIAN_KA_DIAO	= 1901,					//�߿���
	PT_CAO_XIAN_BU_DAI_FENG		= 1902,					//������
	PT_CAO_XIAN_CAI_SHEN_4		= 1903,					//4��
	PT_CAO_XIAN_CAI_SHEN_8		= 1904,					//8��
	PT_CAO_XIAN_ZI_MO_HU		= 1905,					//������

	PT_CAO_XIAN_Bao_Zui_1		= 1906,  //����
	PT_CAO_XIAN_Bao_Zui_2		= 1907,
	PT_CAO_XIAN_Bao_Zui_3		= 1908,

	PT_CAO_XIAN_3_Peo			= 1909,					//���˾�
	PT_CAO_XIAN_4_Peo			= 1910,	//���˾�

	PT_CAO_XIAN_Buy_Que_Men		= 1911,  //��ȱ��

	//�����齫
	PT_LONG_KOU_3_PEO			=2001,
	PT_LONG_KOU_4_PEO			=2002,
	PT_LONG_KOU_DAI_HUI			= 2003,  //������
	PT_LONG_KOU_ZI_MU_HU		= 2004,  //������
	PT_LONG_KOU_BU_KE_CHI		= 2005,	 //���ɳ�
	PT_LONG_KOU_PAO_FEN			= 2006,	 //���ܷ�
	PT_LONG_KOU_ZHUANG_FAN_FAN			= 2007,	 //ׯ����

	/*******************����ռ��****************/

	//��ʩһƦ����
	PT_ENSHI_YIPILAI_GANGSHANGPAO = 3001, //��ʩһƦ����--������
	PT_ENSHI_YIPILAI_JINZHIYANGPI = 3002, //��ʩһƦ����--��ֹ��Ʀ

	//��ʩӲ�齫
	PT_ENSHI_YINGMJ_TAI_ZHUANG    = 3101, //��ʩӲ�齫--̧ׯ
	PT_ENSHI_YINGMJ_PENG_PENG_HU  = 3102, //��ʩӲ�齫--������
	PT_ENSHI_YINGMJ_QUANQIUREN    = 3103, //��ʩӲ�齫--ȫ����
	PT_ENSHI_YINGMJ_BAOSANJIA     = 3104, //��ʩӲ�齫--������

	//�����齫
	PT_XIAN_NING_DAI_BAIBAN_FACAI  = 4001,
	PT_XIAN_NING_3_FAN_HU			= 4002,
	PT_XIAN_NING_4_FAN_HU			= 4003,
	PT_XIAN_NING_SHUANG_DA_HU_4_FAN = 4004,	//˫����ķ�
	PT_XIAN_NING_ZHI_GANG_QIANG_GANG = 4005,	//ֱ������
	PT_XIAN_NING_TONGSHAN			 = 4006,	//����ͨɽ�淨

	PT_XIAN_NING_1_BEI_SCORE		= 4007,	//		1����
	PT_XIAN_NING_2_BEI_SCORE		= 4008,	//		2����
	PT_XIAN_NING_3_BEI_SCORE		= 4009,	//		3����
	PT_XIAN_NING_5_BEI_SCORE		= 4010,	//		5����

};


enum TYPE_HeZe_BuyItem
{
	HeZe_Buy_Min	= 0,
	HeZe_Buy_Zha_Dan = 1,	//������ը��
	HeZe_Buy_Pao_Zi = 2,	//����������
	HeZe_Buy_Max,
};

enum ErrorCode
{
	ErrorNone = 0,
	UserNotExist = 1,
	TaskNotExist = 2,
	ExceedMaximum = 3,
	AlreadyBinded = 4,
	NotQualified = 5,
	TaskNotFinished = 6,
	Expired = 7,
	BindingSelf = 8,
	IviterRegistRecently = 9,
	IviteeRegistLongTimeAgo = 10,
	CannotInviteEachother = 11,
	OutOfMoney = 12,
	OutOfChance = 13,
	NotBind = 14,
	Unknown = 100
};

enum EXCLUSIVE_PLAY
{
	EP_DOU_DI_ZHU = 1,		//������ר���淨�������Ӧ���淨�������������ļ���ExclusivePlay
};
//�����ID
enum APP_ID
{
	APP_MIAN_PACKET		= 33001,
	APP_YANTAI_PACKET	= 33002,
	APP_WEI_FANF_PACKET = 33003,
};

enum MaoType
{
	MT_NULL = 0,
	MT_FENG = 1,   //�������
	MT_JIAN = 2,	//�з�����ɵ�
	MT_ONE  = 3,	//һ��һͰһ����ɵ�
	MT_NINE	= 4,	//���������ͨ
	MT_MAX,			
};

// ֧������
enum PAY_TYPE
{
	PT_ONE_PAY = 0,		// ����֧��
	PT_ALL_PAY = 1,		// ������AA
};

enum SDK_Reward_Type
{
	Reward_Type_Card = 1,		// ����
	Reward_Type_RedPacket = 2,	// ���
	Reward_Type_Coupon = 3,		// ����ȯ
	Reward_Type_Object = 4,		// ʵ��
	Reward_Type_Jewel = 5,		// ��ʯ
	Reward_Type_Gold = 6,		// ���
};

#endif