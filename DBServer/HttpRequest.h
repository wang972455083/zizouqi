#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include "LBase.h"
#include "LSingleton.h"
#include <libcurl/curl.h>

class HttpRequest:public LSingleton<HttpRequest>
{
public:
	virtual bool	Init();
	virtual bool	Final();

	//get����,֧��https
	bool			HttpGet(std::string url, std::string& result,bool isHttps=false);
	//post����,֧��https
	bool			HttpPost(std::string url, std::string postData,std::string& result,bool isHttps=false);
	//md5�����㷨
	Lstring			Md5Hex(const void* data,Lsize size);

	//����url
	void			DeodeUrlParams(const Lstring& str, std::map<Lstring, Lstring>& params, bool urldecode);

	Lstring			UrlDecode(const char *src);
};

#endif