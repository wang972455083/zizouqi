#pragma once

//#include "BaseMove.h"
#include "LBase.h"
#include "ChessGame.h"
#include "vector"
#include "Point.h"

//��������������
class OperatorEvent
{
public:
	virtual void OnMoveEvent() = 0;
	virtual void OnAttackEvent() = 0;
	virtual void OnAttackDeathEvent() = 0;
	virtual void OnDamageEvent() = 0;
	virtual void OnDamageDeathEvent() = 0;

	virtual ~OperatorEvent()
	{

	}
};

class ChessBlock : public OperatorEvent
{
public:
	ChessBlock(int type);
	virtual ~ChessBlock();


	enum
	{
		STATUS_NORMAL,			//����״̬
		STARUS_WEAK,			//����״̬
	};

public:
	virtual void OnMoveEvent() ;
	virtual void OnAttackEvent() ;
	virtual void OnAttackDeathEvent();
	virtual void OnDamageEvent() ;
	virtual void OnDamageDeathEvent() ;


public:

	virtual bool	IsCanAttack(std::shared_ptr<ChessBlock> pBlock);
	
	virtual bool	IsCanMove(const Point& pos) = 0;
	
	
public:
	void			MoveToPos(const Point& pos);
	bool			IsInBoundary(Point point);
	bool			IsRed() { return m_Color == int(CHESS_RED); }
	void			GetDamage(int damage);
	bool			IsAttackPos();  //����Ƿ��ǹ�����
	void			FillBlockMsg(GameBlock& msg_block);
	bool			IsInSelfCamp();	//�ж��Ƿ����Լ���Ӫ��
	bool			IsCanProtectBlock(std::shared_ptr<ChessBlock> pBlock);
	void			AddStep();

	

	PropertyBuilderByName(int, Id, protected);//int				m_Id;		
	//PropertyBuilderByName(int, TableId, protected);//int				��Ӧ���ID;
	PropertyBuilderByName(int, Color, protected);//CHESS_COLOR		m_Color;	//�� ��
	PropertyBuilderByName(int, Type, protected);//TYPE_CHESS		m_Type;		//����
	PropertyBuilderByName(int, Hp, protected);//int				m_Hp;		//Ѫ��
	PropertyBuilderByName(int, MaxHp, protected);//int	m_Hp;		//Ѫ������
	PropertyBuilderByName(int, Attack, protected);//intm_Attack;	//������
	PropertyBuilderByName(Point, Pos, protected);	// λ��

				
	PropertyBuilderByName(int, MaxAttack, protected);//intm_Attack;	//����������

	PropertyBuilderByName(int, State, protected);//
	PropertyBuilderByName(int, ExistStep, protected);	//���ڵĲ���

	PropertyBuilderByName(ChessGame*, Game, protected);
};

class ChessJiang : public ChessBlock
{
public:
	ChessJiang() :ChessBlock(CHESS_JIANG) {}
	
	bool			IsCanMove(const Point& pos);
	
};

class ChessShi : public ChessBlock
{

public:
	ChessShi() :ChessBlock(CHESS_SHI) {}

	bool IsCanMove(const Point& pos);
};

class ChessXiang : public ChessBlock
{
public:
	ChessXiang() :ChessBlock(CHESS_XIANG) {}
	bool IsCanMove(const Point& pos);
};

class ChessMa : public ChessBlock
{
public:
	ChessMa() :ChessBlock(CHESS_MA) {}
	bool IsCanMove(const Point& pos);
};


class ChessChe : public ChessBlock
{
public:
	ChessChe() :ChessBlock(CHESS_CHE) {}
	bool IsCanMove(const Point& pos);
};

class ChessPao : public ChessBlock
{
public:
	ChessPao() :ChessBlock(CHESS_PAO) {}
	bool IsCanMove(const Point& pos);
	bool IsCanAttack(std::shared_ptr<ChessBlock> pBlock);


};

class ChessBing : public ChessBlock
{
public:
	ChessBing() :ChessBlock(CHESS_BING) {}
	bool IsCanMove(const Point& pos);
};

class ChessShiTou : public ChessBlock
{
public:
	ChessShiTou() :ChessBlock(CHESS_SHI_TOU) {};
	bool IsCanMove(const Point& pos)
	{
		return false;
	}

	bool IsCanAttack(std::shared_ptr<ChessBlock> pBlock)
	{
		return false;
	}
};

class ChessZhaDan : public ChessBlock
{
public:
	ChessZhaDan() :ChessBlock(CHESS_ZHA_DAN) {};
	bool IsCanMove(const Point& pos)
	{
		return false;
	}

	bool IsCanAttack(std::shared_ptr<ChessBlock> pBlock)
	{
		return false;
	}
};
