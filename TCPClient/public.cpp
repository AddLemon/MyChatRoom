#include "public.h"



void DealRecvMsg()
{
}

string DescribeServSignal(Result result)
{
	string str{};
	switch (result) {
	case success:
		str = "Operation completed!";
		break;
	case idNotExit:
		str = "ID is not exit, please try again.";
		break;
	case passwordNotCorrect:
		str = "Password is not correct, please try again.";
		break;
	case notOnline:
		str = "You are not online, please log in first.";
		break;
	case receiverNotOnline:
		str = "The receiver is not online, please try later.";
		break;
	case duplicateID:
		str = "Your ID is already exit, please try another one.";
		break;
	case receiverNotExit:
		str = "The receiver is not exit, please try again.";
		break;

	}

	return str;
}

#ifdef _WIN32
//UTF-8转窄字符串
string U8toA(const string& s)
{
	//多字节转换为宽字节
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.length(), pwBuf, nwLen);

	//宽字节转换为多字节
	//返回类型是string，属于多字节
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	string sRet = pBuf;

	delete[]pBuf;
	delete[]pwBuf;
	pBuf = NULL;
	pwBuf = NULL;

	return sRet;
}
#endif // _WIN32