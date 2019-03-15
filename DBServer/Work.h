#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LNet.h"
#include "LMsgS2S.h"
#include "BlockingQueue.h"
#include "DBSession.h"
#include "ActiveManager.h"
#include "LCardsOperType.h"

class ShipInfo
{
public:
	ShipInfo(Lint inviter,Lint fcount, Lint scount)
	{
		m_inviter = inviter;
		m_count = fcount;
		m_secondCount = scount;
		m_inviteeFirst = false;
		m_inviterFirst = false;
		m_inviteeSecond = false;
		m_inviterSecond = false;

	}
	Lint m_inviter;
	Lint m_count;
	Lint m_secondCount;
	bool m_inviteeFirst; //��һ����ȡ
	bool m_inviterFirst; 
	bool m_inviteeSecond; //�ڶ�����ȡ
	bool m_inviterSecond;
private:
	ShipInfo();
};

enum TYPE_DRAW
{
	DRAW_NULL = 0,
	DRAW_BIG_WINNER = 1,
	DRAW_CREATE_ROOM = 2,
	DRAW_FEN_XIANG = 3,
};

struct BaseInfo
{
	Lint	m_id;
	Lint	m_userId;
	Lstring	m_city;
	Lstring m_common_play;

	BaseInfo()
	{
		m_id = 0;
		m_userId = 0;
	}
};

class RewardCfg;

class Work:public LRunnable,public LSingleton<Work>
{
public:
	virtual bool	Init();
	virtual bool	Final();

	virtual void	Start();
	virtual void	Run();
	virtual void	Clear();
	virtual	void	Join();
	virtual	void	Stop();

	void			Tick(LTime& cur);
	void			HanderMsg(LMsg* msg);

	LTime&			GetCurTime();

public:
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);

	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);

	//���24Сʱ������־�� ÿ������6��ִ�� 
	void			ClearOldLogVideo();

public:

	void			SendMsgToMatchServer(LMsg& msg);

	void			SendToClient(LSocketPtr client, LMsg& msg);

	//logicmanager��¼����
	void			HanderLogicManagerLogin(LMsgLMG2LdbLogin* msg);

	//matchserver��¼����
	void	HanderMatchServerLogin(LMsgM2LdbLogin* msg);
	//matchserver����洢��������
	void    HanderMatchReqSaveScore(LMsgM2LDBScoreboard* msg);
	//matchserver����洢�ֽ���������Ϣ
	void    HanderMatchSaveResultInfo(LMsgM2DbMatchResultInfo* msg);
	//logic������ְ�
	void    HanderLogicReqScoreBoard(LMsgL2LDBReqScoreBord* msg);
	//������������¼
	void    HanderRequestRenManMatchRecord(LMsgL2DBRequestRenManMatchRecord* msg);
	//������ת�̻������ˢ��
	void	HanderZhuanPanNotice(LMsgS2LDZhuanPanNotice* msg);

	//logic��¼����
	void			HanderLogicLogin(LMsgL2LdbLogin* msg);

	//logic�������ս��
	void			HanderLogicReqVipLog(LMsgL2LBDReqVipLog* msg);

	//logic�������¼��
	void			HanderLogicReqVideo(LMsgL2LDBReqVideo* msg);

	//logic���󱣴�ս��
	void			HanderLogicReqSaveLog(LMsgL2LDBSaveLogItem* msg);

	//logic���󱣴�¼��
	void			HanderLogicReqSaveVideo(LMsgL2LDBSaveVideo* msg);

	//logic����ĳ����¼��
	void			HanderLogicReqShareVideo(LMsgL2LDBGetShareVideo* msg);

	//logic������ҷ����¼��
	void			HanderLogicGetShareVideo(LMsgL2LDBGetSsharedVideo* msg);

	//logic������һ�ȡĳ����¼��Id
	void			HanderLogicGetShareVideoId(LMsgL2LDBGGetShareVideoId* msg);

	//����share video id
	void			HanderLogicSaveShareVideoId(LMsgL2LDBSaveShareVideoId* msg); 


	//����齱��Ϣ����
	void		HandleLogicReqActive(LMsgL2LDBReqActive* msg);

	//����齱��Ϣ
	void		HandleLogicSaveSyncDraw(LMsgL2LDBSyncDraw* msg);

	//�޸ĳ齱�绰
	void		HandleLogicUpdatePhone(LMsgL2LDBSyncPhone* msg);	

	void		HanderKeepAlive(LMsgS2SKeepAlive* msg);

	void		HanderUserReward(LMsgLMG2LDBUserReward *msg);

	/******************************************************************/
	// ����
	void        HanderUserLogin(LMsgLMG2LdbUserLogin* msg);
	void        HanderLogicSetActivity(LMsgCE2LSetActivity* msg);
	void        SetDrawActive(const Lstring& strSet, int id);
	
	void        HanderActivityRequestShareForDraw(LMsgLGM2LDBActivityRequestShareForDraw *msg);
	void        HanderAddUserPlayCount(LMsgL2LMGModifyPlayCount* msg);
	void        HanderActivityRequestDrawOpen(LMsgLGM2LDBActivityRequestDrawOpen *msg);
	void        HanderActivityRequestDraw(LMsgLGM2LDBActivityRequestDraw *msg);
	void        HanderActivityRequestLog(LMsgLGM2LDBActivityRequestLog *msg);
	void        HanderActivityPhone(LMsgLGM2LDBActivityWritePhone *msg);
	// ���͸����
	void	SendToUser(LSocketPtr LSp,const Lint& userid, const Lstring& uuid ,LMsg& msg);
	/*****************************************************************/
	void        HanderUserCreateMatch(LMsgS2SCreateMatch* msg);
	void        HanderUserDissolveMatch( LMsgS2SDissolveMatch* msg);
	void	    HanderUserCheckCanCreateMatch(LMsgS2SCheckCanCreateMatch* msg);
	/*****************************************************************/
	void		HanderActivityRedPacketBind(LMsgCe2LDBBindingRelation* msg);
	void		HanderDeskUser(LMsgL2LDBDeskUser* msg);
	void		_inviteProcess(Lint invitee, Lint regtime);
	void		HanderRequestRedPacket(LMsgL2DBRequestRedpacket* msg);
	void		SetRedPacketsActive(const Lstring& strSet, int id);
	void		SetRedActivityFalse(Lint invitee, Lint type);
	void		SetRedActivityTrue(Lint invitee, Lint type);

	void		SendToLMUser(const Lint& userid, LMsg& msg);
	void		SaveMysql(Lstring sql);
	void		HanderGetInvitingInfo( LMsgL2DBGetInvitingInfo* msg );
	//����Ļ����
	void		_reloadActivity();
	void		_loadFriendship();

	/******************************************************************/
