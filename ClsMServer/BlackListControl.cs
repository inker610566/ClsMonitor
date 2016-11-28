using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;

namespace ClsMServer
{
    public class BlacklistControl
    {
        private ListView listview;
        private Blacklist blist;
        public BlacklistControl(ListView listview, Blacklist blacklist)
        {
            blist = blacklist;
            this.listview = listview;
            foreach(var s in blist.getCandidateList())
            {
                ListViewItem i = new ListViewItem(s);
                i.ForeColor = blist.Query(s)?System.Drawing.Color.Maroon:System.Drawing.Color.Black;
                listview.Items.Add(i);
            }
        }

        // Add to candidate list
        public void Add(string s)
        {
            if (blist.Add(s))
                listview.Items.Add(s);
        }

        // Update list from Selected Items
        // return message need to be broadcast
        // return null if no item selected
        public Byte[] Enable()
        {
            if (listview.SelectedItems.Count == 0)
                return null;
            string[] ss = new string[listview.SelectedItems.Count];
            Byte[][] bs = new Byte[listview.SelectedItems.Count][];
            for(int i = 0; i < listview.SelectedItems.Count; i ++)
            {
                ListViewItem item = listview.SelectedItems[i];
                item.ForeColor = System.Drawing.Color.Maroon;
                string s = item.Text;
                bs[i] = Blacklist.CmdToByteArray(true, s);
                ss[i] = s;
            }
            blist.Enable(ss);
            return Blacklist.ConcateByteArray(bs);
        }

        // See comment for 'Enable'
        public Byte[] Disable()
        {
            if (listview.SelectedItems.Count == 0)
                return null;
            string[] ss = new string[listview.SelectedItems.Count];
            Byte[][] bs = new Byte[listview.SelectedItems.Count][];
            for(int i = 0; i < listview.SelectedItems.Count; i ++)
            {
                ListViewItem item = listview.SelectedItems[i];
                item.ForeColor = System.Drawing.Color.Black;
                string s = item.Text;
                bs[i] = Blacklist.CmdToByteArray(false, s);
                ss[i] = s;
            }
            blist.Disable(ss);
            return Blacklist.ConcateByteArray(bs);
        }

        // Diff with default blacklist
        public Byte[] ToByteArray()
        {
            return blist.ToByteArray();
        }

    }
}
