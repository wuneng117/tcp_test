//created by xiaoshi 6/30/2016
//------------------------------------------------
//CommonPacket:
//�̳�BasePacket��������Ϣ�������ݰ�
//-----------------------------------------------

#pragma once
#include "Base/BasePacket.h"

class CommonPacket :public BasePacket
{
public:
	CommonPacket();
	~CommonPacket();

	virtual bool handlePacket(const char* pData, int dataSize);
};

