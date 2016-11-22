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

        private void InitForm_Load(object sender, EventArgs e)
        {
            server = new CmdServer("192.168.1.239", 5566);
            this.Message.Text = "Bind CmdServer Success";
            form1 = new Form1(server, this);
            form1.Show();
        }

        private void Message_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
