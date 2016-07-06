#include <assert.h>

#include "ServerSocketClient.h"
#include "support/BitStream.h"
#include "ServerSocket.h"
#include "base/BasePacket.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

ServerSocketClient::ServerSocketClient()
{
	memset(&clientSockAddr, 0, sizeof(clientSockAddr));
	m_completionPort = NULL;
	m_listenSocket = InvalidSocket;
	m_clientId = 0;

	m_connectType = CLIENT_CONNECT;
	m_state = SSF_SHUT_DOWN;
	m_pServer = NULL;
	m_nLastTransTickCount = GetTickCount();

	m_PendSendTimes = 0;
	m_PendingSendBytes = 0;

	m_accpetOv = NULL;
}

ServerSocketClient::~ServerSocketClient()
{
}

void ServerSocketClient::init(HANDLE completionPort,SOCKET listenSocket, ServerSocket* pServer)
{
	m_completionPort = completionPort;
	m_listenSocket = listenSocket;
	m_pServer = pServer;
}

void ServerSocketClient::start()
{
	if(m_state != SSF_SHUT_DOWN && m_state != SSF_RESTARTING)
		return;

	if(!m_pServer || m_listenSocket == InvalidSocket)
		return;

	//套接字未完全关闭
	if(m_socket != InvalidSocket)
	{
		CancelIo((HANDLE)m_socket);
		closesocket(m_socket);
	}

	resetVar();

	m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if(m_socket == InvalidSocket)
		return;
	
	OVERLAPPED_PLUS* ov = m_pServer->createBuffer(m_clientId, m_maxPacketSize);
	m_accpetOv = ov;

	DWORD dwBytesRecvd;
	int ret = AcceptEx(m_listenSocket, m_socket, ov->WsaBuf.buf, m_maxPacketSize - (sizeof(sockaddr_in)+16) * 2 - 1, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytesRecvd, &(ov->overlapped));
	if(ret != 0)
	{
		DWORD error = GetLastError();
		if(error != ERROR_IO_PENDING)
		{
			postEvent(ServerSocket::OP_RESTART, ov);

			return;
		}
	}

	m_state = SSF_ACCEPTING;
}

void ServerSocketClient::resetVar()
{
	m_state = SSF_SHUT_DOWN;
	m_connectType = CLIENT_CONNECT;
	m_socket = InvalidSocket;

	m_nLastTransTickCount = GetTickCount();

	if(m_pServer)
	{
		m_idleTimeout = m_pServer->getIdleTimeout();
		m_connectTimeout = m_pServer->getConnectTimeout();
	}
	else
	{
		m_idleTimeout = 0;
		m_connectTimeout = 0;
	}

	m_PendSendTimes = 0;
	m_PendingSendBytes = 0;
}
void ServerSocketClient::postEvent(int msg, OVERLAPPED_PLUS* data)
{
	if(msg == SSF_RESTARTING)
		m_state = SSF_RESTARTING;

	if(!data)
		data = m_pServer->createBuffer(m_clientId, 0);

	if(!data)
		return;

	BOOL ret = PostQueuedCompletionStatus(m_completionPort, 0, msg, (LPOVERLAPPED)data);
	if(!ret)
		free(data);
}

