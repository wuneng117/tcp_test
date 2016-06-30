#include "CommonServer.h"
#include "ServerSocketClient.h"
#include "Commonpacket.h"

CommonServer::CommonServer()
{
}


CommonServer::~CommonServer()
{
}

ServerSocketClient* CommonServer::loadClient()
{

	ServerSocketClient* pClient = new ServerSocketClient;

	CommonPacket*	pPacket = new CommonPacket;
	pPacket->bindSocket(pClient);
	pClient->bindPacket(pPacket);
	pClient->setMaxPacketSize(m_maxPacketSize);

	return pClient;
}

