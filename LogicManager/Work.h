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
#include "RoomManager.h"

class User;

struct UserLoginInfo
{
	Lint	m_seed;
	Lint	m_time;
	LUser	m_user;
};

struct StrBoardInfo
{
	Lstring m_strInfo;
	Lstring m_strId;
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
	
	//md5�����㷨
	//Lstring			Md5Hex(const void* data,Lsize size);
public:
	//����http��Ϣ 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);

	void            HanderKeepAliveAck(LMsgS2SKeepAliveAck*msg);
	
	void            HanderKeepAlive(LMsgS2SKeepAlive*msg);

	//������ҵ�½����Ϣ 
	void			HanderUserLogin(Lint id,Lint seed,Lint gateId,Llong geteUserId,Lstring& md5,Lstring& ip, const Lstring &location);

	//������Ҷ�����������Ϣ 
	void			HanderUserReLogin(Lint id,Lint seed,Lint gateId,Llong gateUserId,Lstring& md5,Lstring& ip);
	
	//������ҵ�½����Ϣ 
	void			HanderUserLogout(Lint gateId, Llong geteUserId);

public:
	void			ConnectToCenter();

	void			SendToCenter(LMsg& msg);
	//����������Ϣ��center
	void			SendGateInfoToCenter();
	
	//����logic��Ϣ������gate
	void			SendLogicInfoToGates(Lint nGateID = 0);



	//center�������ر�
	void			HanderCenterLogout();
	void			HanderDBServerLogout();
	UserLoginInfo*  GetUserLoginInfo(Lint userId);
	void			DelUserLoginInfo(Lint userId);
	
	

	


	void			HanderGateLogin(LMsgG2ServerLogin* msg);
	void			HanderGateLogout(LMsgKick* msg);
	GateInfo*		GetGateInfoBySp(LSocketPtr sp);
	GateInfo*		GetGateInfoById(Lint id);
	void			DelGateInfo(Lint id);

	void			HanderGateUserMsg(LMsgG2LMUserMsg* msg,GateInfo* gate);
public:
	void			HanderLogicLogin(LMsgL2LMGLogin* msg);
	void			HanderLogicLogout(LMsgKick* msg);
	
	LogicInfo*		GetLogicInfoBySp(LSocketPtr sp);
	
	LogicInfo*		GetLogicInfoById(Lint id);
	
	LogicInfo*		GetLogicInfoByPresure();
	
	LogicInfo*		GetLogicInfoByGameType(Lint game_type);
	void			DelLogicInfo(Lint id);
	


public:
	void SendMessageToLogic(Lint logicID, LMsg& msg);
	void SendMessageToGate(Lint gateID, LMsg& msg);
	void SendMessageToAllLogic(LMsg& msg);
	void SendMessageToAllGate(LMsg& msg);

	void SendMessageToUser(LSocketPtr sp, Lint user_id,LMsg& msg);
	

private:
	void			checkAndTouchLogic();
	void			TryToReconnect();
	void			SendLoginToCenter();
	
	void			EraseExpiredLogics(LTime& cur);
	void			OnUserLogin(User *user);


	//�ָ
	void			HanderGame2LMLogin(LMsgGame2LMGLogin* msg);
	void			HanderGameLogout(LMsgKick* msg);
	void			SendGameServerInfoToGates(Lint GateID = 0);
	GameServerInfo*	GetGameInfoBySp(LSocketPtr sp);
	void			DelGameInfo(Lint id);



public:

	//void			HanderDeskOpt(LMsgC2SDeskOpt* msg);
	
	//void			HanderDeskOptRespon(LMsgL2LMDeskOpt* msg);
	
	//void			FillDeskMsg(DeskMsg& send, LMsgL2LMDeskOpt* msg);

	void			HanderGame2LMQuickRoomOpt(LMsgGame2LMQuickStartOpt* msg);

	void			HanderUserLogin(LSocketPtr sp,LMsgC2SLMLogin* msg);
	void			HanderUserLogout(LMsgG2SUserLogOut* msg);

	void			ModifyUserStatus(int user_id, USER_STATUS status, int logic_server_id, LSocketPtr sp);
	void			HanderLM2LMQuickCreateRoom(LMsgLM2LMQuckCreateRoom* msg);
	void			HanderL2LMQuickCreateRoomOpt(LMsgL2LMQuickCreateRoomOpt* msg);
	void			HanderL2LMQuitRoom(LMsgL2LMQuitRoom* msg);
	void			HanderRecyleRoom(LMsgL2LMRecyleRoom* msg);

	//fenxian
public:
	void			HanderS2SCreateXiangQiRoom(LMsgS2SCreateXiangQiRoom* msg);
	void			HanderL2LMCreateXiangQiRoomResult(LMsgL2LMCreateRoomResult* msg);
	void			HanderS2SAddIntoXiangQiRoom(LMsgS2SAddIntoXiangQiRoom* msg);
	void			HanderL2LMAddInToXiangQiRoom(LMsgL2LMAddIntoRoomResult* msg);



private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_5SceTick;//5��ѭ��һ��

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

	Llong		m_3600SecTick;	// 1Сʱѭ��һ��
	bool		m_IsLogicDB;	//Logic״̬�� false ��Ҫ����
private:


	std::map<Lint, GateInfo> m_gateInfo;

	std::map<Lint, LogicInfo> m_logicInfo;
	
	std::map<Lint, std::map<Lint, Lint> > m_mapLogicServerOnlySupportType; //��Щ������ֻ֧����Щ��Ϸ���͡���ʽ�� ������logicserver����������Ϸ����

	std::map<Lint, Lint>	  m_toEraseLogics;

	std::map<Lint, UserLoginInfo> m_reqLoginUser;//�ȴ���¼����Ҷ���



	GarbageCollectionService m_gcService;
private:
	bool m_open_ios_pay;
	bool m_open_android_pay;

	


	//�ָ
	std::map<Lint, GameServerInfo> m_game_server_info;



};

#define gWork Work::Instance()

#endif