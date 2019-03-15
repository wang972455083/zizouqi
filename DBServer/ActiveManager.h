#ifndef _ACTIVE_MANAGER_H
#define _ACTIVE_MANAGER_H

#include "LSingleton.h"
#include "LMsgS2S.h"
#include "LRedisClient.h"

// ת��
#define ActivityDraw_Config "ActivityDraw_Config"						// ����
#define ActivityDraw_RemainTotalCount_ "ActivityDraw_RemainTotalCount_"	// ʣ��������
#define ActivityDraw_RemainTotalDate "ActivityDraw_RemainTotalDate"		// ��¼ʣ��������

//���
#define ActivityRedPackets_Config "ActivityRedPackets_Config"						// ����
#define ActivityRedPackets_Config_Tomorrow "ActivityRedPackets_Config_Tomorrow"		// ����������Ч
#define ActivityRedPackets_Config_Time "ActivityRedPackets_Config_Time"			// ������ʼʱ��	������Ч		
#define ActivityRedPackets_RemainTotalCount_ "ActivityRedPackets_RemainTotalCount_"	// ʣ��������
#define ActivityRedPackets_Remain          "ActivityRedPackets_Remain"				//	ʣ�����ܽ��
#define ActivityRedPackets_RemainTotalDate "ActivityRedPackets_RemainTotalDate"		// ��¼ʣ��������

// ����
#define ActivityShare_Config "ActivityShare_Config"
//��ת�̻ÿ����е�Ǯ��
#define NewZhuanPanActivityDailyDrawMoney "NewZhuanPanActivityDailyDrawMoney"

//�id
enum
{
	ACTIVITY_ZHUAN_PAN_REDPACKET = 1005,
	NEW_ZHUAN_PAN_ACTIVITY		 = 1006,  //��ת�̻
};

struct Activity
{
	Activity()
	{
		m_ActiveID = 0;
		m_BeginTime = -1;
		m_EndTime = -1;
	}

	Lint	m_ActiveID;		// ActivityID����
	Lstring	m_ActiveName;
	Lstring	m_Desc;
	Lint  m_BeginTime;
	Lint  m_EndTime;

	Lstring m_strConfig;		// ��̨�����ļ�
};

struct Money
{
	Lint m_money;
	Lint m_index;

	Money(Lint money,Lint index)
	{
		m_money = money;
		m_index = index;
	}
};

struct ActivityDrawGift
{
	ActivityDrawGift()
	{
		m_GiftIndex = -1;
		m_GiftType = 0;
		m_GiftCount = 0;
		m_GiftPercent = 0;
		m_GiftTotalCount = 0;
		m_GiftRemainTotalCount = 0;
		m_GiftEnsure = false;
		m_GiftResumeNextDay = false;
	}
	Lint m_GiftIndex;			// �������� �Զ�����
	Lstring m_GiftName;			// ��������
	Lint m_GiftType;			// �������� 1��������λ���� 2���ѣ���λԪ�� 3 �������￨����λԪ�� 4 ����NOTE 4X 32G����λ���� 5 С��5C����λ���� 6 Macbook pro 13����λ̨��
	Lint m_GiftCount;			// ��������
	Lint m_GiftPercent;			// ����Ȩֵ
	Lint m_GiftTotalCount;		// ���õĿ�� -1��ʾ����
	Lint m_GiftRemainTotalCount;// ʣ��Ŀ��  -1��ʾ����
	bool m_GiftEnsure;			// �������Ϊ0ʱ ȷ���еĽ�Ʒ
	bool m_GiftResumeNextDay;	// �����Ƿ�ָ����
};
struct ActivityRedPacketsGift
{
	ActivityRedPacketsGift()
	{
		m_GiftIndex = -1;
		m_GiftIntervalBottom = 0;
		m_GiftIntervalTop = 0;
		m_GiftPercent = 0;
		m_GiftTotalCount = 0;
		m_GiftRemainTotalCount = 0;
		m_GiftEnsure = false;
		m_GiftResumeNextDay = false;
	}
	Lint m_GiftIndex;			// �������� �Զ�����
	Lstring m_GiftName;			// ��������
	Lint m_GiftIntervalBottom;	// ������������
	Lint m_GiftIntervalTop;		// ������������
	Lint m_GiftPercent;			// ����Ȩֵ
	Lint m_GiftTotalCount;		// ���õĿ�� -1��ʾ����
	Lint m_GiftRemainTotalCount;// ʣ��Ŀ��  -1��ʾ����
	bool m_GiftEnsure;			// �������Ϊ0ʱ ȷ���еĽ�Ʒ
	bool m_GiftResumeNextDay;	// �����Ƿ�ָ����
};

