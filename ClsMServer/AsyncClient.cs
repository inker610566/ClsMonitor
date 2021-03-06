﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ClsMServer
{
    public class AsyncClient
    {
        private TcpClient client;
        private Queue<Byte[]> q = new Queue<Byte[]>();
        private ManualResetEvent pending_pop = new ManualResetEvent(true);
        private Thread thread;
        public string ClientInfo { get; private set; }
        public bool isClosed { get; private set; } = false;
        
        public AsyncClient(TcpClient client)
        {
            this.client = client;
            ClientInfo = client.Client.RemoteEndPoint.ToString();
            this.thread = new Thread(() =>
            {
                // consumer
                try
                {
                    while (true)
                    {
                        // Dequeue
                        Byte[] msg = null;
                        lock (q)
                        {
                            if (q.Count != 0)
                                msg = q.Dequeue();
                            else
                                pending_pop.Reset();
                        }
                        if (msg == null)
                        {
                            pending_pop.WaitOne();
                            continue;
                        }
                        client.GetStream().Write(msg, 0, msg.Length);
                    }
                }
                catch(Exception)
                {
                    this.client.Close();
                    this.isClosed = true;
                }
            });
            this.thread.Start();
        }

        public void SendAsync(Byte[] msg)
        {
            lock (q)
            {
                q.Enqueue(msg);
                if(q.Count == 1) pending_pop.Set();
            }
        }

        public void Close()
        {
            this.thread.Abort();
            this.client.Close();
            this.isClosed = true;
        }
    }
}
