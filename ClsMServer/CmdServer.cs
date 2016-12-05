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
        private LinkedList<AsyncClient> clients = new LinkedList<AsyncClient>();
        private bool exit = false;

        public delegate void ClientInfoEvent(string client);
        public delegate void ClientEvent(AsyncClient client);
        public delegate bool ClientResultEvent(AsyncClient client);
        //private ClientEvent onConnect;
        private ClientInfoEvent onDisconnect;

        // NOTE: onDisconnect probably will not be triggered right after client disconnect
        //       (usaually triggered after few rounds of broadcast)
        public CmdServer(string IP, Int32 port,
            ClientEvent onConnect = null,
            ClientResultEvent PreConnect = null,
            ClientInfoEvent onDisconnect = null)
        {
            this.onDisconnect = onDisconnect;
            server = new TcpListener(IPAddress.Parse(IP), port);
            server.Start();
            new Thread(() =>
            {
                while (!exit)
                {
                    AsyncClient c = new AsyncClient(server.AcceptTcpClient());
                    if (PreConnect != null && !PreConnect(c))
                        continue;
                    // Be aware of the interval after PreConnect and before lock clients
                    // clients can be not empty when filter through PreConnect
                    lock (clients)
                    {
                        clients.AddLast(c);
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
                        {
                            clients.Remove(node);
                            if (onDisconnect != null)
                                onDisconnect(node.Value.ClientInfo);
                        }
                        else
                            node.Value.SendAsync(msg);
                        node = nextNode;
                    }
                }
                if(OnBroadcastFinish != null) OnBroadcastFinish();
            }).Start();
        }

        public void BroadcastClose()
        {
            new Thread(() =>
            {
                lock (clients)
                {
                    for(var node = clients.First; node != null; node = node.Next)
                        node.Value.Close();
                    clients.Clear();
                }
            }).Start();
        }

        public void Shutdown()
        {
            exit = true;
            server.Stop();
        }
    }
}
