//created by xiaoshi 6/30/2016
//------------------------------------------------
//CommonPacket:
//继承BasePacket，根据消息处理数据包
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

