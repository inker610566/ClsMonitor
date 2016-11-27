using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;

namespace ClsMServer
{
    public class BlackList
    {
        private HashSet<string> candidatelist = new HashSet<string>(),
                                defaultlist = new HashSet<string>(),
                                minus_set= new HashSet<string>(),
                                add_set = new HashSet<string>();
        private ListView listview;
        public BlackList(ListView listview, string filepath = null)
        {
            this.listview = listview;
            if(filepath != null && System.IO.File.Exists(filepath))
            {
                foreach (var s in System.IO.File.ReadAllLines(filepath))
                {
                    defaultlist.Add(s);
                    candidatelist.Add(s);
                    ListViewItem i = new ListViewItem(s);
                    i.ForeColor = System.Drawing.Color.Maroon;
                    listview.Items.Add(i);
                }
            }
        }

        // Add to candidate list
        public bool Add(string s)
        {
            if (candidatelist.Contains(s))
                return false;
            candidatelist.Add(s);
            listview.Items.Add(s);
            return true;
        }

        // Update list from Selected Items
        // return message need to be broadcast
        // return null if no item selected
        public Byte[] Enable()
        {
            if (listview.SelectedItems.Count == 0)
                return null;
            lock(add_set)
            {
                Byte[][] bs = new Byte[listview.SelectedItems.Count][];
                for(int i = 0; i < listview.SelectedItems.Count; i ++)
                {
                    ListViewItem item = listview.SelectedItems[i];
                    item.ForeColor = System.Drawing.Color.Maroon;
                    string s = item.Text;
                    bs[i] = BlackList.CmdToByteArray(true, s);
                    if (defaultlist.Contains(s))
                        minus_set.Remove(s);
                    else
                        add_set.Add(s);
                }
                return BlackList.ConcateByteArray(bs);
            }
        }

        // See comment for 'Enable'
        public Byte[] Disable()
        {
            if (listview.SelectedItems.Count == 0)
                return null;
            lock(add_set)
            {
                Byte[][] bs = new Byte[listview.SelectedItems.Count][];
                for(int i = 0; i < listview.SelectedItems.Count; i ++)
                {
                    ListViewItem item = listview.SelectedItems[i];
                    item.ForeColor = System.Drawing.Color.Black;
                    string s = item.Text;
                    bs[i] = BlackList.CmdToByteArray(false, s);
                    if (defaultlist.Contains(s))
                        minus_set.Add(s);
                    else
                        add_set.Remove(s);
                }
                return BlackList.ConcateByteArray(bs);
            }
        }

        // Diff with default blacklist
        public Byte[] ToByteArray()
        {
            lock (add_set)
            {
                return ConcateByteArray(
                    add_set.Select((s) => CmdToByteArray(true, s)).Concat(
                        minus_set.Select((s) => CmdToByteArray(false, s))).ToArray()
                );
            }
        }

        public static Byte[] ConcateByteArray(params Byte[][] bytes_arrays)
        {
            int total = bytes_arrays.Aggregate(0, (acc, x) => x.Length + acc);
            int offset = 0;
            Byte[] bs = new Byte[total];

            foreach(var b in bytes_arrays)
            {
                System.Buffer.BlockCopy(b, 0, bs, offset, b.Length);
                offset += b.Length;
            }
            return bs;
        }

        public static Byte[] CmdToByteArray(bool AddOrDel, string name)
        {
            Byte[] bs1 = Encoding.Unicode.GetBytes(name);
            int count = bs1.Length;
            Byte[] bs2 = new byte[count+2];
            if (AddOrDel)
                bs2[0] = 1;
            else
                bs2[0] = 0;
            bs2[1] = Convert.ToByte(count); // OverflowException
            System.Buffer.BlockCopy(bs1, 0, bs2, 2, count);
            return bs2;
        }
    }
}
