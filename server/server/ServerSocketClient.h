#pragma once

#include "TcpSocket.h"

class ServerSocket;
struct OVERLAPPED_PLUS;

//each client has own connect
class ServerSocketClient:public TcpSocket
{
public:

	enum SERVER_STATE_FLAGS
	{
		SSF_RESTARTING,
		SSF_DEAD,
		SSF_ACCEPTING,
		SSF_CONNECTING,
		SSF_CONNECTED,
		SSF_SHUTTING_DOWN,
		SSF_SHUT_DOWN,     //已经关闭
	};

	enum SERVER_TYPE
	{
		CLIENT_CONNECT,
		SERVER_CONNECT,
	};

private:
	sockaddr_in clientSockAddr;
	HANDLE m_completionPort;	//IOCP完成端口
	int m_listenSocket;			//服务器监听端口

	int m_connectType;			//连接类型
	int m_state;				//客户端状态
	ServerSocket* m_pServer;	

	int	m_PendSendTimes;
	int	m_PendingSendBytes;

	int m_receiveTimes;			//接收消息次数
	int m_sendTimes;			//发送消息次数

	OVERLAPPED_PLUS* m_accpetOv;	//accept时接收的ov
public:
	ServerSocketClient();
	~ServerSocketClient();
	
	void init(HANDLE completionPort,SOCKET listenSocket, ServerSocket* pServer);
	void start();
	void resetVar();
	void bindPacket(BasePacket* packet){ m_packet = packet; }

	void setConnectType(int type) {m_connectType = type;}
	int  getConnectType() {return m_connectType;}
	int  getState() {return m_state;}

	void postEvent(int msg, OVERLAPPED_PLUS* data);

	bool handleConnect(OVERLAPPED_PLUS* ov, int byteReceived);
	bool handleReceive(OVERLAPPED_PLUS* ov, int byteReceived);
	bool handleSend(OVERLAPPED_PLUS* ov, int byteReceived);

	virtual void send(const char* pData, int dataSize);

};
