//created by xiaoshi 6/30/2016
//------------------------------------------------
//ClientSocket:
//继承TcpSocket,客户端连接服务器处理类，用非阻塞方
//式接受和发送数据包
//-----------------------------------------------


#pragma once
#include <stdio.h>

#include "TcpSocket.h"

class ClientSocket:public TcpSocket
{
	typedef TcpSocket Parent;

public:
	enum SocketState
	{
		InvalidState,
		Connected,
		ConnectionPending,
	};

private:
	sockaddr_in m_clientSockAddr;
	char* m_recvBuf;	//接收缓存区

public:
	int m_socketState;	//连接状态
	ClientSocket();
	~ClientSocket();
	void bindPacket(BasePacket* packet){ m_packet = packet; }
	virtual void setMaxPacketSize(int maxSize);

	int openConnectTo(const char* ip, unsigned short port);	//开启连接

	void handleReceive();	//接受数据
	virtual void send(const char* pData, int dataSize);	//发送数据

	void process();
};
