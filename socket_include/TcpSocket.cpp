#include "TcpSocket.h"
#include "Base/BasePacket.h"

TcpSocket::TcpSocket()
{

	m_packetBuf = NULL;
	m_packetSize = 0;
	m_packetPos = 0;
}

TcpSocket::~TcpSocket()
{
	delete m_packetBuf;	//�ͷ�����Ŀռ�
}

void  TcpSocket::addPacketBuf(const char* addBuf, int addSize)
{
	if (m_packetSize != 0)
		assert(addSize <= m_packetSize - m_packetPos);

	memcpy(m_packetBuf + m_packetPos, addBuf, addSize);
	m_packetPos += addSize;
}

void TcpSocket::setMaxPacketSize(int maxSize)
{
	Parent::setMaxPacketSize(maxSize);
	m_packetBuf = new char[maxSize];	//Ϊ�����������ڴ�ռ�
}

void TcpSocket::onReceive(char* pRecvBuf, int nRecvSize)
{
	int nRecvPos = 0;

	//������һ��BitSteam
	if(m_packetSize > 0)
	{
		int nLastSize = m_packetSize - m_packetPos;	//ʣ����Ҫ���յ��ֽ���

		//���Ҳû�н������
		if(nRecvSize < nLastSize)
		{
			addPacketBuf(pRecvBuf, nRecvSize);
			nRecvSize = 0;
		}
		else
		{
			addPacketBuf(pRecvBuf,nLastSize);
			//���ݰ�����
			handlePacket(m_packetBuf, m_packetSize);

			nRecvPos += nLastSize;
			nRecvSize -= nLastSize;
		}
	}

	//��BitStream�Ĵ���
	while(1)
	{
		if(nRecvSize <= 0)
			break;

		//����ͷδ������ȫ
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

			//�Ѿ��յ�������BitStream
			if (nRecvSize >= m_packetSize)
			{
				addPacketBuf(pRecvBuf+nRecvPos, m_packetSize);

				nRecvPos += m_packetSize;
				nRecvSize -= m_packetSize;

				//BitStream����
				handlePacket(m_packetBuf, m_packetSize);
			}
			//BitStreamδ������ȫ
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
	//���ü�¼�����ݣ�����Ҫ���m_packetBuf������ÿ�ζ�ȡ���Ŀ϶��Ǳ����ǹ��ģ�
	m_packetPos = 0;
	m_packetSize = 0;

	if (m_packet->handlePacket(pData, dataSize))
	{
		m_nLastTransTickCount = GetTickCount();	//��¼����ʱ��
	}
	else
	{
		m_packet->handleErrorPacket(pData, dataSize);
	}
}
