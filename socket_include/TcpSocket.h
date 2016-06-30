#pragma once
#include "BaseSocket.h"

class BasePacket;

class TcpSocket :public BaseSocket
{
public:
	TcpSocket();
	~TcpSocket();


	char* getPacketBuf()			{return m_packetBuf;}
	void  addPacketPos(int pos)		{m_packetPos += pos;}
	void  addPacketBuf(const char* addBuf, int addSize);
	int getPacketSize()				{return m_packetSize;}
	int getPacketPos()				{return m_packetPos;}

	virtual void  setMaxPacketSize(int maxSize);

	void onReceive(char* pRecvBuf, int nRecvSize);
	void handlePacket(const char* pData, int dataSize);
	virtual void send(const char* pData, int dataSize) = 0;
protected:
	BasePacket* m_packet; //数据包处理
	char* m_packetBuf;	  //接受数据包缓存区
	int  m_packetSize;	  //本次接收数据包总大小
	int m_packetPos;	  //已接受数据包大小
	int	m_nLastTransTickCount;      //the time of the system run
};
