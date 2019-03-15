#include "LTool.h"
#include "LTime.h"

#if _LINUX
#include <iconv.h>	
#endif
Llong L_Atoll(const char* s)
{
	if(s == NULL)
	{
		return 0;
	}

	int i;
	Llong sign,n;
	for(i = 0; isspace(s[i]); i++); /* skip begin white space */

	sign = (s[i] == '-') ? -1 : 1;

	if(s[i] == '+' || s[i] == '-') /* skip sign */
		i++;

	for(n = 0; isdigit(s[i]); i++) /* check number */
		n = 10 * n + (s[i] - '0');

	return sign * n;
}

//BKDR Hash
unsigned int caculateHashKey(const char* pchValue, int iDataLen)
{
	if (pchValue == NULL)
	{
		return 0;
	}

	unsigned int uSeed = 131;// 31 131 1313 13131 131313 etc..  
	unsigned int uHash = 0;

	while (*pchValue && iDataLen > 0)
	{
		uHash = uHash*uSeed + (*pchValue++);
		iDataLen--;
	}

	return uHash;
}

void updateRandomFactor()
{
	static Lstring strSeed = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*(){}[];,.?|";
	Lstring strKey;
	for (int i = 0; i < 128; i++)
	{
		int iRand = L_Rand(0, strSeed.length() - 1);
		strKey += strSeed.at(iRand);
	}

	unsigned int iHash = caculateHashKey(strKey.c_str(), strKey.length());
	srand(iHash);
}

int L_Rand(int begin,int end)
{
	static bool init = false;
	if(!init)
	{
		//随机种子
		srand(LTimeTool::GetUTCStampInSecond());
		init = true;
	}

	if(begin > end)
	{
		int tmp = begin;
		begin = end;
		end = tmp;
	}

	int seed = (int)((double)rand() / RAND_MAX * 1000000000); 

	return seed%(end-begin+1) + begin;
}


void L_ParseString(Lstring src, std::vector<Lstring>& des,Lstring par)
{
	des.clear();
	Lstring::size_type pos = src.find(par);
	while(pos != Lstring::npos)
	{
		Lstring sub = src.substr(0,pos);
		src.erase(0,pos+1);
		des.push_back(sub);
		pos = src.find(par);
	}
	if(src.size() != 0)
	{
		des.push_back(src);
	}
}

/*
*	解析1,2;3,4;这种类型字符串
*/
void L_ParseString(Lstring src, std::map<Lstring,Lstring>& des,Lstring pa1,Lstring pa2)
{
	des.clear();
	Lstring::size_type pos = src.find(pa1);
	while(pos != Lstring::npos)
	{
		Lstring sub = src.substr(0,pos);
		src.erase(0,pos+1);
		pos = src.find(pa1);

		Lstring::size_type pos1 = sub.find(pa2); 
		if(pos1 != Lstring::npos)
		{
			Lstring suba = sub.substr(0,pos1);
			Lstring subb = sub.erase(0,pos1+1);
			des[suba] = subb;
		}

	}
	if(src.size() != 0)
	{
		Lstring::size_type pos1 = src.find(pa2); 
		if(pos1 != Lstring::npos)
		{
			Lstring suba = src.substr(0,pos1);
			Lstring subb = src.erase(0,pos1+1);
			des[suba] = subb;
		};
	}
}

#if _LINUX
int code_convert(const char *from_charset,const char *to_charset,const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
        iconv_t cd;
        int rc;
        char **pin = (char**)&inbuf;
        char **pout = &outbuf;

        cd = iconv_open(to_charset,from_charset);
        if (cd==0) return -1;
        memset(outbuf,0,outlen);
        if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
        iconv_close(cd);
        return 0;
}
#endif



std::string UTF8_To_string(const std::string & str) 
{ 
	std::string retStr; 
#if _LINUX
	char outbuf[1024] = {0}; 
	int outlen = 1024;
	code_convert("utf-8","GB18030",str.c_str() ,str.length(),  outbuf,outlen);		
	retStr = outbuf;
#else
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); 

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	memset(pwBuf, 0, nwLen * 2 + 2); 

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen); 

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 

	char * pBuf = new char[nLen + 1]; 
	memset(pBuf, 0, nLen + 1); 

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 

	retStr = pBuf; 

	delete []pBuf; 
	delete []pwBuf; 

	pBuf = NULL; 
	pwBuf = NULL; 

#endif
	return retStr; 
} 

std::string string_To_UTF8(const std::string & str) 
{ 
	std::string retStr; 
#if _LINUX
	char outbuf[1024] = {0}; 
	int outlen = 1024;
	code_convert("GB18030", "utf-8", str.c_str() ,str.length(),  outbuf,outlen);		
	retStr = outbuf;

#else
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0); 

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2); 

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen); 

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 

	char * pBuf = new char[nLen + 1]; 
	ZeroMemory(pBuf, nLen + 1); 

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 

	retStr = pBuf;; 

	delete []pwBuf; 
	delete []pBuf; 

	pwBuf = NULL; 
	pBuf  = NULL; 

#endif
	return retStr; 
} 

time_t convert_string_to_time_t(const std::string & time_string)
{
	struct tm tm1;
	time_t time1;
	int i = sscanf(time_string.c_str(), "%d/%d/%d %d:%d:%d",
		&(tm1.tm_year),
		&(tm1.tm_mon),
		&(tm1.tm_mday),
		&(tm1.tm_hour),
		&(tm1.tm_min),
		&(tm1.tm_sec));

	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);

	return time1;
}

// 判断活动时间是否有效
bool checkStrTimeValid(const Lstring& strBeginTime, const Lstring& strEndTime)
{
	time_t BeginTime = convert_string_to_time_t(strBeginTime.c_str());
	time_t EndTime = convert_string_to_time_t(strEndTime.c_str());

	return checkTimeValid(BeginTime, EndTime);
}

bool checkTimeValid(const time_t& BeginTime, const time_t& EndTime)
{
	if (BeginTime == -1 || EndTime == -1)
	{
		return false;
	}

	if (BeginTime >= EndTime)
	{
		return false;
	}

	LTime curTime;
	Lint curSecond = curTime.Secs();
	if (curSecond >= BeginTime && curSecond <= EndTime)
	{
		return true;
	}
	return false;
}

// 判断时间配置是否合法
bool   checkActiveTimeSetVaild(const Lstring&  strBeginTime, const Lstring&  strEndTime)
{
	time_t BeginTime = convert_string_to_time_t(strBeginTime.c_str());
	time_t EndTime = convert_string_to_time_t(strEndTime.c_str());

	return checkActiveTimeSetVaild(BeginTime, EndTime);
}

bool checkActiveTimeSetVaild(const time_t& BeginTime, const time_t& EndTime)
{
	if (BeginTime == -1 || EndTime == -1 || BeginTime == 0 || EndTime == 0)
	{
		return false;
	}

	if (BeginTime >= EndTime)
	{
		return false;
	}

	LTime curTime;
	Lint curSecond = curTime.Secs();
	if (curSecond <= EndTime)
	{
		return true;
	}

	return false;
}