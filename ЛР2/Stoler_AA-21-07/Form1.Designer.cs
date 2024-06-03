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
            setEventQuit();//закрываем консоль при закрытии формы
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.Start_Button = new System.Windows.Forms.Button();
            this.Stop_Button = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.MessageBox = new System.Windows.Forms.TextBox();
            this.Send_Button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // listBox1
            // 
            this.listBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 26;
            this.listBox1.Location = new System.Drawing.Point(16, 15);
            this.listBox1.Margin = new System.Windows.Forms.Padding(4);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(383, 446);
            this.listBox1.TabIndex = 0;
            // 
            // Start_Button
            // 
            this.Start_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Start_Button.Location = new System.Drawing.Point(496, 65);
            this.Start_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Start_Button.Name = "Start_Button";
            this.Start_Button.Size = new System.Drawing.Size(260, 65);
            this.Start_Button.TabIndex = 1;
            this.Start_Button.Text = "Старт / Добавить";
            this.Start_Button.UseVisualStyleBackColor = true;
            this.Start_Button.Click += new System.EventHandler(this.Start_Button_Click);
            // 
            // Stop_Button
            // 
            this.Stop_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Stop_Button.Location = new System.Drawing.Point(496, 398);
            this.Stop_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Stop_Button.Name = "Stop_Button";
            this.Stop_Button.Size = new System.Drawing.Size(260, 65);
            this.Stop_Button.TabIndex = 2;
            this.Stop_Button.Text = "Остановить 1 поток";
            this.Stop_Button.UseVisualStyleBackColor = true;
            this.Stop_Button.Click += new System.EventHandler(this.Stop_Button_Click);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F);
            this.textBox1.Location = new System.Drawing.Point(496, 21);
            this.textBox1.Margin = new System.Windows.Forms.Padding(4);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(259, 36);
            this.textBox1.TabIndex = 3;
            this.textBox1.Text = "1";
            // 
            // MessageBox
            // 
            this.MessageBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F);
            this.MessageBox.Location = new System.Drawing.Point(496, 159);
            this.MessageBox.Margin = new System.Windows.Forms.Padding(4);
            this.MessageBox.Multiline = true;
            this.MessageBox.Name = "MessageBox";
            this.MessageBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.MessageBox.Size = new System.Drawing.Size(259, 131);
            this.MessageBox.TabIndex = 4;
            // 
            // Send_Button
            // 
            this.Send_Button.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.Send_Button.Location = new System.Drawing.Point(496, 309);
            this.Send_Button.Margin = new System.Windows.Forms.Padding(4);
            this.Send_Button.Name = "Send_Button";
            this.Send_Button.Size = new System.Drawing.Size(260, 65);
            this.Send_Button.TabIndex = 5;
            this.Send_Button.Text = "Отправить";
            this.Send_Button.UseVisualStyleBackColor = true;
            this.Send_Button.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(811, 527);
            this.Controls.Add(this.Send_Button);
            this.Controls.Add(this.MessageBox);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.Stop_Button);
            this.Controls.Add(this.Start_Button);
            this.Controls.Add(this.listBox1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximumSize = new System.Drawing.Size(829, 574);
            this.MinimumSize = new System.Drawing.Size(829, 574);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button Start_Button;
        private System.Windows.Forms.Button Stop_Button;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox MessageBox;
        private System.Windows.Forms.Button Send_Button;
    }
}

