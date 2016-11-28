using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClsMServer
{
    public class Blacklist
    {
        private HashSet<string> candidatelist = new HashSet<string>(),
                                defaultlist = new HashSet<string>(),
                                minus_set= new HashSet<string>(),
                                add_set = new HashSet<string>();
        public Blacklist(string filepath = null)
        {
            if(filepath != null && System.IO.File.Exists(filepath))
            {
                foreach (var s in System.IO.File.ReadAllLines(filepath))
                {
                    defaultlist.Add(s);
                    candidatelist.Add(s);
                }
            }
        }

        public HashSet<string> getCandidateList()
        {
            return candidatelist;
        }

        public bool Add(string s)
        {
            if (candidatelist.Contains(s))
                return false;
            candidatelist.Add(s);
            return true;
        }

        public void Enable(string[] ss)
        {
            lock(add_set)
            {
                foreach(string s in ss)
                    if (defaultlist.Contains(s))
                        minus_set.Remove(s);
                    else
                        add_set.Add(s);
            }
        }

        public void Disable(string[] ss)
        {
            lock (add_set)
            {
                foreach (string s in ss)
                    if (defaultlist.Contains(s))
                        minus_set.Add(s);
                    else
                        add_set.Remove(s);
            }
        }

        public bool Query(string s)
        {
            lock (add_set)
            {
                return defaultlist.Contains(s) ? !minus_set.Contains(s) : add_set.Contains(s);
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
