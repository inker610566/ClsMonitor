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
        private CmdServer server;
        private Blacklist blacklist;
        private Form1 form1;

        public InitForm()
        {
            InitializeComponent();
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
                Log(String.Format("Client {0} connect\n", new object[] { c.ClientInfo }));
                // send diff with default blacklist
                c.SendAsync(blacklist.ToByteArray());
            }, (c) => {
                Log(String.Format("Client {0} disconnect\n", new object[] { c }));
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
