#include "TcpSocket.h"
#include "BasePacket.h"

TcpSocket::TcpSocket()
{

	m_packetBuf = NULL;
	m_packetSize = 0;
	m_packetPos = 0;
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::setMaxPacketSize(int maxSize)
{
	m_maxPacketSize = maxSize;
	m_packetBuf = new char[maxSize];
}

void TcpSocket::onReceive(char* pRecvBuf, int nRecvSize)
{
	int nRecvPos = 0;

	//处理上一个BitSteam
	if(m_packetSize > 0)
	{
		int nLastSize = m_packetSize - m_packetPos;	//剩余需要接收的字节数

		//这次也没有接收完毕
		if(nRecvSize < nLastSize)
		{
			addPacketBuf(pRecvBuf, nRecvSize);
			nRecvSize = 0;
		}
		else
		{
			addPacketBuf(pRecvBuf,nLastSize);
			//数据包处理，比如打印出来
			handlePacket(m_packetBuf, m_packetSize);

			nRecvPos += nLastSize;
			nRecvSize -= nLastSize;
		}
	}

	//新BitStream的处理
	while(1)
	{
		if(nRecvSize <= 0)
			break;

		//数据头未接收完全
		if(nRecvSize < sizeof(SendPacketHead) - m_packetPos)
		{
			addPacketBuf(pRecvBuf+nRecvPos, nRecvSize);
			m_packetSize = 0;
			break;
		}
		else
		{
			SendPacketHead* pHead = reinterpret_cast<SendPacketHead*>(pRecvBuf+nRecvPos);

			m_packetSize = pHead->PacketSize+sizeof(SendPacketHead);

			//已经收到完整的BitStream
			if (nRecvSize >= m_packetSize)
			{
				addPacketBuf(pRecvBuf+nRecvPos, m_packetSize);

				nRecvPos += m_packetSize;
				nRecvSize -= m_packetSize;

				//BitStream处理，比如打印出来
				handlePacket(m_packetBuf, m_packetSize);
			}
			//BitStream未接收完全
			else
			{
				addPacketBuf(pRecvBuf+nRecvPos, nRecvSize);
				break;
			}
		}
	}
}

void TcpSocket::handlePacket(const char* pData, int dataSize)
{
	m_packetPos = 0;
	m_packetSize = 0;

	if (m_packet->handlePacket(pData, dataSize))
	{
		m_nLastTransTickCount = GetTickCount();
	}
	else
	{
		m_packet->handleErrorPacket(pData, dataSize);
	}
}

void  TcpSocket::addPacketBuf(const char* addBuf, int addSize)
{
	if(m_packetSize != -1)
		assert(addSize <= m_packetSize-m_packetPos);

	memcpy(m_packetBuf+m_packetPos, addBuf, addSize);
	m_packetPos += addSize;
}