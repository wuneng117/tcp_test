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
	BasePacket* m_packet; //���ݰ�����
	char* m_packetBuf;	  //�������ݰ�������
	int  m_packetSize;	  //���ν������ݰ��ܴ�С
	int m_packetPos;	  //�ѽ������ݰ���С
	int	m_nLastTransTickCount;      //the time of the system run
};
