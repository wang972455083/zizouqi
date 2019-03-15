#include "QuickStartManager.h"

#include "Work.h"


QuickStartManager::QuickStartManager()
{
	m_msgDoing = &m_msgQueue[0];
	m_msgRecv = &m_msgQueue[1];
	m_stop = false;
	LTime cur;
	m_cur_time = cur.MSecs();
}

QuickStartManager::~QuickStartManager()
{

}

bool QuickStartManager::Init()
{
	return true;
}

bool QuickStartManager::Final()
{
	return true;
}

void QuickStartManager::Start()
{
	if (!m_pthread)
	{
		m_pthread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&QuickStartManager::Run, this)));
	}
}

void QuickStartManager::Stop()
{
	m_stop = true;
}

void QuickStartManager::Join()
{
	if (m_pthread)
	{
		m_pthread->join();
	}
}

void QuickStartManager::Sleep(int interval)
{
	boost::this_thread::sleep(boost::posix_time::millisec(interval));
}


bool QuickStartManager::GetStop()
{
	return m_stop;
}


void QuickStartManager::Run()
{
	while (!GetStop())
	{
		LTime cur;
		if (cur.MSecs() - m_cur_time >= 1000)
		{
			m_cur_time = cur.MSecs();

			CheckQuickStart();
		}

		if (m_msgRecv->empty())
		{
			Sleep(1);
			continue;
		}
	
	}
}

void QuickStartManager::PushUser(std::shared_ptr<QuickUser> user)
{
	boost::mutex::scoped_lock l(m_lock);
	m_msgRecv->push_front(user);
}

bool QuickStartManager::RemoveUser(int user_id)
{
	if (user_id <= 0)
		return false;

	{
		boost::lock_guard<boost::mutex> guard(m_lock);
		for (auto it = m_msgRecv->begin(); it != m_msgRecv->end(); ++it)
		{
			if ((*it)->user.m_user_id == user_id)
			{
				m_msgRecv->erase(it);
				return true;
			}
		}
	}

	for (auto it = m_users.begin(); it != m_users.end(); ++it)
	{
		list<std::shared_ptr<QuickUser>>& user_list = it->second;
		for (auto iter = user_list.begin(); iter != user_list.end(); ++iter)
		{
			if ((*iter)->user.m_user_id == user_id)
			{
				user_list.erase(iter);
				return true;
			}
		}
	}

	
	return false;
}

void QuickStartManager::CheckQuickStart()
{
	LLOG_ERROR("CheckQuickStart");

	
	if (m_msgDoing->empty())
	{
		Swap();
	}

	while(!m_msgDoing->empty())
	{
		std::shared_ptr<QuickUser> user = m_msgDoing->back();
		m_msgDoing->pop_back();

		int key = GetKeyByScores(user->user.m_scores);

		auto it = m_users.find(key);
		if (it == m_users.end())
		{
			list<std::shared_ptr<QuickUser>> user_list;
			m_users[key] = user_list;
		}
		//新进来的插入到前边用来排序等待时间，即链表最后边的就是等待时间最长的，优先处理
		m_users[key].push_front(user);
	}

	//先匹配同分数段的玩家
	for (auto it = m_users.begin(); it != m_users.end(); ++it)
	{
		while (it->second.size() >= 2)
		{
			std::shared_ptr<QuickUser> quick_use_1 = it->second.back();
			it->second.pop_back();
			
			std::shared_ptr<QuickUser> quick_use_2 = it->second.back();
			it->second.pop_back();

			//匹配成功

			LMsgLM2LMQuckCreateRoom* send = new LMsgLM2LMQuckCreateRoom();
			
			send->m_users.push_back(quick_use_1->user);
			send->m_users.push_back(quick_use_2->user);

			gWork.Push(send);
		}
	}

	

	for (auto it = m_users.begin(); it != m_users.end(); ++it)
	{
		if (it->second.empty())
		{
			auto next_it = it;
			next_it++;

			if (next_it != m_users.end() && !next_it->second.empty())
			{
				std::shared_ptr<QuickUser> quick_use_1 = it->second.back();
				it->second.pop_back();

				std::shared_ptr<QuickUser> quick_use_2 = next_it->second.back();
				next_it->second.pop_back();

				//匹配成功
				LMsgLM2LMQuckCreateRoom* send = new LMsgLM2LMQuckCreateRoom();

				send->m_users.push_back(quick_use_1->user);
				send->m_users.push_back(quick_use_2->user);

				gWork.Push(send);
			}
		}
	}
}

void QuickStartManager::Swap()
{
	boost::mutex::scoped_lock l(m_lock);
	std::list<std::shared_ptr<QuickUser> >* tmp = m_msgDoing;
	m_msgDoing = m_msgRecv;
	m_msgRecv = tmp;
}

int QuickStartManager::GetKeyByScores(int score)
{
	if (score == 0)
		return 50;

	if (score % 50)
	{
		return ((score / 50) + 1) * 50;
	}
	else
	{
		return ((score / 50) * 50);
	}

	return 0;
}
