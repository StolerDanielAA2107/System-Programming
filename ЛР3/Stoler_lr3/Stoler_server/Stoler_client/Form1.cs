using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolTip;

namespace Stoler_client
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        [DllImport(@"..\..\..\..\..\Stoler_DLL.dll", CharSet = CharSet.Ansi)]
        public static extern bool Connect();

        [DllImport(@"..\..\..\..\..\Stoler_DLL.dll", CharSet = CharSet.Ansi)]
        public static extern void Disconnect();

        [DllImport(@"..\..\..\..\..\Stoler_DLL.dll", CharSet = CharSet.Ansi)]
        public static extern IntPtr sendCommand(int id, int commandId, StringBuilder message, int size = 0);


        private int GetThreadId()
        {
            return Convert.ToInt32(threadsListBox.SelectedItem.ToString().Split(" ")[1]);
        }

        private void UpdateListBox(string[] threadArray)
        {
            threadsListBox.Items.Insert(0, "All threads");
            for (var i = 0; i < threadArray.Length - 1; i++)
            {
                threadsListBox.Items.Insert(i + 1, "Session " + threadArray[i]);
            }
        }

        private void startButton_Click(object sender, EventArgs e)
        {
            if (countThreadsBox.Text == "")
            {
                MessageBox.Show("Select a count of threads");
                return;
            }

            IntPtr result = sendCommand(Convert.ToInt32(countThreadsBox.Text), 1, new StringBuilder(""));
            if (result != IntPtr.Zero)
            {
                string resultText = Marshal.PtrToStringAnsi(result);
                MessageBox.Show(resultText);
                Marshal.FreeCoTaskMem(result);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (!Connect())
            {
                timer2.Enabled = false;
                MessageBox.Show("Server is not found");
            }
            threadsListBox.Items.Clear();
        }

        private void sendMsgButton_Click(object sender, EventArgs e)
        {
            if (messageBox.Text == "")
            {
                MessageBox.Show("Enter a message");
                return;
            }
            if (threadsListBox.SelectedItem == null)
            {
                MessageBox.Show("Select a thread");
                return;
            }

            var threadId = -1;
            if (threadsListBox.SelectedIndex != 0)
            {
                threadId = GetThreadId();
            }

            var messageText = messageBox.Text;

            IntPtr result = sendCommand(threadId, 3, new StringBuilder(messageText), messageText.Length);
            if (result != IntPtr.Zero)
            {
                Marshal.FreeCoTaskMem(result);
            }
        }
        private void stopButton_Click(object sender, EventArgs e)
        {
            if (threadsListBox.SelectedItem == null)
            {
                MessageBox.Show("Select a thread");
                return;
            }

            var threadId = -1;
            if (threadsListBox.SelectedIndex != 0)
            {
                threadId = GetThreadId();
            }

            IntPtr result = sendCommand(threadId, 2, new StringBuilder(""));
            if (result != IntPtr.Zero)
            {
                Marshal.FreeCoTaskMem(result);
            }
            threadsListBox.SelectedIndex = threadsListBox.SelectedIndex - 1;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
        }

        private void updateThreadsListBox(object sender, EventArgs e)
        {
            IntPtr result = sendCommand(0, 4, new StringBuilder(""));
            if (result != IntPtr.Zero)
            {
                var cur = threadsListBox.SelectedIndex;
                threadsListBox.Items.Clear();
                string resultText = Marshal.PtrToStringAnsi(result);
                var threadsArray = resultText.Split('|');
                UpdateListBox(threadsArray);
                threadsListBox.SelectedIndex = cur;
            }
            else
            {
                threadsListBox.Items.Clear();
                threadsListBox.Items.Insert(0, "All threads");
            }
        }
    }
}