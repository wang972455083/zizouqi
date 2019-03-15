#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "LSingleton.h"
#include "DUser.h"

typedef std::shared_ptr<DUser> UserPtr;

class UserManager:public LSingleton<UserManager>
{
public:
	
	virtual	bool		Init();
	virtual	bool		Final();

	UserPtr				GetUserById(Lint user_id);

	void				DelUserById(Lint user_id);


	UserPtr				CreateUser(int user_id,Lstring name);

	void				SaveUser(UserPtr user);

	void				SaveUserLastLogin(UserPtr user);

	void				LoadUserInfo();

	void				LoadUserIdInfo(Lint serverID);
	
	void				AddUser(UserPtr user);
	static	Lint		GetRandInsertIDFromDB();
	void				AddChessBlockForNewUser(int user_id);

	
private:
	std::map<Lint, UserPtr >	m_userMap;
	
};

#define gUserManager UserManager::Instance()

#endif