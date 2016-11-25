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
        private BlackList blacklist;
        public Form1(CmdServer server, InitForm msgForm)
        {
            this.cmdServer = server;
            this.msgForm = msgForm;
            InitializeComponent();
            this.blacklist = new BlackList(listView1);
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
                    if (listView1.SelectedItems.Count == 0)
                        return;
                    Byte[][] bs = new Byte[listView1.SelectedItems.Count][];
                    for(int i = 0; i < listView1.SelectedItems.Count; i ++)
                    {
                        bs[i] = BlackList.CmdToByteArray(true, listView1.SelectedItems[i].Text);
                    }
                    cmdServer.Broadcast(BlackList.ConcateByteArray(bs));
                    for(int i = 0; i < listView1.SelectedItems.Count; i ++)
                        listView1.SelectedItems[i].BackColor = Color.AliceBlue;
                },
                ()=>
                {
                    if (listView1.SelectedItems.Count == 0)
                        return;
                    Byte[][] bs = new Byte[listView1.SelectedItems.Count][];
                    for(int i = 0; i < listView1.SelectedItems.Count; i ++)
                    {
                        bs[i] = BlackList.CmdToByteArray(false, listView1.SelectedItems[i].Text);
                    }
                    cmdServer.Broadcast(BlackList.ConcateByteArray(bs));
                    for(int i = 0; i < listView1.SelectedItems.Count; i ++)
                        listView1.SelectedItems[i].BackColor = Color.Empty;
                });
        }
    }
}
