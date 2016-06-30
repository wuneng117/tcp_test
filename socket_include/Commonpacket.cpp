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

	if (pHead->Message == MSG_CONNECT)
	{
		char buffer[64];
		BitStream stream(buffer, 64);
		SendPacketHead* pHead = buildPacketHead(stream, MSG_CONNECTED);

		stream.writeInt(m_socket->getClientId());

		m_socket->send(stream.getBuffer(), stream.getSize());
		return true;
	}

	else if (pHead->Message == MSG_CONNECTED)
	{
		BitStream stream(const_cast<char*>(pData), dataSize);

		m_socket->setClientId(stream.readInt());

		return true;
	}

	return false;
}
