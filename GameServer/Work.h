#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LMsgS2S.h"
#include "DbServer.h"
#include "GarbageCollectionService.h"

#include "LCardsOperType.h"
#include "DUser.h"
#include "Table/ChessBoardCfg.h"

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
	//void			SaveCurrentOnline();
	LTime&			GetCurTime();

public:	
	
	bool		HanderCheckMd5(std::map<Lstring, Lstring>& param);
	


	void		HanderKeepAlive(LMsgS2SKeepAlive* msg);
	DBSession& GetDbSession();

	bool		IsSameDay(LTime t1,LTime t2);
	void		SendMsgToAllLogicManager(LMsg& msg);

	bool		IsRightAppID(Lint appId);
	void		PolicyBlock(DUser* user, Lint optType, Lint startTime, Lint endTime, bool sendLMG);

	template<typename T> string toString(const T& t);
	void		HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg);

public:
	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);
	void			HanderGateLogout(LMsgKick* msg);
	
	//������ҵ�½����Ϣ 
	void			HanderUserMsg(LMsgC2SMsg* msg);

	//socket����
	void			AddSp(LSocketPtr sp);
	void			DelSp(LSocketPtr sp);
	GateInfo*       GetGateInfoBySp(LSocketPtr sp);

	void			HanderGateLogin(LMsgG2ServerLogin* msg);
	void			DelGateInfo(Lint id);
	void			HanderGateUserMsg(LMsgG2GameUserMsg* msg);
	void			HanderUserLogin(LSocketPtr sp,LMsgC2SLogin* msg);
	void			HanderUserLogOut(LMsgG2SUserLogOut* msg);
	void			HanderUserCreateXiangQiRoom(LMsgC2SCreateXiangQiRoom* msg);
	void			HanderUserAddIntoXiangQiRoom(LMsgC2SAddIntoRoom* msg);
	void			HanderUserQuickStart(LMsgC2SQuickRoomOpt* msg);
	void			HanderUserGetBagBlockList(LSocketPtr sp,LMsgC2SGetBagBlockList* msg);
	void			HanderUserbagBlockUpgrade(LSocketPtr sp, LMsgC2SBagBlockUpgrade* msg);
public:
	void			SendUserMsg(LSocketPtr sp,int user_id,LMsg& msg);
private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

private:
	;
	std::map<Lint, GateInfo> m_gateInfo;
	
	std::map<Lint, Lstring> serverInfo;

	

	DBSession		m_dbsession;

	GarbageCollectionService m_gcService;
	boost::mutex m_mutexLogicManager;

private:
//	CRedisClient	m_RedisClient;
protected:
	// ��������
	bool		_initRedisAndActive();
	void        SendActiveSetToLM(Lint LMid);
private:
//	Lstring         m_strHorse;           //�������Ϣ
	std::map<Lint, Lstring>          m_mapAppId2StrSet;


private:
	TCPClientPtr  m_logicManager;

public:
	void			ConnectToLogicManager();
	void			SendGameLoginToLM();

};


#define gWork Work::Instance()

#endif