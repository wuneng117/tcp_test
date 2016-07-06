//created by xiaoshi 6/30/2016
//------------------------------------------------
//ClientSocket:
//�̳�TcpSocket,�ͻ������ӷ����������࣬�÷�������
//ʽ���ܺͷ������ݰ�
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
	char* m_recvBuf;	//���ջ�����

public:
	int m_socketState;	//����״̬
	ClientSocket();
	~ClientSocket();
	void bindPacket(BasePacket* packet){ m_packet = packet; }
	virtual void setMaxPacketSize(int maxSize);

	int openConnectTo(const char* ip, unsigned short port);	//��������

	void handleReceive();	//��������
	virtual void send(const char* pData, int dataSize);	//��������

	void process();
};
