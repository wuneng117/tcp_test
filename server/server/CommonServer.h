//created by xiaoshi 7/1/2016
//------------------------------------------------
//CommonServer:
//继承ServerSocket，服务器类
//-----------------------------------------------

#pragma once
#include "ServerSocket.h"
class CommonServer :public ServerSocket
{
public:
	CommonServer();
	~CommonServer();

	virtual ServerSocketClient* loadClient();	//覆盖，为ServerSocketClient绑定一个CommonPacket
};

