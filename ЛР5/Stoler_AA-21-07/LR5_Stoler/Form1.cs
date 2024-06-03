using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Xml.Schema;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace LR5_Stoler
{
    public partial class Form1 : Form
    {

        const int maxResponseLength = 100;
        StringBuilder responseBuilder = new StringBuilder(maxResponseLength);
        CancellationTokenSource cancelTokenSource;
        bool connectionInitialized = false; 

        public Form1()
        {
            InitializeComponent();
        }
        private void updateReceivedMessages(string message) // обновление списка полученных сообщений
        {
            receivedMessageList.Invoke((MethodInvoker)(() =>
            {
                receivedMessageList.Items.Add(message);
                responseBuilder.Clear();
            }));
            
        }


        private void updateThreadsList(string parseString) // обовление списка потоков
        {
            threadsBox.Invoke((MethodInvoker)(() =>
            {
                threadsBox.Text = String.Empty;
                threadsBox.Items.Clear();

                if (parseString.Length == 0)
                    return;


                string[] ids = parseString.Split(',');
                

                threadsBox.Items.Add(new ComboBoxItem { Text = "main", Value =  0 });
                for (int i = 0; i < ids.Length - 1; ++i)
                {
                    ComboBoxItem item = new ComboBoxItem();
                    item.Text = ids[i];
                    item.Value = Int32.Parse(ids[i]);
                    threadsBox.Items.Add(item);
                }
                threadsBox.Items.Add(new ComboBoxItem { Text = "All threads", Value = 50 });
            }));

            
        }

       
        private void connectButton_Click(object sender, EventArgs e)
        {
            if (!connectionInitialized)
            {
                var initMessage = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_INIT);
                myId.Text = initMessage.header.to.ToString();
                connectionInitialized = true;
            }
            else
            {
                return;
            }

            cancelTokenSource = new CancellationTokenSource();
            CancellationToken token = cancelTokenSource.Token;

            // Task - абстракция асинхронной операции, используется 
            // для выполнения работы в фоновом потоке, что позволяет основному потоку продолжать выполнение без блокировки.
            Task getMessages = Task.Run(() =>   
                {
                    while (!token.IsCancellationRequested)
                    {
                        var m = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_GETDATA);
                        switch (m.header.type)
                        {
                            case MessageTypes.MT_DATA:
                                updateReceivedMessages(m.data);
                                break;
                            case MessageTypes.MT_CLIENTS_LIST:
                                updateThreadsList(m.data);
                                break;
                            default:
                                Thread.Sleep(100);
                                break;
                        }
                    }
                    Thread.Sleep(1000);
                },
                token
            );

        }

        private int getIdFromThreadBox()
        {
            if (threadsBox.SelectedItem != null)
                return Convert.ToInt32((threadsBox.SelectedItem as ComboBoxItem).Value);
            return 0;
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            cancelTokenSource.Cancel();
            
            connectionInitialized = false;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (connectionInitialized)
            {
                int id = getIdFromThreadBox();
                Message.send((MessageRecipients) id, MessageTypes.MT_DATA, $"[{myId.Text}] " + messageBox.Text);
            }
        }
    }
}
