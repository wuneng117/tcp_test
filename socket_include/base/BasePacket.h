//created by xiaoshi 6/26/2016
//------------------------------------------------
//BasePacket:
//这是一个用于处理通信数据包的基础抽象类，是
//TcpSocket的重要组成部分。
//
//SendPacketHead：
//通信数据包包头
//-----------------------------------------------

#pragma once
#include "../support/BitStream.h"

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
	virtual ~BasePacket();

public:
	static bool checkHead(const char* pData);	//标识和CRC校验值检查
	static SendPacketHead* buildCheckMark(const char *pData, int dataSize);	//计算数据包长度和CRC校验值

	//返回一个通信数据包头
	static SendPacketHead* buildPacketHead(BitStream& packet, int messageType = 0, int Id = 0, char DestServerType = 0, unsigned long DestZoneId = 0, unsigned long SrcZoneId = 0, char LineServerId = 0);

	virtual bool handlePacket(const char* pData, int dataSize) = 0;		//处理通信数据包
	virtual void  handleErrorPacket(const char* pData, int dataSize);	//处理错误通信数据包

	TcpSocket* getSocket()				{ return m_socket; }
	void bindSocket(TcpSocket* pSocket)	{ m_socket = pSocket; }
protected:
	TcpSocket* m_socket;	//绑定的TCPSOCKET对象
};

