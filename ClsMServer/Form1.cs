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
        public Form1()
        {
            InitializeComponent();
            addForm = new AddForm();

            listView1.ContextMenuStrip = ClsMServer.MenuFactory.Produce(
                ()=>
                {
                    DialogResult r = addForm.ShowDialog();
                    if(r == DialogResult.OK)
                    {
                        listView1.Items.Add(addForm.AddString);
                    }
                },
                ()=>
                {
                    //NewProcessTextBox.Text = listView1.SelectedItems[0].Text;
                },
                ()=>
                {
                });
        }
    }
}
