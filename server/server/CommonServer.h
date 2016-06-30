#pragma once
#include "ServerSocket.h"
class CommonServer :
	public ServerSocket
{
public:
	CommonServer();
	~CommonServer();

	virtual ServerSocketClient* loadClient();
};

