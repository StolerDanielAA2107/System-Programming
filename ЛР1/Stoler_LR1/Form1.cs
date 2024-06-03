using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace Stoler_LR1
{
    public partial class Form1 : Form
    {
        Process ConsoleAppProcess = null;

        EventWaitHandle event_start = new EventWaitHandle(false, EventResetMode.AutoReset, "event_start");
        EventWaitHandle event_stop = new EventWaitHandle(false, EventResetMode.AutoReset, "event_stop");
        EventWaitHandle event_close = new EventWaitHandle(false, EventResetMode.AutoReset, "event_close");
        EventWaitHandle event_confirm = new EventWaitHandle(false, EventResetMode.AutoReset, "event_confirm");

        int thread_count;

        public Form1()
        {
            InitializeComponent();
        }
        private Boolean consoleIsOpen()
        {
            if (ConsoleAppProcess == null || ConsoleAppProcess.HasExited)
            {
                listBox1.Items.Clear();
                return false;
            }
            return true;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (ConsoleAppProcess == null || ConsoleAppProcess.HasExited)
            {
                thread_count = 0;
                listBox1.Items.Clear();
                ConsoleAppProcess = Process.Start("D:/6 семестр/Системное программирование/Stoler_LR1/x64/Debug/LR1.exe");
                listBox1.Items.Add("Все потоки");
                listBox1.Items.Add("Главный поток");
            } else {
                for (int i = 0; i < (int)numericUpDown1.Value; ++i)
                {
                    event_start.Set();
                    event_confirm.WaitOne();
                    thread_count++;
                    listBox1.Items.Add("Поток - " + thread_count);
                }
            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            if (!ConsoleAppProcess.HasExited)
            {                
                if (listBox1.Items.Count - 1 == 0)
                {
                    event_stop.Set();
                    event_confirm.WaitOne();
                    listBox1.Items.RemoveAt(0);
                    thread_count--;
                } else {
                    event_stop.Set();
                    event_confirm.WaitOne();
                    listBox1.Items.RemoveAt(listBox1.Items.Count - 1);
                    thread_count--;
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (consoleIsOpen())
            {
                event_close.Set();
            }
        }
    }
}
