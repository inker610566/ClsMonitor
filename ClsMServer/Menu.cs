using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace ClsMServer
{
    class MenuFactory
    {
        private ContextMenuStrip cms = null;

        public static ContextMenuStrip Produce(
            Action ClickAdd,
            Action ClickEnable,
            Action ClickDisable,
            Action ClickLock,
            Action ClickUnLock
        )
        {
            String[] OptionName = new String[] {
                "Add", "Enable", "Disable"
            };
            Action[] handlers = new Action[] {
                ClickAdd,
                ClickEnable,
                ClickDisable
            };
            ContextMenuStrip cms = new ContextMenuStrip();
            Debug.Assert(OptionName.Length == handlers.Length);
            cms.Items.Add(
            new ToolStripButton("Add", null, new EventHandler(delegate (Object o, EventArgs a) {
                ClickAdd();
            })));
            cms.Items.Add(
            new ToolStripButton("Enable", null, new EventHandler(delegate (Object o, EventArgs a) {
                ClickEnable();
            })));
            cms.Items.Add(
            new ToolStripButton("Disable", null, new EventHandler(delegate (Object o, EventArgs a) {
                ClickDisable();
            })));
            cms.Items.Add(
            new ToolStripButton("Lock", null, new EventHandler(delegate (Object o, EventArgs a) {
                ClickLock();
            })));
            cms.Items.Add(
            new ToolStripButton("UnLock", null, new EventHandler(delegate (Object o, EventArgs a) {
                ClickUnLock();
            })));
            return cms;
        }

    }
}
