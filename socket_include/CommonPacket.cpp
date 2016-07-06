#include "Commonpacket.h"
#include "PacketType.h"
#include "TcpSocket.h"

CommonPacket::CommonPacket()
{
}


CommonPacket::~CommonPacket()
{
}

bool CommonPacket::handlePacket(const char* pData, int dataSize)
{
	const SendPacketHead* pHead = reinterpret_cast<const SendPacketHead*>(pData);

	//-----------------------------------------------------------------------------
	//----------------------服务端接收处理的消息列表-------------------------------
	//-----------------------------------------------------------------------------

	//服务端对客户端发送过来的连接消息响应
	if (pHead->Message == MSG_CONNECT)
	{
		char buffer[64];
		BitStream stream(buffer, 64);
		SendPacketHead* pHead = buildPacketHead(stream, MSG_CONNECTED);

		stream.writeInt(m_socket->getClientId());	//发送唯一连接编号

		m_socket->send(stream.getBuffer(), stream.getSize());
		return true;
	}



	//-----------------------------------------------------------------------------
	//----------------------客户端接收处理的消息列表-------------------------------
	//-----------------------------------------------------------------------------

	//客户端对服务端发送过来的连接消息响应
	else if (pHead->Message == MSG_CONNECTED)
	{
		BitStream stream(const_cast<char*>(pData), dataSize);

		m_socket->setClientId(stream.readInt());	//设置连接唯一编号

		return true;
	}

	return false;
}
