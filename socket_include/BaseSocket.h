#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include <WS2tcpip.h>
#include <assert.h>
#include <stdio.h>

#define  InvalidSocket  -1


static void printWSAError(const char* stError)
{
	printf("%s(ErrorCode:%d)\n", stError, WSAGetLastError());
}

static void printLastError(const char* stError)
{
	printf("%s(ErrorCode:%d)\n", stError, GetLastError());
}

//base
class BaseSocket
{
protected:
	int m_clientId;		
	int m_socket;
	int m_maxPacketSize;		//������ݰ���С
	unsigned int m_idleTimeout;
	unsigned int m_connectTimeout;

public:
	BaseSocket();
	~BaseSocket(){};

	void setClientId(int clientId){ m_clientId = clientId; }
	int  getClientId(){ return m_clientId; }

	virtual void setMaxPacketSize(int maxSize)				{m_maxPacketSize = maxSize;}
	void setIdleTimeout(unsigned int idleTimeout)		{m_idleTimeout = idleTimeout;}
	void setConnectTimeout(unsigned int connectTimeout)	{m_connectTimeout = connectTimeout;}
	unsigned int getIdleTimeout()					{return m_idleTimeout;}
	unsigned int getConnectTimeout()				{return m_connectTimeout;}

};

struct Net
{
	static int openSocket();

	static int setBlocking(int socket, bool blockingIO);

	
	
	static void init();
	static void shutdown();

};
