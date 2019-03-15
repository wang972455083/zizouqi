#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "LIni.h"
#include "LSingleton.h"

class Config :public LSingleton<Config>
{
public:
	virtual	bool	Init();

	virtual	bool	Final();


	Lint	GetLogLevel();

	Lstring GetLogicManagerIp();
	Lshort GetLogicManagerPort();
	Lstring GetCenterKey();


	Lstring GetDBIp();
	Lshort GetDBPort();
	Lstring GetDBKey();


	Lstring GetInsideIp();
	Lshort GetInsidePort();
	bool	GetIfAddRobot();
	bool	GetDebugModel();
	bool	GetDebugModel(int debug);
	Lint	GetServerID();
	Lstring	GetServerName();
	Lint	GetExclusivePlay();

	Lint  GetDeskCircleTimeout();
private:
	void InitDebug();

private:
	LIniConfig	m_ini;

public://remote log
	Lstring GetRemoteLogUrl();
	Lstring GetRemoteLogCacheFileName();
	Lint GetMaxCachedLogSize();
	Lshort GetMaxCachedLogNum();
	Lint GetMaxFileLogSize();
	Lint GetRemoteLogThreadStart();

public:
	Lstring GetTeamIp();
	Lshort GetTeamPort();
	Lstring GetTeamKey();
private:
	Lshort		m_inPort;
	Lshort		m_outPort;

	Lstring		m_inIp;
	Lstring		m_outIp;
	Lint		m_addRobot;						//�Ƿ��Զ���ӻ�����
	Lint		m_DebugMod;
	Lint		m_ServerID;						//������ID
	Lint		m_ExclusivePlay;				//ר���淨,û����0
	std::set<int>	m_debug;
};

#define gConfig Config::Instance()

#endif