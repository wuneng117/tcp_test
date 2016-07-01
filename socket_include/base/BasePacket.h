//created by xiaoshi 6/26/2016
//------------------------------------------------
//BasePacket:
//����һ�����ڴ���ͨ�����ݰ��Ļ��������࣬��
//TcpSocket����Ҫ��ɲ��֡�
//
//SendPacketHead��
//ͨ�����ݰ���ͷ
//-----------------------------------------------

#pragma once
#include "../support/BitStream.h"

#define STX                             0x36        //������ʼ��ʶ
#define CKX                             0x63        //����У���ʶ

struct SendPacketHead
{
	char Stx;					//����ʶ
	char LineServerId;			//��·���
	char DestServerType;		//Ŀ�������
	char Ckx;
	unsigned long Message;  	//������Ϣ
	unsigned long SrcZoneId;    //��ͼ���������
	unsigned long DestZoneId;	//Ŀ��
	int	 Id;					//��ұ�ʶ
	int  RSV;					//TCP����CRCУ��ֵ��UDPת�����IP��
	int  PacketSize;			//�����ݴ�С    
};

class TcpSocket;
class BasePacket
{
public:
	BasePacket();
	virtual ~BasePacket();

public:
	static bool checkHead(const char* pData);	//��ʶ��CRCУ��ֵ���
	static SendPacketHead* buildCheckMark(const char *pData, int dataSize);	//�������ݰ����Ⱥ�CRCУ��ֵ

	//����һ��ͨ�����ݰ�ͷ
	static SendPacketHead* buildPacketHead(BitStream& packet, int messageType = 0, int Id = 0, char DestServerType = 0, unsigned long DestZoneId = 0, unsigned long SrcZoneId = 0, char LineServerId = 0);

	virtual bool handlePacket(const char* pData, int dataSize) = 0;		//����ͨ�����ݰ�
	virtual void  handleErrorPacket(const char* pData, int dataSize);	//�������ͨ�����ݰ�

	TcpSocket* getSocket()				{ return m_socket; }
	void bindSocket(TcpSocket* pSocket)	{ m_socket = pSocket; }
protected:
	TcpSocket* m_socket;	//�󶨵�TCPSOCKET����
};

