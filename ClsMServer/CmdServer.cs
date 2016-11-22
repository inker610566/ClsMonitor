using System;
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

        private LinkedList<AsyncClient> clients;
        private bool exit = false;

        public CmdServer(string IP, Int32 port)
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
                }
            }).Start();
        }

        // Async
        public void Broadcast(string msg, Action OnBroadcaseFinish = null)
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
                if(OnBroadcaseFinish != null) OnBroadcaseFinish();
            }).Start();
        }

        public void Shutdown()
        {
            exit = true;
            server.Stop();
        }
    }
}
