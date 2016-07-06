//created by xiaoshi 6/22/2016
//------------------------------------------------
//BaseSocket:
//这是一个SOCKET的基础抽象类，定义了一些基础变量和
//函数用于TCP通信。
//
//Net:
//包含了一些网络SOCKET常用的全局函数。
//-----------------------------------------------

#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include <WS2tcpip.h>
#include <assert.h>
#include <stdio.h>

#define  InvalidSocket  -1	//无效的SOCKET


//用于其他SOCKET类继承
class BaseSocket
{
protected:
	int m_clientId;			//服务器分发的唯一ID标识
	int m_socket;
	int m_maxPacketSize;	//最大数据包长度

	unsigned int m_idleTimeout;		//默认空闲等待超时时间
	unsigned int m_connectTimeout;  //默认连接超时时间

public:

	BaseSocket()
	{
		m_clientId = 0;
		m_socket = InvalidSocket;
		m_maxPacketSize = 1024;
		m_idleTimeout = 0;
		m_connectTimeout = 0;
	}

	virtual ~BaseSocket(){};

	void setClientId(int clientId)	{ m_clientId = clientId; }
	int  getClientId()				{ return m_clientId; }
	virtual void setMaxPacketSize(int maxSize)				{m_maxPacketSize = maxSize;}

	void setIdleTimeout(unsigned int idleTimeout)		{m_idleTimeout = idleTimeout;}
	void setConnectTimeout(unsigned int connectTimeout)	{m_connectTimeout = connectTimeout;}
	unsigned int getIdleTimeout()						{return m_idleTimeout;}
	unsigned int getConnectTimeout()					{return m_connectTimeout;}

};

namespace Net
{
	//TCP初始化
	static void init()
	{
		WSADATA wsaData;
		const WORD wVersionRequested = MAKEWORD(2, 2);
		int wsa_startup_err = WSAStartup(wVersionRequested, &wsaData);
		assert(wsa_startup_err == 0);
	}

	//TCP结束
	static void shutdown()
	{
		WSACleanup();
	}

	//返回初始化过的TCP用SOCKET
	static int openSocket()
	{
		int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (nSocket < 0)
		{
			printf("socket() failed:%d\n", WSAGetLastError());
			return InvalidSocket;
		}

		return nSocket;
	}

	//设置SOCKET是否阻塞处理
	static int setBlocking(int socket, bool blockingIO)
	{
		unsigned long notblock = !blockingIO;
		int ret = ioctlsocket(socket, FIONBIO, &notblock);
		if (ret != 0)
		{
			int error = WSAGetLastError();
			printf("ioctlsocket() failed:%d\n", error);
			return error;
		}

		return 0;
	}
	
	//输出WSA错误代码
	static void printWSAError(const char* stError)
	{
		printf("%s(ErrorCode:%d)\n", stError, WSAGetLastError());
	}

	//输出WINDOWS错误代码
	static void printLastError(const char* stError)
	{
		printf("%s(ErrorCode:%d)\n", stError, GetLastError());
	}
};
