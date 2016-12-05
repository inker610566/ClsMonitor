using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ScreenLocker
{
    public partial class ScreenLockForm : Form
    {
        private string UnLockKeys = "WINDOWSUPDATE";
        private int UnLockMatch;
        private UserActivityHook hook;
        public ScreenLockForm()
        {
            InitKeyChecker();
            HookKeys();
            InitializeComponent();
            SetFullScreen();
        }

        private void InitKeyChecker()
        {
            UnLockMatch = 0;
        }

        private void KeyDownHandler(object sender, KeyEventArgs e)
        {
            if(!(65 <= e.KeyValue && e.KeyValue < 65+26))
            {
                e.Handled = true;
            }
        }

        private void KeyUpHandler(object sender, KeyEventArgs e)
        {
            if(!(65 <= e.KeyValue && e.KeyValue < 65+26))
            {
                e.Handled = true;
            }
        }

        private void KeyPressHandler(object sender, KeyPressEventArgs e)
        {
        }

        private void HookKeys()
        {
            hook = new UserActivityHook();
            hook.KeyDown += new KeyEventHandler(KeyDownHandler);
            hook.KeyPress += new KeyPressEventHandler(KeyPressHandler);
            hook.KeyUp += new KeyEventHandler(KeyUpHandler);
            hook.Start(false, true);
        }

        private void SetFullScreen()
        {
            int minx, miny, maxx, maxy;
            minx = miny = int.MaxValue;
            maxx = maxy = int.MinValue;

            foreach (Screen screen in Screen.AllScreens)
            {
                var bounds = screen.Bounds;
                minx = Math.Min(minx, bounds.X);
                miny = Math.Min(miny, bounds.Y);
                maxx = Math.Max(maxx, bounds.Right);
                maxy = Math.Max(maxy, bounds.Bottom);
            }

            Rectangle tempRect = new Rectangle(0, 0, maxx, maxy);
            this.DesktopBounds = tempRect;
        }

        private const int CS_NOCLOSE = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CS_NOCLOSE;
                return cp;
            }
        }

        private void ScreenLockForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == Convert.ToInt32(UnLockKeys[UnLockMatch]))
            {
                UnLockMatch++;
                if (UnLockMatch == UnLockKeys.Length)
                    this.Close();
            }
            else
                UnLockMatch = 0;
        }
    }
}
