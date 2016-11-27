#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SockThread.h"
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib,"WS2_32")
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "ConsoleLogger.h"
#include "SockThreadException.h"
using namespace std;

char RecvBuf[256];

int RecvUtil(SOCKET s, char *buf, int len);

DWORD WINAPI PSockThread(
	_In_ LPVOID lpParameter
)
{
	SockThread *st = (SockThread*)lpParameter;

	// Init Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		 stringstream ss;
		 ss << "WSAStartup failed with error: " << iResult;
		 ConsoleLogger::getInstance()->log(ss.str());
		 return 0;
	}

	do
	{
		SOCKADDR_IN addr;
		ZeroMemory( &addr, sizeof(addr));
		addr.sin_addr.s_addr = inet_addr(st->ip);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(st->port);

		SOCKET sock = INVALID_SOCKET;
		if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			 stringstream ss;
			 ss << "Socket create error";
			 ConsoleLogger::getInstance()->log(ss.str());
			 break;
		}

		if(connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR){
			 stringstream ss;
			 ss << "Socket connect error";
			 ConsoleLogger::getInstance()->log(ss.str());
			 continue;
		}

		st->IsConnected = true;
		st->ConnectEvent();

		while (true)
		{
			// AddOrDel
			int result = RecvUtil(sock, RecvBuf, 1);
			if (result == 0 || result == SOCKET_ERROR) break;
			int EventType = RecvBuf[0];
			// size
			result = RecvUtil(sock, RecvBuf, 1);
			if (result == 0 || result == SOCKET_ERROR) break;
			int size = *((unsigned char*)RecvBuf);
			// Name
			result = RecvUtil(sock, RecvBuf, size);
			if (result == 0 || result == SOCKET_ERROR) break;

			switch (EventType)
			{
				case 0:
				case 1:
					st->q->Push(new QueueEvent(EventType, wstring((const wchar_t*)RecvBuf, size/2))); // if size/2???
					break;
			}
		}
		st->IsConnected = false;
		st->DisconnectEvent();
	} while (true);
	return 0;
}

int RecvUtil(SOCKET s, char *buf, int len)
{
	int rlen = len;
	while (rlen > 0)
	{
		int cnt = recv(s, buf, rlen, MSG_WAITALL);
		if (cnt == 0) return 0;
		else if (cnt == SOCKET_ERROR)
		{
			ConsoleLogger::getInstance()->log("Socket Recv Error");
			return SOCKET_ERROR;
		}
		rlen -= cnt;
		buf += cnt;
	}
	return len;
}


SockThread::SockThread(const char* ip, int port, EventQueue* q)
	:ip(ip), port(port), q(q), IsConnected(false)
{
	connect_cb.cb = NULL;
	disconnect_cb.cb = NULL;
}

void SockThread::Start()
{
	// Start Thread
	thread = CreateThread(
		NULL,
		0,
		PSockThread,
		this,
		0,
		&threadid
	);
}

void SockThread::SetConnectCallback(NTCallback cb)
{
	connect_cb = cb;
}

void SockThread::SetDisconnectCallback(NTCallback cb)
{
	disconnect_cb = cb;
}

void SockThread::ConnectEvent()
{
	if (connect_cb.cb != NULL)
		connect_cb.cb(connect_cb.params);
}

void SockThread::DisconnectEvent()
{
	if (disconnect_cb.cb != NULL)
		disconnect_cb.cb(disconnect_cb.params);
}

SockThread::~SockThread()
{
}
