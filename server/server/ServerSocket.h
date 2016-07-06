//created by xiaoshi 6/30/2016
//------------------------------------------------
//OVERLAPPED_PLUS��
//IOCPģ���п��Դ������ݵ��Զ�����
//
//stOpParam
//OVERLAPPED_PLUS������ר�õĲ���������
//��ʱû���õ�
//
//ServerSocket:
//�̳�BasePacket������˹���ͻ���������
//-----------------------------------------------

#pragma once
#include <hash_map>
#include "Base/BaseSocket.h"

using namespace std;
class ServerSocketClient;

struct OVERLAPPED_PLUS
{
	OVERLAPPED overlapped;  
	INT64		seq;
	int			time;
	int			queueSize;          //save the sendlist size
	int			CtrlFlag;			//the flag of the client(like PT_NEG)
	int			ClientId;           //the number of the client(ServerSocketClient)
	//ClientConnect*	pClient;
	bool		bCtxWrite;			//Ϊtrueʱ��ʾ����һ��WSASend����
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
		MAX_COMPLETION_THREAD_NUM	= 1,		//Ϊ���̰߳�ȫ��ֻ�����̣߳��Ժ��ٿ���ô�İɡ�����
		ADD_CLIENT_NUM				= 20,
		MAX_PENDING_SEND			= 50,
	};

	//�Զ���������ͣ�Ϊ�˶��̰߳�ȫ�����ж�ServerSocketClient�Ĵ�����IOCPReceiveThread�ﴦ��
	enum OP_CODE
	{
		//OP_TIMETRACE,			 //���ٲ鿴IOCP�̵߳Ĵ����ٶ�
		OP_NORMAL,				 //����IOCP��Ϣ����
		OP_QUIT,				 //�ر�IOCP�߳�
		OP_RESTART,				 //�����ͻ�������
		OP_DISCONNECT,			 //�Ͽ��ͻ�������
		OP_SEND,				 //���ʹ���δ�õ���ֱ��ServerSocketClient�ﴦ���ͣ�
		OP_ADDCLIENT,			 //����acceptex�ȴ��Ŀͻ���		
		OP_MAINTENANCE,			 //ά�����пͻ�������
		OP_SETCONNECTTYPE,		 //���ÿͻ�����������(�ͻ��ˣ�������������

		//��֪����ɶ�õ�
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
	virtual ~ServerSocket()	{closesocket(m_socket);};

	bool start(unsigned short port, int max_connect_requests);	//���������
	void postEvent(int Msg,void *pData = 0);	//�����Զ������

	static unsigned int WINAPI addClientMonitorThread(LPVOID lpParam);	//���ӿͻ��������߳�
	static unsigned int WINAPI IOCPRecvThread(LPVOID lpParam);			//ServerSocketClient������IOCPģ�Ͳ����߳�

	OVERLAPPED_PLUS* createBuffer(int clientId, int size);	//Ϊ�µ�OVERLAPPED_PLUS�����ڴ�ռ�
	void releaseBuffer(OVERLAPPED_PLUS* ov);				//�ͷ�OVERLAPPED_PLUS

//����ͻ�������
protected:
	hash_map<int, ServerSocketClient*> m_clientMap;	//���пͻ�������
	ServerSocketClient** m_clientArray;				//�ͻ������飬Ӧ����Ϊ�˿��ٷ���

	volatile int m_clientIdx;	//��ǰ�ѷ���ClientId�Ĵ�С
	int m_clientCount;			//��ǰ�ͻ�����������
	int m_maxClients;			//���ͻ�����������

public:
	void addClient(int num);	//����num���ͻ���
	virtual ServerSocketClient* loadClient();	//����һ���µ�ServerSocketCliend��������������Ҫ����
	ServerSocketClient* getClient(int clientId);//����clientID��ŵ�ServerSocketClient����
	int assingClientId();	//����һ���µ�ClientId
};