struct ActivityDraw : public Activity
{
	ActivityDraw()
	{
		m_SpendType = 0;
		m_SpendCount = 0;
		m_GiftTotalDate = 0;
		m_GiftTime = 0;
		m_TotalGiftPercent = 0;
	}

	Lint m_SpendType;			// �������� 0��� 1��������λ����
	Lint m_SpendCount;			// ��������
	Lint m_GiftTotalDate;		// �����־ ���ڸ��ջָ�
	Lint m_GiftTime;			// ��ʾ�齱�Ĵ���
	std::vector<ActivityDrawGift> m_Gift;
	Lint m_TotalGiftPercent;	// �н��ܵ�Ȩֵ
};

struct ActivityShare : public Activity
{
	ActivityShare()
	{
		m_GiftType = 0;
		m_GiftCount = 0;
	}

	Lstring m_GiftName;			// ��������
	Lint m_GiftType;			// ������ 1��������λ����
	Lint m_GiftCount;			// ��������
};

struct ActivityRedPackets : public Activity
{
	ActivityRedPackets()
	{
		m_GiftTotalDate = 0;
		m_TotalGiftPercent = 0;
		m_limit = 0;
		m_left = 0;
		m_limiteResumeNextDay = false;
	}

	Lint m_GiftTotalDate;		// �����־ ���ڸ��ջָ�
	std::vector<ActivityRedPacketsGift> m_Gift;
	Lint m_TotalGiftPercent;	// �н��ܵ�Ȩֵ
	Lint m_limit;				// ���պ�������������������֣�
	Lint m_left;			// ʣ��ĺ���ܽ��֣�
	bool m_limiteResumeNextDay;  //���պ����������
};

//��ת���ͺ���
struct ActivityZhuanPanRedpacket
{
	Lint		m_drawCnt;
	Lint		m_drawTime;

	ActivityZhuanPanRedpacket()
	{
		m_drawCnt = 0;
		m_drawTime = 0;
	}
};

struct NewZhuanPanActivity
{
	Lint	m_userId;
	Lint	m_DailyMaxWinnerCnt;
	Lint	m_DailyMaxWinnerDarwCnt;
	Lint	m_DailyCreateRoomCnt;
	Lint	m_DailyCreateRoomDarwCnt;
	Lint	m_LastFenXiangTime;
	Lint	m_ContinueFenXiangDays;
	Lint	m_ResetTime;

	NewZhuanPanActivity()
	{
		m_userId = 0;
		m_DailyMaxWinnerCnt = 0;
		m_DailyMaxWinnerDarwCnt = 0;
		m_DailyCreateRoomCnt = 0;
		m_DailyCreateRoomDarwCnt = 0;
		m_LastFenXiangTime = 0;
		m_ContinueFenXiangDays = 0;
		m_ResetTime = 0;
	}

	void DailyClear()
	{
		m_DailyMaxWinnerCnt = 0;
		m_DailyMaxWinnerDarwCnt = 0;
		m_DailyCreateRoomCnt = 0;
		m_DailyCreateRoomDarwCnt = 0;
	}
};

class ActiveManager:public LSingleton<ActiveManager>
{
public:
	virtual	bool		Init();
	virtual	bool		Final();

