//created by xiaoshi 6/22/2016
//------------------------------------------------
//BaseSocket:
//����һ��SOCKET�Ļ��������࣬������һЩ����������
//��������TCPͨ�š�
//
//Net:
//������һЩ����SOCKET���õ�ȫ�ֺ�����
//-----------------------------------------------

#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include <WS2tcpip.h>
#include <assert.h>
#include <stdio.h>

#define  InvalidSocket  -1	//��Ч��SOCKET


//��������SOCKET��̳�
class BaseSocket
{
protected:
	int m_clientId;			//�������ַ���ΨһID��ʶ
	int m_socket;
	int m_maxPacketSize;	//������ݰ�����

	unsigned int m_idleTimeout;		//Ĭ�Ͽ��еȴ���ʱʱ��
	unsigned int m_connectTimeout;  //Ĭ�����ӳ�ʱʱ��

public:

	BaseSocket()
	{
		m_clientId = 0;
		m_socket = InvalidSocket;
		m_maxPacketSize = 1024;
		m_idleTimeout = 0;
		m_connectTimeout = 0;
	}

	virtual ~BaseSocket(){};

	void setClientId(int clientId)	{ m_clientId = clientId; }
	int  getClientId()				{ return m_clientId; }
	virtual void setMaxPacketSize(int maxSize)				{m_maxPacketSize = maxSize;}

	void setIdleTimeout(unsigned int idleTimeout)		{m_idleTimeout = idleTimeout;}
	void setConnectTimeout(unsigned int connectTimeout)	{m_connectTimeout = connectTimeout;}
	unsigned int getIdleTimeout()						{return m_idleTimeout;}
	unsigned int getConnectTimeout()					{return m_connectTimeout;}

};

namespace Net
{
	//TCP��ʼ��
	static void init()
	{
		WSADATA wsaData;
		const WORD wVersionRequested = MAKEWORD(2, 2);
		int wsa_startup_err = WSAStartup(wVersionRequested, &wsaData);
		assert(wsa_startup_err == 0);
	}

	//TCP����
	static void shutdown()
	{
		WSACleanup();
	}

	//���س�ʼ������TCP��SOCKET
	static int openSocket()
	{
		int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (nSocket < 0)
		{
			printf("socket() failed:%d\n", WSAGetLastError());
			return InvalidSocket;
		}

		return nSocket;
	}

	//����SOCKET�Ƿ���������
	static int setBlocking(int socket, bool blockingIO)
	{
		unsigned long notblock = !blockingIO;
		int ret = ioctlsocket(socket, FIONBIO, &notblock);
		if (ret != 0)
		{
			int error = WSAGetLastError();
			printf("ioctlsocket() failed:%d\n", error);
			return error;
		}

		return 0;
	}
	
	//���WSA�������
	static void printWSAError(const char* stError)
	{
		printf("%s(ErrorCode:%d)\n", stError, WSAGetLastError());
	}

	//���WINDOWS�������
	static void printLastError(const char* stError)
	{
		printf("%s(ErrorCode:%d)\n", stError, GetLastError());
	}
};
