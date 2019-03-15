
#include "ChessManager.h"


void ChessManager::Init()
{
	InitChessPos();
	InitMoveBoundary();
}

void ChessManager::InitChessPos()
{
	std::vector<Point> vecTmp;

	//��ɫ����
	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(4, 0));
	m_mapRedPoints[CHESS_JIANG] = vecTmp;

	//ʿ��λ��
	vecTmp.clear();
	vecTmp.push_back(Point(3, 0));
	vecTmp.push_back(Point(5, 0));
	m_mapRedPoints[CHESS_SHI] = vecTmp;

	/*//���λ��
	vecTmp.clear();
	vecTmp.push_back(Point(2, 0));
	vecTmp.push_back(Point(6, 0));
	m_mapRedPoints[CHESS_XIANG] = vecTmp;

	//���λ��
	vecTmp.clear();
	vecTmp.push_back(Point(1, 0));
	vecTmp.push_back(Point(7, 0));
	m_mapRedPoints[CHESS_MA] = vecTmp;

	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(0, 0));
	vecTmp.push_back(Point(8, 0));
	m_mapRedPoints[CHESS_MA] = vecTmp;

	//�ڵ�λ��
	vecTmp.clear();
	vecTmp.push_back(Point(1, 2));
	vecTmp.push_back(Point(7, 2));
	m_mapRedPoints[CHESS_PAO] = vecTmp;

	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(0, 3));
	vecTmp.push_back(Point(2, 3));
	vecTmp.push_back(Point(4, 3));
	vecTmp.push_back(Point(6, 3));
	vecTmp.push_back(Point(8, 3));
	m_mapRedPoints[CHESS_MA] = vecTmp;*/

	//��ɫ��
	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(4, 9));
	m_mapBlackPoints[CHESS_JIANG] = vecTmp;

	//ʿ��λ��
	vecTmp.clear();
	vecTmp.push_back(Point(3, 9));
	vecTmp.push_back(Point(5, 9));
	m_mapBlackPoints[CHESS_SHI] = vecTmp;

	/*//���λ��
	vecTmp.clear();
	vecTmp.push_back(Point(2, 9));
	vecTmp.push_back(Point(6, 9));
	m_mapBlackPoints[CHESS_XIANG] = vecTmp;

	//���λ��
	vecTmp.clear();
	vecTmp.push_back(Point(1, 9));
	vecTmp.push_back(Point(7, 9));
	m_mapBlackPoints[CHESS_MA] = vecTmp;

	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(0, 9));
	vecTmp.push_back(Point(8, 9));
	m_mapBlackPoints[CHESS_CHE] = vecTmp;

	//�ڵ�λ��
	vecTmp.clear();
	vecTmp.push_back(Point(1, 7));
	vecTmp.push_back(Point(7, 7));
	m_mapBlackPoints[CHESS_PAO] = vecTmp;

	//����λ��
	vecTmp.clear();
	vecTmp.push_back(Point(0, 6));
	vecTmp.push_back(Point(2, 6));
	vecTmp.push_back(Point(4, 6));
	vecTmp.push_back(Point(8, 6));
	m_mapBlackPoints[CHESS_BING] = vecTmp;*/
}

void ChessManager::InitMoveBoundary()
{
	//��ɫ��
	m_mapRedMoveBoundary[CHESS_JIANG] = MoveBoundary(3, 5, 0, 2);
	m_mapRedMoveBoundary[CHESS_SHI] = MoveBoundary(3, 5, 0, 2);
	m_mapRedMoveBoundary[CHESS_XIANG] = MoveBoundary(0, 8, 0, 4);
	m_mapRedMoveBoundary[CHESS_MA] = MoveBoundary(0, 8, 0, 9);
	m_mapRedMoveBoundary[CHESS_CHE] = MoveBoundary(0, 8, 0, 9);
	m_mapRedMoveBoundary[CHESS_PAO] = MoveBoundary(0, 8, 0, 9);
	m_mapRedMoveBoundary[CHESS_BING] = MoveBoundary(0, 8, 0, 9);


	//��ɫ��
	m_mapBlackMoveBoundary[CHESS_JIANG] = MoveBoundary(3, 5, 7, 9);
	m_mapBlackMoveBoundary[CHESS_SHI] = MoveBoundary(3, 5, 7, 9);
	m_mapBlackMoveBoundary[CHESS_XIANG] = MoveBoundary(0, 8, 5, 9);
	m_mapBlackMoveBoundary[CHESS_MA] = MoveBoundary(0, 8, 0, 9);
	m_mapBlackMoveBoundary[CHESS_CHE] = MoveBoundary(0, 8, 0, 9);
	m_mapBlackMoveBoundary[CHESS_PAO] = MoveBoundary(0, 8, 0, 9);
	m_mapBlackMoveBoundary[CHESS_BING] = MoveBoundary(0, 8, 0, 9);

}


std::vector<Point>	ChessManager::GetPoints(int color, TYPE_CHESS type)
{
	std::vector<Point> result;

	if (color == CHESS_RED)
	{
		std::map< TYPE_CHESS, std::vector<Point> >::iterator iter = m_mapRedPoints.find(type);
		if (iter != m_mapRedPoints.end())
		{
			result = iter->second;
		}
	}
	else
	{
		std::map< TYPE_CHESS, std::vector<Point> >::iterator iter = m_mapBlackPoints.find(type);
		if (iter != m_mapBlackPoints.end())
		{
			result = iter->second;
		}
	}

	return result;
}

MoveBoundary ChessManager::GetChessBlockBoundary(int color, TYPE_CHESS type, Point pt)
{
	MoveBoundary boundary;

	if (color == CHESS_RED)
	{
		std::map<TYPE_CHESS, MoveBoundary>::iterator iter = m_mapRedMoveBoundary.find(type);
		if (iter != m_mapRedMoveBoundary.end())
		{
			boundary = iter->second;
		}
	}
	else
	{
		std::map<TYPE_CHESS, MoveBoundary>::iterator iter = m_mapBlackMoveBoundary.find(type);
		if (iter != m_mapBlackMoveBoundary.end())
		{
			boundary = iter->second;
		}
	}

	return boundary;
}