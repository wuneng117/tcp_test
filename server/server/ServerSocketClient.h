//created by xiaoshi 6/30/2016
//------------------------------------------------
//ServerSocketClient:
//�̳�TcpSocket������˴���ͻ������ӵ���
//-----------------------------------------------

#pragma once

#include "TcpSocket.h"

class ServerSocket;
struct OVERLAPPED_PLUS;

//each client has own connect
class ServerSocketClient:public TcpSocket
{
public:

	//�ͻ�������״̬
	enum SERVER_STATE_FLAGS
	{
		SSF_RESTARTING,
		SSF_DEAD,
		SSF_ACCEPTING,
		SSF_CONNECTING,
		SSF_CONNECTED,
		SSF_SHUTTING_DOWN,
		SSF_SHUT_DOWN,     //�Ѿ��ر�
	};

	//�ͻ�������
	enum SERVER_TYPE
	{
		CLIENT_CONNECT,
		SERVER_CONNECT,
	};

private:
	sockaddr_in clientSockAddr;
	HANDLE m_completionPort;	//IOCP��ɶ˿�
	int m_listenSocket;			//�����������˿�

	int m_connectType;			//��������
	int m_state;				//�ͻ���״̬
	ServerSocket* m_pServer;	

	//��δ�õ�
	int	m_PendSendTimes;
	int	m_PendingSendBytes;

	int m_receiveTimes;			//������Ϣ����
	int m_sendTimes;			//������Ϣ����

	OVERLAPPED_PLUS* m_accpetOv;	//acceptʱ���յ�ov
public:
	ServerSocketClient();
	virtual ~ServerSocketClient();
	
	void init(HANDLE completionPort,SOCKET listenSocket, ServerSocket* pServer);
	void start();	//�����ͻ�������
	void resetVar();
	void bindPacket(BasePacket* packet){ m_packet = packet; }

	void setConnectType(int type)	{m_connectType = type;}
	int  getConnectType()			{return m_connectType;}
	int  getState()					{return m_state;}

	void postEvent(int msg, OVERLAPPED_PLUS* data);	//Ͷ��һ���Զ������

	bool handleConnect(OVERLAPPED_PLUS* ov, int byteReceived);
	bool handleReceive(OVERLAPPED_PLUS* ov, int byteReceived);
	bool handleSend(OVERLAPPED_PLUS* ov, int byteReceived);

	virtual void send(const char* pData, int dataSize);

};
