//created by xiaoshi 7/1/2016
//------------------------------------------------
//CommonServer:
//�̳�ServerSocket����������
//-----------------------------------------------

#pragma once
#include "ServerSocket.h"
class CommonServer :public ServerSocket
{
public:
	CommonServer();
	~CommonServer();

	virtual ServerSocketClient* loadClient();	//���ǣ�ΪServerSocketClient��һ��CommonPacket
};

