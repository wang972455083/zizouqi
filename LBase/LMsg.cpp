#include "LMsg.h"
#include "LLog.h"
#include "LMsgS2S.h"


LMsg::LMsg(int id)
:m_msgId(id), m_isMsgPack(false)
{

}

LMsg::~LMsg()
{

}

LBuffPtr LMsg::GetSendBuff()
{
	if(!m_send)
	{
		m_send.reset(new LBuff());
		if(preAllocSendMemorySize() > BUFF_SIZE)
		{
			m_send->Alloc(preAllocSendMemorySize());
		}

		if (m_isMsgPack)
		{
			msgpack::sbuffer buffer;
			msgpack::packer<msgpack::sbuffer> pac(&buffer);
			Write(pac);

			MsgHeader header = { (unsigned short)(buffer.size()), 0, 0, m_msgId };
			m_send->Write((char*)&header, sizeof(header));
			m_send->Write(buffer.data(), buffer.size());
		}
		else
		{
			LBuff tmpBuff;
			WriteMsgId(tmpBuff);
			Write(tmpBuff);

			MsgHeader header = { (unsigned short)(tmpBuff.Size()), 0, 0, m_msgId };
			m_send->Write((char*)&header, sizeof(header));
			m_send->Write(tmpBuff.Data(), tmpBuff.Size());
		}
	}
	return m_send;
}

void LMsg::WriteMsgId(LBuff& buff)
{
	buff.Write(m_msgId);
}

void LMsg::ReadMsgId(LBuff& buff)
{
	buff.Read(m_msgId);
}

bool LMsg::Read(LBuff& buff)
{
	return true;
}

bool LMsg::Read(msgpack::object& pack)
{
	return true;
}

bool LMsg::Write(LBuff& buff)
{
	return true;
}

bool LMsg::Write(msgpack::packer<msgpack::sbuffer>& pack)
{
	return true;
}

LMsg* LMsg::Clone()
{
	return NULL;
}

Lint LMsg::preAllocSendMemorySize()
{
	return BUFF_SIZE;
}

