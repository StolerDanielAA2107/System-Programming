namespace Stoler_client
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            startButton = new Button();
            stopButton = new Button();
            countThreadsBox = new TextBox();
            threadsListBox = new ListBox();
            label1 = new Label();
            label2 = new Label();
            messageBox = new TextBox();
            sendMsgButton = new Button();
            timer2 = new System.Windows.Forms.Timer(components);
            SuspendLayout();
            // 
            // startButton
            // 
            startButton.Location = new Point(26, 96);
            startButton.Margin = new Padding(3, 4, 3, 4);
            startButton.Name = "startButton";
            startButton.Size = new Size(301, 34);
            startButton.TabIndex = 0;
            startButton.Text = "Старт";
            startButton.UseVisualStyleBackColor = true;
            startButton.Click += startButton_Click;
            // 
            // stopButton
            // 
            stopButton.Location = new Point(391, 240);
            stopButton.Margin = new Padding(3, 4, 3, 4);
            stopButton.Name = "stopButton";
            stopButton.Size = new Size(301, 32);
            stopButton.TabIndex = 1;
            stopButton.Text = "Стоп";
            stopButton.UseVisualStyleBackColor = true;
            stopButton.Click += stopButton_Click;
            // 
            // countThreadsBox
            // 
            countThreadsBox.Location = new Point(26, 49);
            countThreadsBox.Margin = new Padding(3, 4, 3, 4);
            countThreadsBox.Name = "countThreadsBox";
            countThreadsBox.Size = new Size(302, 27);
            countThreadsBox.TabIndex = 2;
            // 
            // threadsListBox
            // 
            threadsListBox.FormattingEnabled = true;
            threadsListBox.ItemHeight = 20;
            threadsListBox.Location = new Point(391, 13);
            threadsListBox.Margin = new Padding(3, 4, 3, 4);
            threadsListBox.Name = "threadsListBox";
            threadsListBox.Size = new Size(301, 204);
            threadsListBox.TabIndex = 4;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(65, 13);
            label1.Name = "label1";
            label1.Size = new Size(224, 20);
            label1.TabIndex = 5;
            label1.Text = "Введите N количество потоков";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(68, 151);
            label2.Name = "label2";
            label2.Size = new Size(221, 20);
            label2.TabIndex = 6;
            label2.Text = "Введите текстовое сообщение";
            // 
            // messageBox
            // 
            messageBox.Location = new Point(25, 190);
            messageBox.Margin = new Padding(3, 4, 3, 4);
            messageBox.Name = "messageBox";
            messageBox.Size = new Size(302, 27);
            messageBox.TabIndex = 7;
            // 
            // sendMsgButton
            // 
            sendMsgButton.Location = new Point(27, 240);
            sendMsgButton.Margin = new Padding(3, 4, 3, 4);
            sendMsgButton.Name = "sendMsgButton";
            sendMsgButton.Size = new Size(301, 32);
            sendMsgButton.TabIndex = 8;
            sendMsgButton.Text = "Отправить";
            sendMsgButton.UseVisualStyleBackColor = true;
            sendMsgButton.Click += sendMsgButton_Click;
            // 
            // timer2
            // 
            timer2.Enabled = true;
            timer2.Interval = 2000;
            timer2.Tick += updateThreadsListBox;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(728, 307);
            Controls.Add(stopButton);
            Controls.Add(startButton);
            Controls.Add(sendMsgButton);
            Controls.Add(messageBox);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(threadsListBox);
            Controls.Add(countThreadsBox);
            Margin = new Padding(3, 4, 3, 4);
            MaximizeBox = false;
            MdiChildrenMinimizedAnchorBottom = false;
            MinimizeBox = false;
            Name = "Form1";
            Text = "Столер АА-21-07";
            FormClosing += Form1_FormClosing;
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button startButton;
        private Button stopButton;
        private TextBox countThreadsBox;
        private ListBox threadsListBox;
        private Label label1;
        private Label label2;
        private TextBox messageBox;
        private Button sendMsgButton;
        private System.Windows.Forms.Timer timer2;
    }
}