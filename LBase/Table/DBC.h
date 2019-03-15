#ifndef _DBC_H_
#define _DBC_H_

#include <vector>
#ifdef WIN32
	#define		tsnprintf		_snprintf
	#include <unordered_map>
#else
	#include <ext/hash_map>
	#define		tsnprintf		snprintf
#endif

#define VOID				void			//��׼��
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned char		UCHAR;			//��׼�޷���CHAR
typedef char				CHAR;			//��׼CHAR

#ifndef TRUE
	#define TRUE 1
#endif
//��
#ifndef FALSE
	#define FALSE 0
#endif

#define MAX_FILE_PATH 260
using namespace std;

class DBCFile	
{
public:
	//�ļ�ͷ
	struct FILE_HEAD
	{
		UINT		m_Identify;				//��ʾ	0XDDBBCC00
		INT			m_nFieldsNum;			//����
		INT			m_nRecordsNum;			//����
		INT			m_nStringBlockSize;		//�ַ�������С
	};

	//�ֶ���������
	enum FIELD_TYPE
	{
		T_INT		= 0,	//����
		T_FLOAT		= 1,	//������
		T_STRING	= 2,	//�ַ���
	};

	//���ݿ��ʽ����
	typedef vector< FIELD_TYPE >	FILEDS_TYPE;

	//���ݶ�
	union FIELD
	{
		FLOAT		fValue;
		INT			iValue;
		WORD		wValue;
		CHAR		cValue;
		const CHAR*	pString;	// Just for runtime!
		//Construct
		FIELD() {}
		FIELD(INT value) { iValue = value; }
		FIELD(FLOAT value) { fValue = value; }
		FIELD(const CHAR* value) { pString = value; }
	};
	//������
	typedef vector< FIELD >		DATA_BUF;

public:
	//���ı��ļ�������һ�����ݿ�
	BOOL					OpenFromTXT(const CHAR* szFileName);
	//�����ڴ��е��ļ���
	BOOL					OpenFromMemory(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);
	//
	BOOL					GetVarListFromTXT(const CHAR* szFileName,vector<vector<std::string> >& vecList);
protected:
	//��ȡ�ı���ʽ����
	BOOL					OpenFromMemoryImpl_Text(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);
	//��ȡ�����Ƹ�ʽ����
	BOOL					OpenFromMemoryImpl_Binary(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);

	//�����б�
	BOOL					GetFileVarList_Text(vector<vector<std::string> >& vecList,const CHAR* pMemory, const CHAR* pDeadEnd);

public:
	//����������(��һ��Ϊ����)
	virtual const FIELD*	Search_Index_EQU(INT nValue) const;
	//����λ�ò���
	virtual const FIELD*	Search_Posistion(INT nRecordLine, INT nColumNum) const;
	//����ĳ�е���ָ��ֵ�ĵ�һ��
	virtual const FIELD*	Search_First_Column_Equ(INT nColumnNum, const FIELD& value) const;
public:
	//ȡ��ID
	UINT GetID(VOID) const				{ return m_ID; }
	//ȡ������
	INT	GetFieldsNum(VOID) const	    { return m_nFieldsNum; }
	//ȡ�ü�¼������
	INT GetRecordsNum(VOID) const		{ return m_nRecordsNum; }
	//����������
	VOID CreateIndex(INT nColum = 0, const CHAR* szFileName=0);

protected:
#ifdef __SGI_STL_PORT
	typedef std::hash_map< INT, FIELD*>	FIELD_HASHMAP;
#else
	#ifdef WIN32
		typedef unordered_map< INT, FIELD*>	FIELD_HASHMAP;
	#else
		typedef __gnu_cxx::hash_map< INT,FIELD*>    FIELD_HASHMAP;
	#endif
#endif

	//���ݿ��ʽ�ļ���
	UINT			m_ID;
	//���ݿ��ʽ����
	FILEDS_TYPE				m_theType;
	//����
	INT						m_nRecordsNum;
	//����
	INT						m_nFieldsNum;
	//������
	DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
	//�ַ�����
	CHAR*					m_pStringBuf;
	//�ַ�������С
	INT						m_nStringBufSize;
	//������
	FIELD_HASHMAP			m_hashIndex;
	//������
	INT						m_nIndexColum;

public:

	static INT			_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);
	//���ڴ����ַ�����ȡһ���ı�(���ջ��з�)
	static const CHAR*	_GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd);
	//�Ƚ�����ֵ�Ƿ����
	template < FIELD_TYPE T>	
	static bool			_FieldEqu(const FIELD& a, const FIELD& b);

public:
	DBCFile(UINT id);
	virtual ~DBCFile();
};

#endif
