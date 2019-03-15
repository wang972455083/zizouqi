#pragma once


#include "LBase.h"
#include "LRunnable.h"
#include "LSingleton.h"
#include <list>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp> 
#include <boost/thread/condition_variable.hpp>

#include "LMsgS2S.h"
struct QuickUser
{
	LUser user;			//���
	Llong m_quick_time;	//����ƥ���ʱ��

	QuickUser()
	{
		m_quick_time = 0;
	}
};

class QuickStartManager : public LSingleton<QuickStartManager> 
{

public:
	QuickStartManager();
	~QuickStartManager();

public:
	virtual	bool	Init();
	virtual	bool	Final();

	void Start();
	void Stop();
	void Join();
	void Sleep(int interval);

	bool GetStop();

	//void Clear();

	/*
	*	brief ������߳����еĺ��������ݾ����߼�ȥʵ��
	*/
	virtual void Run();

	void  PushUser(std::shared_ptr<QuickUser> user);
	bool  RemoveUser(int user_id);
	void  CheckQuickStart();
	void  Swap();
	int   GetKeyByScores(int score);

private:
	
	std::list<int>		m_users_list;
	boost::condition_variable_any		m_canCreateRoom;
	boost::condition_variable_any		m_notFull;
	boost::mutex						m_lock;
	boost::mutex						m_remove_lock;
	boost::shared_ptr<boost::thread>	m_pthread;
	bool								m_stop;


	std::list<std::shared_ptr<QuickUser> >					m_msgQueue[2];
	std::list<std::shared_ptr<QuickUser> >*					m_msgRecv;
	std::list<std::shared_ptr<QuickUser> >*					m_msgDoing;

	std::map<int, std::list<std::shared_ptr<QuickUser> > >		m_users;	//��һ���Ƿ���,ÿ50�ֵı�����һ��key������0�־�Ҳ����50��
	Llong								m_cur_time;
};

#define gQuickStart QuickStartManager::Instance()