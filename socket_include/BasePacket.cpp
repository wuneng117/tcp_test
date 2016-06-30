#include<cstdio>

#include "BasePacket.h"

BasePacket::BasePacket()
{
	m_socket = NULL;
}


BasePacket::~BasePacket()
{
}

bool BasePacket::checkHead(const char* pData)
{
	SendPacketHead* pHead = (SendPacketHead*)pData;
	
	return (pHead->Ckx == CKX && pHead->RSV == pHead->PacketSize);
}

SendPacketHead* BasePacket::buildPacketHead(BitStream& packet, int messageType, int Id, char DestServerType, unsigned long DestZoneId, unsigned long SrcZoneId, char LineServerId)
{
	SendPacketHead* pHead = reinterpret_cast<SendPacketHead*>(packet.getBuffer());

	pHead->Stx = STX;
	pHead->LineServerId = LineServerId;
	pHead->DestServerType = DestServerType;
	pHead->Ckx = CKX;
	pHead->Message = messageType;
	pHead->SrcZoneId = SrcZoneId;
	pHead->DestZoneId = DestZoneId;
	pHead->Id = Id;
	pHead->PacketSize = 0;

	packet.setPosition(sizeof(SendPacketHead));

	return pHead;
}

SendPacketHead* BasePacket::buildCheckMark(const char *pData, int dataSize)
{
	SendPacketHead* pHead = (SendPacketHead*)pData;
	pHead->PacketSize = dataSize - sizeof(SendPacketHead);
	pHead->RSV = pHead->PacketSize;

	return pHead;
}

void  BasePacket::handleErrorPacket(const char* pData, int dataSize)
{
	const SendPacketHead* pHead = reinterpret_cast<const SendPacketHead*>(pData);
	printf("handleErrorPacket: msgId is %d\n", pHead->Message);
}

