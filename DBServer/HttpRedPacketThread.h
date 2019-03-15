#ifndef __HTTP_RED_PACKET_THREAD__
#define __HTTP_RED_PACKET_THREAD__

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <iostream>
#include "LBase.h"
#include "LSingleton.h"
#include "CircularArrayQueue.h"
#include "HttpRequest.h"

using namespace std;

enum RedPacketType
{
	inviteeFirst  =  1, //�������ߵ�һ����ȡ
	inviterFirst  =  2, //�����ߵ�һ����ȡ
	inviteeSecond =  3, //�������ߵڶ�����ȡ
	inviterSecond =  4, //�����ߵڶ�����ȡ
};

struct RedPacketRequest
{
	Lint	m_userId;
	Lint	m_invitee; //ÿ����������ȷ��4�����
	Lint    m_type;
	Lstring m_url;
	Lint    m_index;   //�����λ
	Lint	m_money;   //������
};

class HttpRedPacketThread : public LSingleton<HttpRedPacketThread>
{
	typedef CircularArrayQueue<RedPacketRequest> UrlQueue;

public:
	int Init(int start);
	void start();
	void stop();
	void push(RedPacketRequest& url);
	void requestRedPacket(RedPacketRequest& url);

	// ����LSingletonһ��Ҫǿ��ʵ�������������о��Ƚ�...
	bool Init()
	{
		return true;
	}

	bool Final()
	{
		return true;
	}

private:
	void run(void);
	boost::thread* _thread;
	boost::shared_ptr<UrlQueue>  _queue_ptr;
	int _start;
	boost::mutex m_mutexUrlQueue;
	boost::mutex m_mutexCatchFile;
};

#define gHRP HttpRedPacketThread::Instance()

#endif
