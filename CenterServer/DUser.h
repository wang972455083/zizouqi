#ifndef _D_USER_H_
#define _D_USER_H_


#include "LUser.h"

class DUser
{
public:
	DUser();
	~DUser();

	bool	GetCopy();
	void	SetCopy(bool copy);
	
	
public:
	LUser m_usert;
	bool  m_hasCopy;		//��logic�������Ƿ��б���
	Lint  m_lastReqTime;	//���һ�������¼��ʱ��
	Lint  m_logicID;		//��ҵ�ǰ���ڷ�����Index
	
};


#endif