//created by xiaoshi 6/26/2016
//------------------------------------------------
//TcpSocket:
//�̳�BaseSocket�ĳ�����,��ͨ�����ݰ��Ľ���ճ����
//�������BasePacket��Ա�������ڽ������ݰ�
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

	virtual void  setMaxPacketSize(int maxSize);		//��������������ݰ����Ⱥ���

	void onReceive(char* pRecvBuf, int nRecvSize);		//ճ������
	void handlePacket(const char* pData, int dataSize); //�������ݰ�
	virtual void send(const char* pData, int dataSize) = 0;	//�������ݰ�

protected:
	BasePacket* m_packet; //���ݰ�������
	char* m_packetBuf;	  //�������ݰ�������
	int  m_packetSize;	  //���ν������ݰ��ܴ�С
	int m_packetPos;	  //�ѽ������ݰ���С
	int	m_nLastTransTickCount;      //the time of the system run
};
