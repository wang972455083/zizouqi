#include "LVipLog.h"


LVipLogItem::LVipLogItem() : m_id("")
{
	m_time = 0;
	m_flag = 0;	//
	m_state = 0;
	m_deskId = 0;		//��ע
	m_maxCircle = 0;//��Ȧ��
	m_curCircle = 0;//��ǰȦ��
	memset(m_posUserId, 0, sizeof(m_posUserId));
	m_curZhuangPos = 0;
	m_score[0] = 0;
	m_score[1] = 0;
	m_score[2] = 0;
	m_score[3] = 0;
	m_reset = 0;
	m_playtype.clear();
	m_appId = 0;
}


LVipLogItem::~LVipLogItem()
{
	for(Lsize i = 0 ; i < m_log.size(); ++i)
	{
		delete m_log[i];
	}
}

Lstring LVipLogItem::ToString()
{
	std::stringstream ss;

	for (Lsize i = 0; i < m_log.size(); ++i)
	{
		ss << m_log[i]->m_gold[0] << "," << m_log[i]->m_gold[1] << "," << m_log[i]->m_gold[2] << "," << m_log[i]->m_gold[3] << ",";
		ss << m_log[i]->m_win[0] << "," << m_log[i]->m_win[1] << "," << m_log[i]->m_win[2] << "," << m_log[i]->m_win[3] << ",";
		ss << m_log[i]->m_mgang[0] << "," << m_log[i]->m_mgang[1] << "," << m_log[i]->m_mgang[2] << "," << m_log[i]->m_mgang[3] << ",";
		ss << m_log[i]->m_angang[0] << "," << m_log[i]->m_angang[1] << "," << m_log[i]->m_angang[2] << "," << m_log[i]->m_angang[3] << ",";
		ss << m_log[i]->m_bomb[0] << "," << m_log[i]->m_bomb[1] << "," << m_log[i]->m_bomb[2] << "," << m_log[i]->m_bomb[3] << ",";
		ss << m_log[i]->m_zhuangPos << "," << m_log[i]->m_time << "," << m_log[i]->m_videoId <<";";
	}

	return ss.str();
}

void LVipLogItem::FromString(Lstring str)
{
	std::vector<Lstring> str1, str2;

	L_ParseString(str, str2, ";");

	for(Lint j = 0 ; j < str2.size(); ++j)
	{
		str1.clear();
		L_ParseString(str2[j], str1, ",");
		if (str1.size() >= 19)
		{
			VipDeskLog* log = new VipDeskLog();
			Lint i = 0;
			for (; i < DESK_USER_COUNT; ++i)
			{
				log->m_gold[i] = atoi(str1[i].c_str());
				log->m_win[i] = atoi(str1[i+4].c_str());
				log->m_mgang[i] = atoi(str1[i+8].c_str());
				log->m_angang[i] = atoi(str1[i+12].c_str());
				log->m_bomb[i] = atoi(str1[i+16].c_str());
			}

			log->m_zhuangPos = atoi(str1[20].c_str());
			log->m_time = atoi(str1[21].c_str());
			log->m_videoId = str1[22].c_str();
			m_log.push_back(log);
		}
	}
}

Lstring LVipLogItem::PlayTypeToStrint()
{
	std::stringstream ss;

	for (Lsize i = 0; i < m_playtype.size(); ++i)
	{
		ss << m_playtype[i] << ";";
	}

	return ss.str();
}

Lstring LVipLogItem::PlayTypeToString4RLog()
{
	std::stringstream ss; 

	if (!m_playtype.empty())
	{   
		ss << m_playtype.front();
	}   

	for (Lsize i = 1; i < m_playtype.size(); ++i)
	{   
		ss << ";" << m_playtype[i];
	}   
	return ss.str();
}

void LVipLogItem::PlayTypeFromString(Lstring str)
{
	std::vector<Lstring> strSplid;

	L_ParseString(str, strSplid, ";");
	if (strSplid.size() > 0)
	{
		for(Lint j = 0 ; j < strSplid.size(); ++j)
		{
			m_playtype.push_back( atoi(strSplid[j].c_str()));
		}
	}
}