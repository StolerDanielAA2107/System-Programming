using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Stoler_AA_21_07
{
    public partial class Form1 : Form
    {
        bool connectionInitialized = false;
        CancellationTokenSource cancelTokenSource;

        const int maxResponseLength = 300;
        StringBuilder responseBuilder = new StringBuilder(maxResponseLength);

        DateTime lastActivityTime;

        public Form1()
        {
            InitializeComponent();
            Disconnect_Button.Enabled = false;
        }
               
        [DllImport("Stoler_AA-21-07_Library_MFC.dll", CharSet = CharSet.Ansi)] static extern int Connect(int port = 12345);
        [DllImport("Stoler_AA-21-07_Library_MFC.dll", CharSet = CharSet.Ansi)] static extern void Disconnect();
        [DllImport("Stoler_AA-21-07_Library_MFC.dll", CharSet = CharSet.Ansi)] static extern int sendMessageTo(int addr, StringBuilder data);
        [DllImport("Stoler_AA-21-07_Library_MFC.dll", CharSet = CharSet.Ansi)] static extern int ProcessMessages(int maxBufferLength, ref int mes_code, StringBuilder buffer);



        private void UpdateLastActivityTime()
        {
            lastActivityTime = DateTime.Now;
        }

        System.Windows.Forms.Timer timeoutTimer;

        private void StartTimeoutTimer()
        {
            timeoutTimer = new System.Windows.Forms.Timer();
            timeoutTimer.Interval = 5 * 60 * 1000;
            timeoutTimer.Tick += TimeoutTimer_Tick;
            timeoutTimer.Start();
        }

        private void TimeoutTimer_Tick(object sender, EventArgs e)
        {
            TimeSpan timeSinceLastActivity = DateTime.Now - lastActivityTime;
            if (timeSinceLastActivity.TotalMinutes >= 5)
            {
                Disconnect_Button_Click(sender, e);
            }
        }



        //выводим сообщение
        private void updateReceivedMessages(int responseLength)
        {
            Chat_Box.Invoke(
            (MethodInvoker)(() => {

                string[] Str= responseBuilder.ToString().Remove(responseLength, responseBuilder.ToString().Length - responseLength).Split('/');

                Chat_Box.AppendText(Str[0] + Environment.NewLine + Str[1] + Environment.NewLine);
                responseBuilder.Clear();
            }));

        }

        //обновляем список клиентов в сети
        private void updateThreadsList(int nlength, string idString)
        {
            Users_Box.Text = "";
            Users_Box.Items.Clear();
            Users_Box.Invoke((MethodInvoker)(() =>
            {
                if (nlength == 0) return;

                string[] ids = idString.Remove(nlength, idString.Length - nlength).Split('|');

                foreach (var id in ids)
                {
                    if (id != ID_label.Text)
                        Users_Box.Items.Add(id);
                }
                Users_Box.Items.Add("Всем");
            }));
        }
        
        //запрос на обновление данных
        private void TheSurvey()
        {
            cancelTokenSource = new CancellationTokenSource();
            CancellationToken token = cancelTokenSource.Token;

            Task getMessages = Task.Run
            (() => {
                while (!token.IsCancellationRequested)
                {
                    int messageCode = 0;
                    int length = ProcessMessages(maxResponseLength, ref messageCode, responseBuilder);
                    if (messageCode == 3)
                    {
                        updateReceivedMessages(length);
                    }
                    else if (messageCode == 4)
                    {
                        updateThreadsList(length, responseBuilder.ToString());
                    }
                    responseBuilder.Clear();
                    Thread.Sleep(1000);
                }
            }, token);

        }

        //подключение к серверу
        private void Connect_Button_Click(object sender, EventArgs e)
        {
            StartTimeoutTimer();
            int netId = Connect();

            ID_label.Text = netId.ToString();
            connectionInitialized = true;

            Connect_Button.Enabled = false;
            Disconnect_Button.Enabled = true;

            TheSurvey();
        }


        //отправка сообщения
        private void Send_Button_Click(object sender, EventArgs e)
        {
            UpdateLastActivityTime();

            if (connectionInitialized && Users_Box.SelectedItem != null)
            {
                
                int selectedValue = Convert.ToString(Users_Box.SelectedItem) == "Всем" ? -1 : Convert.ToInt32(Users_Box.SelectedItem);
                if (selectedValue == -1)
                    Chat_Box.AppendText($"Вы отправили всем пользователям:" + Environment.NewLine + MessageBox.Text + Environment.NewLine);
                else
                    Chat_Box.AppendText($"Вы отправили пользователю с ID {selectedValue}:" + Environment.NewLine + MessageBox.Text + Environment.NewLine);

                string text = $"Пользователь с ID: {ID_label.Text}/{MessageBox.Text}";
                sendMessageTo(selectedValue, new StringBuilder(text));
            }
        }


        //отключение
        private void Disconnect_Button_Click(object sender, EventArgs e)
        {
            Disconnect_Button.Enabled = false;
            Connect_Button.Enabled = true;

            Users_Box.Text = "";
            Users_Box.Items.Clear();
            timeoutTimer.Stop();

            Disconnect();

            cancelTokenSource.Cancel();
            connectionInitialized = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
        