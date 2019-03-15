#include "UserManager.h"
#include "DbServer.h"
#include "LLog.h"
#include "LMsgS2S.h"
#include "Work.h"
#include "ChecssBlockManager.h"

bool UserManager::Init()
{
	LoadUserInfo();
	
	return true;
}

void UserManager::LoadUserIdInfo(Lint serverID)
{
	UserIdInfo info;
	std::map<Lint, UserPtr>::iterator it = m_userMap.begin();
	for(; it != m_userMap.end(); ++it)
	{	
		/*LMsgCe2LUserIdInfo send;
		info.m_id = it->second->m_usert.m_id;
		info.m_nike = it->second->m_usert.m_nike;
		info.m_headImageUrl = it->second->m_usert.m_headImageUrl;
		info.m_openId = it->second->m_usert.m_openid;
		info.m_sex = it->second->m_usert.m_sex;
		info.m_blockEndTime = it->second->m_usert.m_blockEndTime;
		send.m_info.push_back(info);*/
		//gWork.SendMsgToLogic(send, serverID);
	}
}

void UserManager::LoadUserInfo()
{
	//�������е����
	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::LoadUserIdInfo mysql null");
		return;
	}

	Lint limitfrom = 0;
	Lint count = 500000;

	while(true)
	{
		std::stringstream ss;
		ss << "select Id,UserId,Region,Name,Sex,HeadImageUrl,Money FROM user ORDER BY Id DESC LIMIT ";
		ss << limitfrom << "," << count;

		if (mysql_real_query(m, ss.str().c_str(), ss.str().size()))
		{
			LLOG_ERROR("UserManager::LoadUserIdInfo sql error %s", mysql_error(m));
			break;
		}

		MYSQL_RES* res = mysql_store_result(m);
		MYSQL_ROW row = mysql_fetch_row(res);
		if(!row)
		{
			break;
		}

		while(row)
		{
			++limitfrom;
			UserPtr user = std::make_shared<DUser>();
			user->m_usert.m_id = atoi(*row++);
			user->m_usert.m_user_id = atoi(*row++);
			user->m_usert.m_name = *row++;
			user->m_usert.m_sex = atoi(*row++);
			user->m_usert.m_headImageUrl = *row++;
			user->m_usert.m_money = atoi(*row++);

			AddUser(user);
			row = mysql_fetch_row(res);
		}
		mysql_free_result(res);
	}

	LLOG_ERROR("UserManager::LoadUserIdInfo user count %d", limitfrom);
}



bool UserManager::Final()
{
	return true;
}

void UserManager::SaveUser(UserPtr user)
{
	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::SaveUser mysql null");
		return;
	}

	//��ѯ���ݿ�
	std::stringstream ss;
	ss << "UPDATE user SET ";
	ss << "Name='"<<  user->m_usert.m_name <<"',";
	ss << "Sex='"<<  user->m_usert.m_sex <<"',";
	ss << "Scores='" << user->m_usert.m_scores << "',";
	ss << "HeadImageUrl='"<<  user->m_usert.m_headImageUrl<<"',";
	ss << "Money='"<<  user->m_usert.m_money<<"' WHERE UserId='";
	ss << user->m_usert.m_user_id << "'";

	LLOG_DEBUG("UserManager::SaveUser sql =%s", ss.str().c_str());

	Lstring* sql = new Lstring(ss.str());
	gDbServer.Push(sql);
}


void UserManager::SaveUserLastLogin(UserPtr user)
{
	//DbTool tool;
	/*MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::SaveUser mysql null");
		return;
	}

	//��ѯ���ݿ�
	std::stringstream ss;
	ss << "UPDATE user SET ";
	ss << "Nike='"<<  user->m_usert.m_nike <<"',";
	ss << "Sex='"<<  user->m_usert.m_sex <<"',";
	ss << "Provice='"<<  user->m_usert.m_provice <<"',";
	ss << "City='"<<  user->m_usert.m_city <<"',";
	ss << "Country='"<<  user->m_usert.m_country<<"',";
	ss << "HeadImageUrl='"<<  user->m_usert.m_headImageUrl<<"',";
	ss << "NumsCard1='"<<  user->m_usert.m_numOfCard1s<<"',";
	ss << "NumsCard2='"<<  user->m_usert.m_numOfCard2s<<"',";
	ss << "NumsCard3='"<<  user->m_usert.m_numOfCard3s<<"',";
	ss << "LastLoginTime='"<<  user->m_usert.m_lastLoginTime<<"',";
	ss << "Gm='" << user->m_usert.m_gm << "',";
	ss << "New='"<<  user->m_usert.m_new<< "',";
	ss << "TotalCardNum='" << user->m_usert.m_totalbuynum << "',";
	ss << "TotalPlayNum='" << user->m_usert.m_totalplaynum << "',";
	ss << "location='" << user->m_usert.m_location << "' ";
	ss << " WHERE UnionId='" << user->m_usert.m_unioid << "'";

	LLOG_DEBUG("UserManager::SaveUser sql =%s", ss.str().c_str());
	
	Lstring* sql = new Lstring(ss.str());
	gDbServer.Push(sql);*/
}

