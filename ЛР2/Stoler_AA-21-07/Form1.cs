using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Stoler_AA_21_07
{
    public partial class Form1 : Form
    {

        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void createEvents();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void setEventStart();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void waitEventConfirm();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void setEventQuit();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void setEventStop();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll")] public static extern void createMessage(string message, int threadNumber);


        Process childProcess = null;
        EventWaitHandle eventStart = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle eventConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle eventClose = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProcKushch");
        EventWaitHandle eventExit = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitProcKushch");

        int threadsCounter = 0;

        public Form1()
        {
            InitializeComponent();
            createEvents();//создаём дескрипторы
        }

        private void updateList(string name)
        {
            listBox1.Items.Add(name);
        }

        private void Start_Button_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)//проверяем не была ли закрыта консоль или была ли она открыта вообще
            {
                threadsCounter = 0;
                listBox1.Items.Clear();
                childProcess = Process.Start("Stoler_AA-21-07_ConsoleApplication_MFC.exe");
                waitEventConfirm();//ожидаем ответа
                updateList("Главный поток");
                updateList("Побочные потоки:");
                threadsCounter++;
            }
            else
            {
                int n = 0;
                try
                {
                    n = Int32.Parse(textBox1.Text);
                }
                catch
                { }

                for (int i = 0; i < n; ++i)
                {
                    setEventStart();//активируем дескрипторор создание потока
                    waitEventConfirm();//ожидаем ответа
                    updateList((threadsCounter++).ToString());//обновляем список
                }
            }
        }

        private void Stop_Button_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                listBox1.Items.Clear();
            }
            else
            {
                setEventStop();//активируем дескрипторор(событие) удаления потока
                waitEventConfirm();//ожидаем ответа
                listBox1.Items.RemoveAt(threadsCounter--);// обновляем список
                if (threadsCounter == 0)
                {
                    listBox1.Items.RemoveAt(0);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string text = MessageBox.Text;
            var threadNumber = listBox1.SelectedIndex;
            if (text.Length == 0 || threadNumber == -1 || threadNumber == 0) 
                return;
            else if (threadNumber == 1)
                threadNumber = -1;
            else
                threadNumber -= 2;
            createMessage(text, threadNumber);//запускаем процесс создания виртуального адресного пространство, с дальнейшим выводом
            //в консоль и в файл
        }
    }
}
