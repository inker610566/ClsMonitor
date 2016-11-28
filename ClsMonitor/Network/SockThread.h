#pragma once

#include "Frame.h"

namespace Network
{
	typedef void(*Callback)(void*);
	typedef void(*FrameCallback)(void*, Frame*);

	struct NTCallback
	{
		Callback cb;
		void *params;
	};

	struct NTFrameCallback
	{
		FrameCallback cb;
		void *params;
	};

	class SockThread
	{
	public:
		struct SockThreadParam
		{
			SockThread *st;
			char *RecvBuf;
		};
		const char* ip;
		int port;
		SockThread(const char* ip, int port);
		void Start();
		void SetConnectCallback(NTCallback cb);
		void SetDisconnectCallback(NTCallback cb);
		void SetFrameCallback(NTFrameCallback cb);
		void TriggerConnectEvent();
		void TriggerDisconnectEvent();
		void TriggerFrameEvent(Frame *f);
		~SockThread();
	private:
		DWORD threadid;
		HANDLE thread;
		NTCallback connect_cb;
		NTCallback disconnect_cb;
		NTFrameCallback frame_cb;
		SockThreadParam stp;
	};
}
