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
	//----------------------����˽��մ������Ϣ�б�-------------------------------
	//-----------------------------------------------------------------------------

	//����˶Կͻ��˷��͹�����������Ϣ��Ӧ
	if (pHead->Message == MSG_CONNECT)
	{
		char buffer[64];
		BitStream stream(buffer, 64);
		SendPacketHead* pHead = buildPacketHead(stream, MSG_CONNECTED);

		stream.writeInt(m_socket->getClientId());	//����Ψһ���ӱ��

		m_socket->send(stream.getBuffer(), stream.getSize());
		return true;
	}



	//-----------------------------------------------------------------------------
	//----------------------�ͻ��˽��մ������Ϣ�б�-------------------------------
	//-----------------------------------------------------------------------------

	//�ͻ��˶Է���˷��͹�����������Ϣ��Ӧ
	else if (pHead->Message == MSG_CONNECTED)
	{
		BitStream stream(const_cast<char*>(pData), dataSize);

		m_socket->setClientId(stream.readInt());	//��������Ψһ���

		return true;
	}

	return false;
}
