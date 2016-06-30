#include "TChar.h"
#include <cstdio>
#include <string>
#include <process.h>

#include "ClientSocket.h"
#include "BitStream.h"
#include "BasePacket.h"

unsigned int WINAPI CreateThread(LPVOID param)
{
	ClientSocket* pClientSock = new ClientSocket;
	pClientSock->openConnectTo("192.168.1.41", 5008);

	while (1)
	{
		Sleep(100);
		char buffer[1024];
		BitStream stream(buffer, 1024);
		SendPacketHead* pHead = BasePacket::buildPacketHead(stream);
		
		for (int i = 0; i < 100; ++i)
		{
			stream.writeInt(100);
		}
		pHead->PacketSize = stream.getSize() - sizeof(SendPacketHead);
		pClientSock->send(stream.getBuffer(), stream.getSize());

		pClientSock->process();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Net::init();

	//ClientSocket* pClientSock = new ClientSocket;
	//pClientSock->openConnectTo("192.168.1.41", 5008);
	int loop = 0;
	while (loop++ < 5000)
	{
		unsigned int threadID;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, CreateThread, NULL, 0, &threadID);

		//pClientSock->process();

		//char sendBuffer[MAX_PACKET_SIZE] = {0};
		//BitStream stream(sendBuffer, MAX_PACKET_SIZE);
		//SendPacketHead* head = BasePacket::buildPacketHead(stream);

		//char szBuffer[1024];
		//sprintf_s(szBuffer, "没有什么好说的哎呀\n");
		//for(int i=0; i<1000; ++i)
		//	stream.writeString(szBuffer, 1024);

		//head->PacketSize = stream.getSize() - sizeof(SendPacketHead);
		//pClientSock->sendPacket(stream.getBuffer(), stream.getSize());

		//sprintf(sendBuffer, "sdjkdfjkgjkdfgbfhdjkgfdjkg");
		//pClientSock->sendPacket(sendBuffer, 2048);


	}
	
	while (1)
	{
		Sleep(100);
	}

	Net::shutdown();
	return 0;
}
