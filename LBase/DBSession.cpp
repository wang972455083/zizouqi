#include <boost/thread/lock_guard.hpp> 
#include "LLog.h"
#include "DBSession.h"

boost::mutex DBSession::m_initLock;

DBSession::DBSession()
	:m_Port(0),m_Mysql(NULL)
{
}

DBSession::~DBSession()
{
	if(m_Mysql)
	{
		mysql_close(m_Mysql);
	}
}

bool DBSession::Init(const Lstring& host, const Lstring& user, const Lstring& pass, const Lstring& dbname, const Lstring&  charset, Lshort port)
{
	m_Host = host;
	m_User = user;
	m_Pass = pass;
	m_DBName = dbname;
	m_Port = port;
	m_CharSet = charset;

	boost::lock_guard<boost::mutex> guard(m_initLock);
	//��ʼ��mysql
	if((m_Mysql = mysql_init(NULL)) == 0)
	{
		LLOG_ERROR("mysql_init() error");
		return false;
	}

	//����mysql�Զ�����
	char value = 1;
	if(mysql_options(m_Mysql, MYSQL_OPT_RECONNECT, (char*)&value) != 0)
	{
		LLOG_ERROR("mysql_options error %s",mysql_error(m_Mysql));
		return false;
	}

	//�������ݿ�
	if(mysql_real_connect(m_Mysql,m_Host.c_str(),m_User.c_str(),m_Pass.c_str(),dbname.c_str(),m_Port,NULL,0) == 0)
	{
		LLOG_ERROR("mysql_real_connect error %s",mysql_error(m_Mysql));
		return false;
	}

	//����mysql�ַ���
	if(mysql_set_character_set(m_Mysql, m_CharSet.c_str()) != 0)
	{
		LLOG_ERROR("mysql_set_character_set error %s",mysql_error(m_Mysql));
		return false;
	}

	LLOG_ERROR("DBSession::Init success");

	return true;
}

MYSQL* DBSession::GetMysql()
{
	try
	{
		mysql_ping(m_Mysql);
	}
	catch(...)
	{
		return NULL;
	}

	return m_Mysql;
}

bool DBSession::Excute(const std::string & sql)
{
	boost::recursive_mutex::scoped_lock l(m_Mutex);
	LLOG_DEBUG("DBSession::Excute sql = %s", sql.c_str());
	bool succeed = false;
	if (m_Mysql != NULL)
	{
		if (mysql_real_query(m_Mysql, sql.c_str(), sql.size()) == 0)
			succeed = true;
		else
			LLOG_ERROR("DbServer::Excute error = %s,sql = %s ", mysql_error(m_Mysql), sql.c_str());
	}
	else
		LLOG_ERROR("DBSession::Excute MYSQL is NULL when sql %s is to execute", sql.c_str());
	return succeed;
}