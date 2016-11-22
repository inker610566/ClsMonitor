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
        private HashSet<string> list = new HashSet<string>();
        private ListView listview;
        public BlackList(ListView listview)
        {
            this.listview = listview;
        }

        public bool Add(string s)
        {
            if (list.Contains(s))
                return false;
            list.Add(s);
            listview.Items.Add(s);
            return true;
        }

        // Presume to be add
        public Byte[] ToByteArray()
        {
            return ConcateByteArray((list.Select((s) => CmdToByteArray(true, s)).ToArray()));
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
