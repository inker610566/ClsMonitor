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
    public partial class Form1 : Form
    {
        private AddForm addForm;
        private CmdServer cmdServer;
        private InitForm msgForm;
        public BlacklistControl blacklist;
        public Form1(CmdServer server, InitForm msgForm, Blacklist blist)
        {
            this.cmdServer = server;
            this.msgForm = msgForm;
            InitializeComponent();
            this.blacklist = new BlacklistControl(listView1, blist);
            addForm = new AddForm();

            listView1.ContextMenuStrip = ClsMServer.MenuFactory.Produce(
                ()=>
                {
                    DialogResult r = addForm.ShowDialog();
                    if(r == DialogResult.OK)
                    {
                        blacklist.Add(addForm.AddString);
                    }
                },
                ()=>
                {
                    // Enable
                    Byte[] msg = blacklist.Enable();
                    if(msg != null) cmdServer.Broadcast(msg);
                },
                ()=>
                {
                    Byte[] msg = blacklist.Disable();
                    if(msg != null) cmdServer.Broadcast(msg);
                },
                ()=>
                {
                    msgForm.LockScreen();
                },
                ()=>
                {
                    msgForm.UnLockScreen();
                });
        }
    }
}
