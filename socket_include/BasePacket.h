#pragma once
#include "BitStream.h"


#define STX                             0x36        //包文起始标识
#define CKX                             0x63        //包文校验标识

struct SendPacketHead
{
	char Stx;					//包标识
	char LineServerId;			//线路编号
	char DestServerType;		//目标服务器
	char Ckx;
	unsigned long Message;  	//网络消息
	unsigned long SrcZoneId;    //地图服务器编号
	unsigned long DestZoneId;	//目标
	int	 Id;					//玩家标识
	int  RSV;					//TCP包的CRC校验值，UDP转发会带IP，
	int  PacketSize;			//包内容大小    
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

