//created by xiaoshi 6/26/2016
//------------------------------------------------
//TcpSocket:
//继承BaseSocket的抽象类,对通信数据包的进行粘包处
//理，添加了BasePacket成员变量用于解析数据包
//-----------------------------------------------


#pragma once
#include "Base/BaseSocket.h"

class BasePacket;

class TcpSocket :public BaseSocket
{
	typedef BaseSocket Parent;

public:
	TcpSocket();
	virtual ~TcpSocket();

	void	addPacketBuf(const char* addBuf, int addSize);
	char*	getPacketBuf()			{return m_packetBuf;}
	void	addPacketPos(int pos)	{m_packetPos += pos;}
	int		getPacketPos()			{return m_packetPos;}
	int		getPacketSize()			{ return m_packetSize; }

	virtual void  setMaxPacketSize(int maxSize);		//覆盖设置最大数据包长度函数

	void onReceive(char* pRecvBuf, int nRecvSize);		//粘包处理
	void handlePacket(const char* pData, int dataSize); //解析数据包
	virtual void send(const char* pData, int dataSize) = 0;	//发送数据包

protected:
	BasePacket* m_packet; //数据包处理类
	char* m_packetBuf;	  //接受数据包缓存区
	int  m_packetSize;	  //本次接收数据包总大小
	int m_packetPos;	  //已接受数据包大小
	int	m_nLastTransTickCount;      //the time of the system run
};
