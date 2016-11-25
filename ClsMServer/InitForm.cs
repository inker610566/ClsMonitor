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
            server = new CmdServer("192.168.1.239", 5566, (c) => {
                Log(String.Format("Client {0} connect\n", new object[] { c }));
            }, (c) => {
                Log(String.Format("Client {0} disconnect\n", new object[] { c }));
            });
            this.Message.Text = "Bind CmdServer Success\r\n";
            form1 = new Form1(server, this);
            form1.Show();
        }

        private void Message_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
