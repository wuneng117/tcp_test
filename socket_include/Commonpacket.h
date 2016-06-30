#pragma once
#include "BasePacket.h"

class CommonPacket :public BasePacket
{
public:
	CommonPacket();
	~CommonPacket();

	virtual bool handlePacket(const char* pData, int dataSize);
};

