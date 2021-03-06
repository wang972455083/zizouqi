#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LNet.h"
#include "LMsgS2S.h"
#include "TCPClient.h"
#include "GarbageCollectionService.h"
#include "LCardsOperType.h"
#include "User.h"



struct UserLoginInfo
{
	Lint	m_seed;
	Lint	m_time;
	LUser	m_user;
};

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
	//md5加密算法
	//Lstring			Md5Hex(const void* data,Lsize size);
public:
	//处理http消息 
	void			HanderHttp(LMsgHttp* msg);	
	//处理客户端连接上来消息
	void			HanderClientIn(LMsgIn* msg);
	//处理客户端掉线的消息 
	void			HanderUserKick(LMsgKick* msg);
	void			ConnectToLogicManager();
	void			SendToLogicManager(LMsg& msg);
	
public:
	void			HanderGateLogin(LMsgG2ServerLogin* msg);
	void			HanderGateLogout(LMsgKick* msg);
	GateInfo*		GetGateInfoBySp(LSocketPtr sp);
	GateInfo*		GetGateInfoById(Lint id);
	void			DelGateInfo(Lint id);
	void			HanderGateUserMsg(LMsgG2LUserMsg* msg, GateInfo* gate);
	void			HanderKeepAlive(LMsgS2SKeepAlive* msg);
	void			HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg);
private:
	void			SendLogicLoginToLM();

public:

	void			HanderQuitRoom(int gate_id,LMsgC2SQuitRoom* msg);
	//void			HanderChessMove(LMsgC2SChessMove* msg);
	//void			HanderChessAttack(LMsgC2SChessAttack* msg);
	

	void			HanderQuickCreateRoomOpt(LMsgLM2LQuickCreateRoomOpt* msg);
//	void			HanderAutoSelectCard(LMsgL2LAutoSelectCard* msg);
	void			HanderLM2LUserLogout(LMsgLM2LUserLogOut* msg);
	void			HanderLM2LUserLogin(LMsgLM2LUserLogin* msg);
public:
	void			SendMessageToUser(LSocketPtr sp, Lint user_id, LMsg& msg);
	void			SendMessageToUser(int gateId, int user_id, LMsg& msg);
	//void			FillDeskMsg(LMsgL2LMDeskOpt& send, LDeskPtr desk);

	//**************************************
public:
	void			HanderCreateXiangQiRoom(LMsgS2SCreateXiangQiRoom* msg);
	void			HanderAddInToXiangQiRoom(LMsgS2SAddIntoXiangQiRoom* msg);

	void			HanderReadyOpt( LMsgC2SReadyOpt* msg);

	void			HanderAddBlockToChess(LMsgC2SAddBlockToChess* msg);
	void			HanderMoveChessBlock(LMsgC2SChessBlockMove* msg);
	void			HanderChessBlockAttack(LMsgC2SChessBlockAttack* msg);
	void			HanderChessBlockUpgrade(LMsgC2SChessBlockUpgrade* msg);
	void			HanderChessBlockZhuiSha(LMsgC2SChessBlockZhuiSha* msg);

private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30毫秒循环一次

	Llong		m_200MSceTick;//200毫秒循环一次

	Llong		m_1SceTick;//1秒循环一次

	Llong		m_5SceTick;//5秒循环一次

	Llong		m_30SceTick;//30秒循环一次

	Llong		m_600SceTick;//5分钟秒循环一次

private:
	TCPClientPtr	m_logicManager;
	std::map<Lint, GateInfo> m_gateInfo;
	bool				m_loginLMFirst;
	GarbageCollectionService m_gcService;
};

#define gWork Work::Instance()

#endif