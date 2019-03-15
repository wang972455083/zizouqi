#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LMsgS2S.h"
#include "DbServer.h"
#include "GarbageCollectionService.h"
#include "LRedisClient.h"
#include "LCardsOperType.h"
#include "DUser.h"

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
	void			SaveCurrentOnline();
	LTime&			GetCurTime();

public:	
	void		HanderLogicLogin(LMsgL2CeLogin* msg);
	void		HanderLogicGateInfo(LMsgL2CeGateInfo* msg);
	void		HanderLogicSaveUser(LMsgL2CeSaveUser* msg);
	void		HanderLogicSaveCardInfo(LMsgL2CeSaveCardInfo* msg);
	void		HanderLogicLogout(LMsgKick*msg);
	GateInfo*	GetGateUsefulInfo(Lint logicID);
	void		SendMsgToLogic(LMsg& msg, Lint logicID);
	void		HanderLogicUserLogin(LMsgL2CeUserServerLogin* msg);
	void		HanderLogicUserLogout(LMsgL2CeUserServerLogout* msg);
	Lstring&	getServerNameByID(Lint serverID);
	void		HanderLogicUserReLogin(LMsgL2CeUerReLogin* msg);
	void		HanderModifyUserPlayCount(LMsgL2LMGModifyPlayCount* msg);
	void		HanderLogicManagerCardExchange(LMsgLMG2CeCardExchange* msg);
	void		HanderLastRelief(LMsgLMG2CeLastRelief* msg);
	void		HanderPhoneNumber(LMsgLMG2CePhoneNumber* msg);
	void        HanderSetFreeSet(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void        HanderActivityDrawSet(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderFenXiang(LMsgLMG2CeFenXiang* msg);
	void		HanderLogicRestBlockUser(LMsgLMGResetBlockUser* msg);
public:
	void		HanderGMCharge(std::map<Lstring,Lstring>& param,LSocketPtr sp);
	void		HanderGMHorse(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMBuyInfo(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMOnlineCount(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMCloseServer(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMboard(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMPaySwitch(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	bool		HanderCheckMd5(std::map<Lstring, Lstring>& param);
	void		HanderActivityRedPackets(std::map<Lstring, Lstring>& param, LSocketPtr sp);
	void		HanderGMPolicyBlock(std::map<Lstring, Lstring>& param, LSocketPtr sp);

public:
	void		HanderLGWrappedMsg(LMsgLGCEUserMsgWrapper* msg);
	int			HanderUserLogin(LMsgC2SMsg* msg, LMsgS2CMsg& ret);

	void		HanderUserLocalLogin(LMsgC2SMsg* msg, LMsgS2CMsg& ret);
	int			HanderUserWechatLogin(LMsgC2SMsg* msg, LMsgS2CMsg& ret);
	void		HanderKeepAlive(LMsgS2SKeepAlive* msg);
	DBSession& GetDbSession();

	bool		IsSameDay(LTime t1,LTime t2);
	void		SendMsgToAllLogicManager(LMsg& msg);

	bool		IsRightAppID(Lint appId);
	void		PolicyBlock(DUser* user, Lint optType, Lint startTime, Lint endTime, bool sendLMG);
	void		HanderDeviceCheckRequest(LMsgDeviceCheckRequest* msg);
	Lstring		_makeParam(std::map<Lstring, Lstring>& param);
	Lint		_checkDeviceWhenReg(Lstring deviceId, Lstring& errMsg);
	template<typename T> string toString(const T& t);


public:
	//����http��Ϣ 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);
	
	//������ҵ�½����Ϣ 
	void			HanderUserMsg(LMsgC2SMsg* msg);

	//socket����
	void			AddSp(LSocketPtr sp);
	void			DelSp(LSocketPtr sp);
private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

private:
	//	LSocketPtr	m_logicClient;
	//	std::map<Lint, GateInfo> m_gateInfo;
	std::map<Lint, LSocketPtr> m_logicClient;
	std::map<Lint, std::map<Lint, GateInfo> > m_gateInfo;
	std::map<LSocketPtr, Lint>	m_spMap;
	std::map<Lint, Lstring> serverInfo;

	std::map<Lint,Lint> m_onlineUserCount;

	DBSession		m_dbsession;

	GarbageCollectionService m_gcService;
	boost::mutex m_mutexLogicManager;

private:
	CRedisClient	m_RedisClient;
protected:
	// ��������
	bool		_initRedisAndActive();
	void        SendActiveSetToLM(Lint LMid);
private:
//	Lstring         m_strHorse;           //�������Ϣ
	std::map<Lint, Lstring>          m_mapAppId2StrSet;
};


#define gWork Work::Instance()

#endif