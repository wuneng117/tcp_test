#pragma once
#include "singleton.h"
#include "TcpSocket.h"
#include <stdio.h>

const int MAX_PACKET_SIZE = 32 * 1024;

class ClientSocket:public TcpSocket
{
public:
	enum SocketState
	{
		InvalidState,
		Connected,
		ConnectionPending,
	};

private:
	sockaddr_in m_clientSockAddr;
	char* m_recvBuf;	//Ω” ’ª∫¥Ê«¯
public:
	int m_socketState;
	ClientSocket();
	~ClientSocket();

	int openConnectTo(const char* ip, unsigned short port);

	void bindPacket(BasePacket* packet){ m_packet = packet; }

	void handleReceive();
	virtual void send(const char* pData, int dataSize);

	virtual void handlePacket();

	void process();
};
