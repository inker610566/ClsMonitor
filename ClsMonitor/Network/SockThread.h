#pragma once
#include "../Process/EventQueue.h"

typedef void(*Callback)(void*);

struct NTCallback
{
	Callback cb;
	void *params;
};

class SockThread
{
	DWORD threadid;
	HANDLE thread;
	NTCallback connect_cb;
	NTCallback disconnect_cb;
public:
	const char* ip;
	int port;
	bool IsConnected;
	EventQueue *q;
	SockThread(const char* ip, int port, EventQueue *q);
	void Start();
	void SetConnectCallback(NTCallback cb);
	void SetDisconnectCallback(NTCallback cb);
	void ConnectEvent();
	void DisconnectEvent();
	~SockThread();
};