bool ServerSocketClient::handleConnect(OVERLAPPED_PLUS* ov, int byteReceived)
{
	if(m_state != SSF_ACCEPTING)
	{
		free(ov);
		return false;
	}

	m_accpetOv = NULL;

	setsockopt( m_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *) &m_listenSocket, sizeof(m_listenSocket) );

	if(!CreateIoCompletionPort((HANDLE)m_socket, m_completionPort, (ULONG_PTR)ServerSocket::OP_NORMAL, 0))
	{
		Net::printWSAError("handleConnect() failed");
		postEvent(ServerSocket::OP_RESTART, ov);
		return false;
	}

	m_state = SSF_CONNECTED;
	
	//设置连接属性，比如心跳包
	DWORD ret;
	tcp_keepalive alive;
	alive.keepalivetime = 120000;
	alive.keepaliveinterval = 1000;
	WSAIoctl(m_socket, SIO_KEEPALIVE_VALS, &alive, sizeof(tcp_keepalive), NULL, 0, &ret, NULL,NULL);

	BOOL val = true;
	if(setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,(const char *)&val,sizeof(BOOL))==SOCKET_ERROR )
	{
		Net::printWSAError("set SO_REUSEADDR failed");
	}

	val = 1; //m_pServer->GetNagle() ? 0 : 1;  //Ray: 这里很容易写反
	if(setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,(const char *)&val,sizeof(BOOL))==SOCKET_ERROR )
	{
		Net::printWSAError("set TCP_NODELAY failed");
	}

	handleReceive(ov, byteReceived);
	return true;
}

bool ServerSocketClient::handleReceive(OVERLAPPED_PLUS* ov, int byteReceived)
{
	if (m_state != SSF_CONNECTED)
	{
		m_pServer->releaseBuffer(ov);
		return false;
	}

	ov->WsaBuf.buf[byteReceived] = 0;
	onReceive(ov->WsaBuf.buf, byteReceived);
	++m_receiveTimes;
	m_nLastTransTickCount = GetTickCount();

	//为下一个重叠调用投递消息
	if (ov->WsaBuf.len != m_maxPacketSize)
	{
		m_pServer->releaseBuffer(ov);
		ov = (OVERLAPPED_PLUS *)m_pServer->createBuffer(m_clientId, m_maxPacketSize);
	}

	DWORD dwBytesReceived = 0;
	ov->WsaBuf.len = m_maxPacketSize - 1;
	DWORD dwFlags = 0;
	if (WSARecv(m_socket, &ov->WsaBuf, 1, &dwBytesReceived, &dwFlags, &(ov->overlapped), NULL))
	{
		DWORD dwLastError = WSAGetLastError();
		if (WSA_IO_PENDING != dwLastError)
		{
			//写日志


			m_pServer->postEvent(ServerSocket::OP_RESTART, ov);
			return false;
		}
	}

	return true;
}

bool ServerSocketClient::handleSend(OVERLAPPED_PLUS* ov, int byteReceived)
{
	m_pServer->releaseBuffer(ov);
	return true;
}

void ServerSocketClient::send(const char* pData, int dataSize)
{
	if (dataSize == 0)
	{
		printf("丢弃0字节发送包\n");
		return;
	}

	if (dataSize > m_maxPacketSize)
	{
		printf("客户端[%d]发送单个数据包%d超过最大发送字节限制%d\n", m_clientId, dataSize, m_maxPacketSize);
		return;
	}

	SendPacketHead* pHead = BasePacket::buildCheckMark(pData, dataSize);

	//发送消息统计，以后再写吧……(在ServerSocket统计)
	/*if (0 != pHead && pHead->Message < MAX_PACKET_COUNT)
	{
	m_NetFNListTotalFreq[pHead->Message]++;
	m_NetFNListTotalSize[pHead->Message] += pHead->PacketSize;
	}*/

	

	OVERLAPPED_PLUS* ov = m_pServer->createBuffer(m_clientId, dataSize);
	if (0 == ov)
	{
		printf("send ov error\n");
		return;
	}

	memcpy(ov->WsaBuf.buf, pData, dataSize);
	//ov->seq = getseq();包序列，以后再说吧……

	ov->bCtxWrite = 1;

	DWORD dwBytesWritten = 0, dwFlags = 0;
	if (WSASend(m_socket, &ov->WsaBuf, 1, &dwBytesWritten, dwFlags, &(ov->overlapped), NULL))
	{
		DWORD dwLastError = WSAGetLastError();
		if (WSA_IO_PENDING != dwLastError)
		{
			//写错误日志

			m_pServer->postEvent(ServerSocket::OP_RESTART, ov);
			return;
		}
	}

	++m_sendTimes;
	m_nLastTransTickCount = GetTickCount();
}
