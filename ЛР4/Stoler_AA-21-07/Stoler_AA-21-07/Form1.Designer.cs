namespace Stoler_AA_21_07
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            //setEventQuit();//закрываем консоль при закрытии формы
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.Connect_Button = new System.Windows.Forms.Button();
            this.Disconnect_Button = new System.Windows.Forms.Button();
            this.MessageBox = new System.Windows.Forms.TextBox();
            this.Send_Button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.ID_label = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.Users_Box = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.Chat_Box = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // Connect_Button
            // 
            this.Connect_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Connect_Button.Location = new System.Drawing.Point(413, 13);
            this.Connect_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Connect_Button.Name = "Connect_Button";
            this.Connect_Button.Size = new System.Drawing.Size(131, 46);
            this.Connect_Button.TabIndex = 1;
            this.Connect_Button.Text = "Connect";
            this.Connect_Button.UseVisualStyleBackColor = true;
            this.Connect_Button.Click += new System.EventHandler(this.Connect_Button_Click);
            // 
            // Disconnect_Button
            // 
            this.Disconnect_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Disconnect_Button.Location = new System.Drawing.Point(552, 13);
            this.Disconnect_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Disconnect_Button.Name = "Disconnect_Button";
            this.Disconnect_Button.Size = new System.Drawing.Size(154, 46);
            this.Disconnect_Button.TabIndex = 2;
            this.Disconnect_Button.Text = "Disconnect";
            this.Disconnect_Button.UseVisualStyleBackColor = true;
            this.Disconnect_Button.Click += new System.EventHandler(this.Disconnect_Button_Click);
            // 
            // MessageBox
            // 
            this.MessageBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.MessageBox.Location = new System.Drawing.Point(413, 177);
            this.MessageBox.Margin = new System.Windows.Forms.Padding(4);
            this.MessageBox.Multiline = true;
            this.MessageBox.Name = "MessageBox";
            this.MessageBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.MessageBox.Size = new System.Drawing.Size(269, 126);
            this.MessageBox.TabIndex = 4;
            // 
            // Send_Button
            // 
            this.Send_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Send_Button.Location = new System.Drawing.Point(451, 361);
            this.Send_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Send_Button.Name = "Send_Button";
            this.Send_Button.Size = new System.Drawing.Size(174, 51);
            this.Send_Button.TabIndex = 5;
            this.Send_Button.Text = "Отправить";
            this.Send_Button.UseVisualStyleBackColor = true;
            this.Send_Button.Click += new System.EventHandler(this.Send_Button_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.label1.Location = new System.Drawing.Point(417, 90);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(189, 26);
            this.label1.TabIndex = 6;
            this.label1.Text = "ID пользователя:";
            // 
            // ID_label
            // 
            this.ID_label.AutoSize = true;
            this.ID_label.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.ID_label.Location = new System.Drawing.Point(625, 90);
            this.ID_label.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.ID_label.Name = "ID_label";
            this.ID_label.Size = new System.Drawing.Size(0, 26);
            this.ID_label.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.label2.Location = new System.Drawing.Point(417, 147);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(195, 26);
            this.label2.TabIndex = 8;
            this.label2.Text = "Ваше сообщение:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Bold);
            this.label3.Location = new System.Drawing.Point(28, 15);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 26);
            this.label3.TabIndex = 9;
            this.label3.Text = "Чат:";
            // 
            // Users_Box
            // 
            this.Users_Box.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Users_Box.FormattingEnabled = true;
            this.Users_Box.Location = new System.Drawing.Point(530, 318);
            this.Users_Box.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Users_Box.Name = "Users_Box";
            this.Users_Box.Size = new System.Drawing.Size(140, 34);
            this.Users_Box.TabIndex = 10;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.label4.Location = new System.Drawing.Point(408, 321);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(115, 26);
            this.label4.TabIndex = 11;
            this.label4.Text = "Адресант:";
            // 
            // Chat_Box
            // 
            this.Chat_Box.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Chat_Box.Location = new System.Drawing.Point(28, 54);
            this.Chat_Box.Margin = new System.Windows.Forms.Padding(4);
            this.Chat_Box.Multiline = true;
            this.Chat_Box.Name = "Chat_Box";
            this.Chat_Box.ReadOnly = true;
            this.Chat_Box.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.Chat_Box.Size = new System.Drawing.Size(357, 356);
            this.Chat_Box.TabIndex = 12;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(749, 436);
            this.Controls.Add(this.Chat_Box);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.Users_Box);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ID_label);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Send_Button);
            this.Controls.Add(this.MessageBox);
            this.Controls.Add(this.Disconnect_Button);
            this.Controls.Add(this.Connect_Button);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Stoler_AA-21-07";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button Connect_Button;
        private System.Windows.Forms.Button Disconnect_Button;
        private System.Windows.Forms.TextBox MessageBox;
        private System.Windows.Forms.Button Send_Button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label ID_label;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox Users_Box;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox Chat_Box;
    }
}

