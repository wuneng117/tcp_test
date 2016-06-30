#pragma once
#include <hash_map>
#include "BaseSocket.h"

using namespace std;
class ServerSocketClient;

struct OVERLAPPED_PLUS
{
	OVERLAPPED overlapped;  
	INT64		seq;
	int			time;
	int			queueSize;          //save the sendlist size
	int			CtrlFlag;			//the flag of the client(like PT_NEG)
	bool		bCtxWrite;
	int			ClientId;           //the number of the client(dtServerSocketClient)
	//ClientConnect*	pClient;
	WSABUF		WsaBuf;
};

//���ݲ���
struct stOpParam
{
	int msg;
	int val;
};

class ServerSocket :public BaseSocket
{
public:
	enum
	{
		MAX_COMPLETION_THREAD_NUM	= 16,		//���߳�
		ADD_CLIENT_NUM				= 20,
		MAX_PENDING_SEND			= 50,
	};

	enum OP_CODE
	{
		//OP_TIMETRACE,			 //���ٲ鿴IOCP�̵߳Ĵ����ٶ�
		OP_NORMAL,
		OP_QUIT,				//�ر�IOCP�߳�
		OP_RESTART,				
		OP_DISCONNECT,
		OP_SEND,
		OP_ADDCLIENT,			//����acceptex�ȴ��Ŀͻ���		
		OP_MAINTENANCE,			//ά�����пͻ���
		OP_SETCONNECTTYPE,		//���ÿͻ�����������(�ͻ��ˣ�������������

		OP_ON_CONNECT,
		OP_ON_DISCONNECT,
		OP_ON_PACKET,
	};

private:
	sockaddr_in listenSockAddr;

	//IOCP��ɶ˿ں��߳�
	HANDLE m_completionPort;		//IOCP��ɶ˿�
	int m_iocpThreadNum;			//iocp�߳���
	HANDLE m_completionThread[MAX_COMPLETION_THREAD_NUM];	//iocp�̳߳�

	//��������acceptex�˿��������̺߳��¼�
	HANDLE m_addClientMonitorThread;
	HANDLE m_addClientEvent;
	HANDLE m_monitorEndEvent;

public:


	ServerSocket();
	~ServerSocket()	{closesocket(m_socket);};

	bool start(unsigned short port, int max_connect_requests);

	void postEvent(int Msg,void *pData = 0);

	static unsigned int WINAPI addClientMonitorThread(LPVOID lpParam);
	static unsigned int WINAPI IOCPRecvThread(LPVOID lpParam);

	OVERLAPPED_PLUS* createBuffer(int clientId, int size);
	void releaseBuffer(OVERLAPPED_PLUS* ov);

//����ͻ�������
protected:
	hash_map<int, ServerSocketClient*> m_clientMap;
	ServerSocketClient** m_clientArray;

	volatile int m_clientIdx;
	int m_clientCount;
	int m_maxClients;

public:
	void addClient(int num);
	virtual ServerSocketClient* loadClient();
	ServerSocketClient* getClient(int clientId);
	int assingClientId();
};
