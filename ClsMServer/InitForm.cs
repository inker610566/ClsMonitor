using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ClsMServer
{
    public partial class InitForm : Form
    {
        private CmdServer server, stream_server;
        private Blacklist blacklist;
        private Form1 form1;
        private bool IsLockScreen = false;

        public InitForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Should be called in UI thread only
        /// </summary>
        public void LockScreen()
        {
            IsLockScreen = true;
            // type 2 for screenlock
            server.Broadcast(new byte[] {2, 0});
        }

        /// <summary>
        /// Should be called in UI thread only
        /// </summary>
        public void UnLockScreen()
        {
            IsLockScreen = false;
            stream_server.BroadcastClose();
        }

        public void Log(string s)
        {
            if (Message.InvokeRequired)
            {
                Message.BeginInvoke(new Action(() => { Log(s); }));
                return;
            }
            Message.Text += s + "\r\n";
        }

        private void InitForm_Load(object sender, EventArgs e)
        {
            blacklist = new Blacklist("blacklist.txt");
            server = new CmdServer("0.0.0.0", 5566, (c) => {
                Log(String.Format("s1 Client {0} connect\n", new object[] { c.ClientInfo }));
                // send diff with default blacklist
                c.SendAsync(blacklist.ToByteArray());
                if(IsLockScreen)
                {
                    c.SendAsync(new byte[] { 2, 0 }); // LockScreen event
                }
            }, null, (c) => {
                Log(String.Format("s1 Client {0} disconnect\n", new object[] { c }));
            });
            stream_server = new CmdServer("0.0.0.0", 5567, (c) =>
            {
                Log(String.Format("s2 Client {0} connect\n", new object[] { c.ClientInfo }));
            }, (c) =>
            {
                if(!IsLockScreen)
                {
                    c.Close();
                    return false;
                }
                return true;
            }, (c) =>
            {
                Log(String.Format("s2 Client {0} disconnect\n", new object[] { c }));
            });
            this.Message.Text = "Bind CmdServer Success\r\n";
            form1 = new Form1(server, this, blacklist);
            form1.Show();
        }

        private void Message_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