	// ת�̽ӿ�====================================================
	// ����ת������ �ڲ�����������ͱ���
	void SetDrawActiveConfig(const Lstring& strSet,int id);
	// ���ת������
	void DrawSetClear();
	// ����ת������ �ڲ�����
	void _UpdateDrawActiveSet(const Lstring& strSet,int id);
	// �����ȡת������
	void SaveDrawActiveSet();
	void ReadDrawActiveSet();
	// �Ƿ���ת��
	bool CheckDrawActiveValid();
	//�û��Ƿ�ӵ�п���ת�̵�����
	bool CheckUserActiveValid();
	// ת��ID
	int GetDrawActiveId();
	int GetDrawBeginTime();
	int GetDrawEndTime();
	// ת������
	int GetDrawSpendType();
	int GetDrawSpendCount();
	// ����ת���н� fGift��ʾҪ���ɵļ�����
	ActivityDrawGift GetDrawGift( ActivityDrawGift& fGift );
	Lint    getRestTimes(Lint userid);
	bool	isRedisTimeNotToday(const Lstring& key);
	void	updateRedisKeyTime(const Lstring& key);
	void	increaseRedisKeyValue(const Lstring& key);
	//�˴���װ��Ŀ����Ϊ�˶��̵߳Ļ������
	bool    redisWriteDataByString(const std::string& strKey, int iWrite);
	bool    redisReadDataByString(const std::string& strKey, int& iRead);
	bool    redisSetKeyTTLTime(const std::string& strKey, unsigned int tim_secs);
	bool    redisReadDataByList(const std::string& strKey, unsigned int iFromIndex, unsigned int iReadCount, std::list<std::string>& listRead);
	bool    redisWriteDataByList(const std::string& strKey, const std::string& strWrite, unsigned int limit);
	// ����ӿ�
	void UpdateShareActiveSet(const Lstring& strSet,int id);
	Lstring GetShareActiveSet(); 
	int GetShareActiveId();

	ActivityShare GetShareSet();

	bool CheckShareActiveValid();
	bool CheckShareActiveStart();
	void ShareSetClear();

	//����ӿ�
	void SetRedPacketsActiveConfig(const Lstring& strSet, int id);
	void RedPacketsSetClear();
	void _UpdateRedPacketsActiveSet(const Lstring& strSet, int id);
	void SaveRedPacketsActiveSet();
	void ReadRedPacketsActiveSet();
	bool CheckRedPacketActiveValid();

	//������������redis��
	void _ReloadRedPacketsActiveSet(const Lstring& strSet, int id);
	void _ReloadRedPacketsActiveTime(const Lstring& strSet);

	// ���
	int GetRedPacketsActiveId();
	int GetRedPacketsBeginTime();
	int GetRedPacketsEndTime();
	Money GetRedPacketGift();
	int GetRaminGiftCount();
	void ReturnRedPacketBack(Lint index, Lint money, Lint invitee, Lint type);
	void ReloadRedPacketActivitySetTomorrow();


	//��ת�̻
	void UpdateZhuanPanRedpacketMap(Lint userId,ActivityZhuanPanRedpacket* pData);
	ActivityZhuanPanRedpacket* GetZhuanPanRedpacketData(Lint userId);
	bool	IsZhuanPanRedpacketOpen();	

	//����ת�̻
public:
	void InsertNewZhuanPanActivityMap(Lint userId,NewZhuanPanActivity* pData);
	NewZhuanPanActivity* GetNewZhuanPanActivityData(Lint userId);

public:
	boost::recursive_mutex m_Mutex;
	CRedisClient m_RedisClient;

	ActivityDraw m_Draw;
	ActivityShare m_Share;
	ActivityRedPackets m_RedPackets;

	std::map<Lint,ActivityZhuanPanRedpacket*>	m_mapZhuanPanRedpacket;
	std::map<Lint,NewZhuanPanActivity*>			m_mapNewZhuanPanActivity;
};

#define gActiveManager ActiveManager::Instance()

#endif