private:
	void		HandleMsgLoop(void);

public:
	ActivityZhuanPanRedpacket*		LoadZhuanPanRedpacketData(Lint userId);
	void							HanderZhuanPanRedpacketOpt(LMsgL2DBZhuanpanRedpacket* msg);
public:
	NewZhuanPanActivity*			LoadNewZhuanPanActivityData(Lint userId);
	bool							ResetNewZhuanPanActivityData(NewZhuanPanActivity* pActivity);
	bool							SaveNewZhuanPanActivityToSql(NewZhuanPanActivity* pActivity);
	int								GetDrawTimes(NewZhuanPanActivity* pActivity);
	TYPE_DRAW						GetDrawType(NewZhuanPanActivity* pActivity);
	void							DrawEndSetData(NewZhuanPanActivity* pActivity,TYPE_DRAW type);
	void							SaveRewardToSql(Lint userId,RewardCfg* pReward);
	void							HanderNewZhuanPanActivityOpt(LMsgL2DBNewZhuanPanActivity* msg);
	void							HanderJiaXiangOpt(LMsgL2DBJiaXiangOpt* msg);
public:
	void							LoadBaseInfoFromSql();
	BaseInfo*						CreateBaseInfo(Lint userId);
	BaseInfo*						GetBaseInfo(Lint userId);
	void							SaveBaseInfo(BaseInfo* pInfo);

private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_5SceTick;

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

private:

	BlockingQueue<LMsg*>		m_queue;
	boost::thread_specific_ptr<DBSession>	m_dbsession;
	DBSession								m_Maindbsession;
	LSocketPtr	m_logicManager;//����manager
	LSocketPtr	m_MatchServer;//����MatchServer
	boost::mutex m_mutexUser2recommIdMap;
	boost::recursive_mutex m_Mutex;


private:
	std::map<Lint,ShipInfo> m_user2recommIdMap;
	std::map<Lint, BaseInfo*>	m_mapBaseInfo;
};


#define gWork Work::Instance()

#endif