UserPtr UserManager::GetUserById(Lint user_id)
{
	if (m_userMap.count(user_id))
	{
		LLOG_DEBUG("UserManager::GetUserByOpenId %d", user_id);
		return m_userMap[user_id];
	}

	LLOG_DEBUG("UserManager::GetUserByOpenId null %d", user_id);
	return nullptr;
}



void UserManager::DelUserById(Lint user_id)
{
	if (m_userMap.count(user_id))
	{
		m_userMap.erase(m_userMap.find(user_id));
	}
}

UserPtr UserManager::CreateUser(int user_id,Lstring name)
{
	UserPtr user = std::make_shared<DUser>();
	if (user == nullptr)
		return nullptr;

	user->m_usert.m_user_id = user_id;
	user->m_usert.m_sex = 1;
	user->m_usert.m_money = 100;
	user->m_usert.m_name = name;

	//DbTool tool;
	MYSQL* m = gWork.GetDbSession().GetMysql();

	if (m == NULL)
	{
		LLOG_ERROR("UserManager::CreateUser mysql null");
		return 0;
	}

	//ָ�������id	
	/*Lint nInsertID = GetRandInsertIDFromDB();
	if (nInsertID == 0)
	{
		LLOG_ERROR("UserManager::RandInsertID==0");
		return 0;
	}*/

	std::stringstream ss;
	ss << "INSERT INTO user (UserId,Name,Sex,HeadImageUrl,Scores,Money) VALUES (";
	ss << "'" << user->m_usert.m_user_id << "',";
	ss << "'" << user->m_usert.m_name << "',";
	ss << "'" << user->m_usert.m_sex << "',";
	ss << "'" << user->m_usert.m_headImageUrl << "',";
	ss << "'" << user->m_usert.m_scores << "',";
	ss << "'" << user->m_usert.m_money << "'";
	ss << ")";

	if (mysql_real_query(m, ss.str().c_str(), ss.str().size()))
	{
		LLOG_ERROR("UserManager::CreateUser sql error %s", ss.str().c_str());
		return 0;
	}

	user->m_usert.m_id = (Lint)mysql_insert_id(m);

	AddUser(user);

	AddChessBlockForNewUser(user_id);

	return user;
}

void UserManager::AddUser(UserPtr user)
{
	if (!m_userMap.count(user->m_usert.m_user_id))
	{
		m_userMap[user->m_usert.m_user_id] = user;
	}
}

Lint UserManager::GetRandInsertIDFromDB()
{
	MYSQL* m = gWork.GetDbSession().GetMysql();
	if (m == NULL)
	{
		LLOG_ERROR("UserManager::GetRandInsertIDFromDB mysql null");
		return 0;
	}
	////////////////////////////////////////////////////////
	Lint nid = 0;
	Lint nInsertID = 0;
	static Lstring  SQLSelect = "select Id, UserId from user_id_table order by Id  limit 1";
	if (mysql_real_query(m, SQLSelect.c_str(), SQLSelect.size()))
	{
		LLOG_ERROR("GetRandInsertIDFromDB1  error: %s  Sql: %s", mysql_error(m), SQLSelect.c_str());
		return 0;
	}

	MYSQL_RES* res = mysql_store_result(m);
	if (!res)
	{
		LLOG_ERROR("GetRandInsertIDFromDB2  error: %s ", mysql_error(m));
		return 0;
	}

	MYSQL_ROW row = mysql_fetch_row(res);
	if (!row)
	{
		mysql_free_result(res);
		LLOG_ERROR("GetRandInsertIDFromDB3  user_id_table Empty!");
		return 0;
	}
	nid = atoi(*row++);
	nInsertID = atoi(*row);
	mysql_free_result(res);
	std::stringstream ss;
	ss << "delete from user_id_table where Id = " << nid;
	if (mysql_real_query(m, ss.str().c_str(), ss.str().size()))
	{
		LLOG_ERROR("GetRandInsertIDFromDB4 sql error: %s  Sql: %s", mysql_error(m), ss.str().c_str());
		return 0;
	}
	return nInsertID;
}

void UserManager::AddChessBlockForNewUser(int user_id)
{
	
	for (int i = CHESS_JIANG; i < CHESS_MAX; ++i)
	{
		ChessBoardTbl* pTbl = ChessBoardCfg::Instance().GetChessBoard(i);
		if (pTbl == nullptr)
			continue;

		for (int j = 0; j < pTbl->m_max_cnt; ++j)
		{
			gChessBlockManager.CreateBlock(user_id, pTbl->m_type, pTbl->m_init_hp, pTbl->m_init_max_hp, pTbl->m_init_attack, pTbl->m_init_max_attack,true,true);
		}
	}

}
