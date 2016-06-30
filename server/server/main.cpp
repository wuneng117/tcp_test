#include "TChar.h"
#include <cstdio>
#include <string>

#include "CommonServer.h"
#include "BaseSocket.h"
#include "PacketType.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Net::init();
	CommonServer pServer;
	pServer.setMaxPacketSize(MAX_PACKET_SIZE);
	pServer.start(5008, 6000);
	
	while(1)
	{
		Sleep(500);
	}

	return 0;
}
