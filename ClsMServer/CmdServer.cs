﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace ClsMServer
{
    public class CmdServer
    {
        private TcpListener server;
        private LinkedList<AsyncClient> clients = new LinkedList<AsyncClient>();
        private bool exit = false;

        public delegate void ClientEvent(TcpClient client);
        //private ClientEvent onConnect;

        public CmdServer(string IP, Int32 port, ClientEvent onConnect = null)
        {
            server = new TcpListener(IPAddress.Parse(IP), port);
            server.Start();
            new Thread(() =>
            {
                while (!exit)
                {
                    TcpClient c = server.AcceptTcpClient();
                    lock (clients)
                    {
                        clients.AddLast(new AsyncClient(c));
                    }
                    if(onConnect != null)
                        onConnect(c);
                }
            }).Start();
        }

        // Async
        public void Broadcast(Byte[] msg, Action OnBroadcastFinish = null)
        {
            new Thread(() =>
            {
                lock(clients)
                {
                    for(var node = clients.First; node != null; ) {
                        var nextNode = node.Next;
                        if (node.Value.isClosed)
                            clients.Remove(node);
                        else
                            node.Value.SendAsync(msg);
                        node = nextNode;
                    }
                }
                if(OnBroadcastFinish != null) OnBroadcastFinish();
            }).Start();
        }

        public void Shutdown()
        {
            exit = true;
            server.Stop();
        }
    }
}