//////////////////////////////////////////////////////////////////////////
bool LMsgFactory::Init()
{
	Final();

	RegestMsg(new LMsgIn());
	RegestMsg(new LMsgKick());
	RegestMsg(new LMsgHttp());

	RegestMsg(new LMsgC2SMsg());
	RegestMsg(new LMsgS2CMsg());

	RegestMsg(new LMsgC2SHeart());
	RegestMsg(new LMsgS2CHeart());

	RegestMsg(new LMsgC2SLogin());
	RegestMsg(new LMsgS2CLogin());

	//RegestMsg(new LMsgC2SDeskOpt());
	//RegestMsg(new LMsgS2CDeskOpt());

	RegestMsg(new LMsgC2SNoticeLogin());

	RegestMsg(new LMsgG2ServerLogin());
	RegestMsg(new LMsgG2LUserMsg());
	RegestMsg(new LMsgL2GUserMsg());
	
	

	RegestMsg(new LMsgG2LUserOutMsg());
	RegestMsg(new LMsgL2GUserOutMsg());

	

			
	
	RegestMsg(new LMsgL2LMGLogin());
	RegestMsg(new LMsgLMG2GateLogicInfo());

	RegestMsg(new LMsgS2SKeepAlive());
	RegestMsg(new LMsgS2SKeepAliveAck());

	RegestMsg(new LMsgGame2LMGLogin());
	RegestMsg(new LMsgLMG2GateGameServerInfo());
	RegestMsg(new LMsgG2GameUserMsg());
	RegestMsg(new LMsgGame2GUserMsg());
	RegestMsg(new LMsgG2LMUserMsg());
	RegestMsg(new LMsgLM2GUserMsg());

	//RegestMsg(new LMsgLM2LDeskOpt());
	//RegestMsg(new LMsgL2LMDeskOpt());

	RegestMsg(new LMsgC2SLMLogin());
	RegestMsg(new LMsgS2CLMLogin());

//	RegestMsg(new LMsgC2STest());
	RegestMsg(new LMsgS2CTest());

	RegestMsg(new LMsgC2SQuickRoomOpt());
	RegestMsg(new LMsgS2CQuickRoomOpt());

	RegestMsg(new LMsgS2CQuickRoomResult());

	RegestMsg(new LMsgLM2LMQuckCreateRoom());

	RegestMsg(new LMsgLM2LQuickCreateRoomOpt());
	RegestMsg(new LMsgL2LMQuickCreateRoomOpt());

	RegestMsg(new LMsgC2SCreateDeskAsk());
	RegestMsg(new LMsgS2CNoticeCreateDeskAsk());
	RegestMsg(new LMsgC2SCreateDeskRespon());
	RegestMsg(new LMsgS2CNoticeCreateDeskResult());



	RegestMsg(new LMsgLM2GUserStatusModify());

	RegestMsg(new LMsgC2SSelectCard());
	RegestMsg(new LMsgS2CSelectCard());
	RegestMsg(new LMsgS2CTurnDeskPos());
	RegestMsg(new LMsgS2CDeskOverResult());
	RegestMsg(new LMsgS2CRoomUserModify());
	RegestMsg(new LMsgC2SQuitRoom());
	RegestMsg(new LMsgS2CQuitRoom());
	
	RegestMsg(new LMsgS2CRoomInfoWhenRelogin());

	//*********************************
	RegestMsg(new LMsgC2SCreateXiangQiRoom());
	RegestMsg(new LMsgS2CCreateXiangQiRoom());

	RegestMsg(new LMsgS2CRoomInfoModify());
	RegestMsg(new LMsgC2SAddIntoRoom());
	RegestMsg(new LMsgS2CAddIntoRoom());
	RegestMsg(new LMsgC2SReadyOpt());
	RegestMsg(new LMsgS2CReadyOpt());
	RegestMsg(new LMsgS2CSendGameInfo());
	RegestMsg(new LMsgS2CQuickStartGame());
	RegestMsg(new LMsgC2SAddBlockToChess());
	RegestMsg(new LMsgS2CAddBlockToChess());
	RegestMsg(new LMsgS2CSendTurn());
	RegestMsg(new LMsgC2SChessBlockMove());
	RegestMsg(new LMsgS2CChessBlockMove());

	RegestMsg(new LMsgC2SChessBlockAttack());
	RegestMsg(new LMsgS2CChessBlockAttack());
	RegestMsg(new LMsgS2CChessCoinModify());
	RegestMsg(new LMsgC2SChessBlockUpgrade());
	RegestMsg(new LMsgS2CChessBlockUpgrade());
	RegestMsg(new LMsgS2CChessGameOver());

	RegestMsg(new LMsgC2SChessBlockZhuiSha());
	RegestMsg(new LMsgS2CChessBlockZhuiSha());

	RegestMsg(new LMsgS2CChessRewardUse());


	RegestMsg(new LMsgL2LMQuitRoom());
	RegestMsg(new LMsgG2SUserLogOut());
	RegestMsg(new LMsgLM2LUserLogOut());
	RegestMsg(new LMsgLM2LUserLogin());
	RegestMsg(new LMsgL2LMRecyleRoom());

	//***********
	RegestMsg(new LMsgS2SCreateXiangQiRoom());
	RegestMsg(new LMsgL2LMCreateRoomResult());
	RegestMsg(new LMsgS2SAddIntoXiangQiRoom());
	RegestMsg(new LMsgL2LMAddIntoRoomResult());
	RegestMsg(new LMsgGame2LMQuickStartOpt());
	return true;
}

bool LMsgFactory::Final()
{
	std::map<int,LMsg*>::iterator it = m_msgMap.begin();
	for(;it != m_msgMap.end();++it)
	{
		delete it->second;
	}
	return true;
}

void LMsgFactory::RegestMsg(LMsg* pMsg)
{
	std::map<int,LMsg*>::iterator it = m_msgMap.find(pMsg->m_msgId);
	if(it != m_msgMap.end())
	{
		LLOG_INFO("LMsgFactory::RegestMsg msg exiset msgId:%d",pMsg->m_msgId);
	}
	else
	{
		m_msgMap[pMsg->m_msgId] = pMsg;
	}
}

LMsg* LMsgFactory::CreateMsg(int msgId)
{
	std::map<int,LMsg*>::iterator it = m_msgMap.find(msgId);
	if(it == m_msgMap.end())
	{
		return NULL;
	}
	else
	{
		return it->second->Clone();
	}
}
