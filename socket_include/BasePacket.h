#pragma once
#include "BitStream.h"


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
	~BasePacket();

public:
	static bool checkHead(const char* pData);
	static SendPacketHead* buildPacketHead(BitStream& packet, int messageType = 0, int Id = 0, char DestServerType = 0, unsigned long DestZoneId = 0, unsigned long SrcZoneId = 0, char LineServerId = 0);
	static SendPacketHead* buildCheckMark(const char *pData, int dataSize);

	virtual bool handlePacket(const char* pData, int dataSize) = 0;
	virtual void  handleErrorPacket(const char* pData, int dataSize);

	void bindSocket(TcpSocket* pSocket){ m_socket = pSocket; }
	TcpSocket* getSocket(){ return m_socket; }
protected:
	TcpSocket* m_socket;

};

