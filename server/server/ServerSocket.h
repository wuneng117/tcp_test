//created by xiaoshi 6/30/2016
//------------------------------------------------
//OVERLAPPED_PLUS：
//IOCP模型中可以传递数据的自定义类
//
//stOpParam
//OVERLAPPED_PLUS传递是专用的参数处理类
//暂时没有用到
//
//ServerSocket:
//继承BasePacket，服务端管理客户端连接类
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
	bool		bCtxWrite;			//为true时表示这是一个WSASend传递
	WSABUF		WsaBuf;
};

//传递参数
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
		MAX_COMPLETION_THREAD_NUM	= 1,		//为了线程安全，只开单线程（以后再看怎么改吧……）
		ADD_CLIENT_NUM				= 20,
		MAX_PENDING_SEND			= 50,
	};

	//自定义操作类型，为了多线程安全，所有对ServerSocketClient的处理都在IOCPReceiveThread里处理
	enum OP_CODE
	{
		//OP_TIMETRACE,			 //跟踪查看IOCP线程的处理速度
		OP_NORMAL,				 //正常IOCP消息处理
		OP_QUIT,				 //关闭IOCP线程
		OP_RESTART,				 //重启客户端连接
		OP_DISCONNECT,			 //断开客户端连接
		OP_SEND,				 //发送处理（未用到，直接ServerSocketClient里处理发送）
		OP_ADDCLIENT,			 //增加acceptex等待的客户端		
		OP_MAINTENANCE,			 //维护所有客户端数量
		OP_SETCONNECTTYPE,		 //设置客户端连接类型(客户端，其他服务器）

		//不知道干啥用的
		OP_ON_CONNECT,
		OP_ON_DISCONNECT,
		OP_ON_PACKET,
	};

private:
	sockaddr_in listenSockAddr;

	//IOCP完成端口和线程
	HANDLE m_completionPort;		//IOCP完成端口
	int m_iocpThreadNum;			//iocp线程数
	HANDLE m_completionThread[MAX_COMPLETION_THREAD_NUM];	//iocp线程池

	//辅助监视acceptex端口数量的线程和事件
	HANDLE m_addClientMonitorThread;
	HANDLE m_addClientEvent;
	HANDLE m_monitorEndEvent;

public:


	ServerSocket();
	virtual ~ServerSocket()	{closesocket(m_socket);};

	bool start(unsigned short port, int max_connect_requests);	//开启服务端
	void postEvent(int Msg,void *pData = 0);	//发送自定义操作

	static unsigned int WINAPI addClientMonitorThread(LPVOID lpParam);	//监视客户端熟练线程
	static unsigned int WINAPI IOCPRecvThread(LPVOID lpParam);			//ServerSocketClient操作、IOCP模型操作线程

	OVERLAPPED_PLUS* createBuffer(int clientId, int size);	//为新的OVERLAPPED_PLUS申请内存空间
	void releaseBuffer(OVERLAPPED_PLUS* ov);				//释放OVERLAPPED_PLUS

//管理客户端连接
protected:
	hash_map<int, ServerSocketClient*> m_clientMap;	//所有客户端连接
	ServerSocketClient** m_clientArray;				//客户端数组，应该是为了快速访问

	volatile int m_clientIdx;	//当前已分配ClientId的大小
	int m_clientCount;			//当前客户端连接数量
	int m_maxClients;			//最大客户端连接数量

public:
	void addClient(int num);	//增加num个客户端
	virtual ServerSocketClient* loadClient();	//创建一个新的ServerSocketCliend对象，在子类中需要覆盖
	ServerSocketClient* getClient(int clientId);//返回clientID编号的ServerSocketClient对象
	int assingClientId();	//申请一个新的ClientId
};
