namespace RSA
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBoxP = new System.Windows.Forms.TextBox();
            this.labelP = new System.Windows.Forms.Label();
            this.labelQ = new System.Windows.Forms.Label();
            this.textBoxQ = new System.Windows.Forms.TextBox();
            this.generatePQButton = new System.Windows.Forms.Button();
            this.groupBoxRSA = new System.Windows.Forms.GroupBox();
            this.numericL = new System.Windows.Forms.NumericUpDown();
            this.labelL = new System.Windows.Forms.Label();
            this.numericK = new System.Windows.Forms.NumericUpDown();
            this.labelK = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.cipherRTB = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textRTB = new System.Windows.Forms.RichTextBox();
            this.buttonShow = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBoxRSA.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericL)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericK)).BeginInit();
            this.SuspendLayout();
            // 
            // textBoxP
            // 
            this.textBoxP.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxP.Location = new System.Drawing.Point(40, 9);
            this.textBoxP.Name = "textBoxP";
            this.textBoxP.Size = new System.Drawing.Size(179, 22);
            this.textBoxP.TabIndex = 0;
            this.textBoxP.TextChanged += new System.EventHandler(this.textBoxP_TextChanged);
            // 
            // labelP
            // 
            this.labelP.AutoSize = true;
            this.labelP.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelP.Location = new System.Drawing.Point(12, 12);
            this.labelP.Name = "labelP";
            this.labelP.Size = new System.Drawing.Size(22, 16);
            this.labelP.TabIndex = 1;
            this.labelP.Text = "P:";
            // 
            // labelQ
            // 
            this.labelQ.AutoSize = true;
            this.labelQ.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelQ.Location = new System.Drawing.Point(12, 40);
            this.labelQ.Name = "labelQ";
            this.labelQ.Size = new System.Drawing.Size(23, 16);
            this.labelQ.TabIndex = 3;
            this.labelQ.Text = "Q:";
            // 
            // textBoxQ
            // 
            this.textBoxQ.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxQ.Location = new System.Drawing.Point(40, 37);
            this.textBoxQ.Name = "textBoxQ";
            this.textBoxQ.Size = new System.Drawing.Size(179, 22);
            this.textBoxQ.TabIndex = 2;
            this.textBoxQ.TextChanged += new System.EventHandler(this.textBoxQ_TextChanged);
            // 
            // generatePQButton
            // 
            this.generatePQButton.Location = new System.Drawing.Point(250, 9);
            this.generatePQButton.Name = "generatePQButton";
            this.generatePQButton.Size = new System.Drawing.Size(121, 50);
            this.generatePQButton.TabIndex = 4;
            this.generatePQButton.Text = "Generate P and Q (might be slow)";
            this.generatePQButton.UseVisualStyleBackColor = true;
            this.generatePQButton.Click += new System.EventHandler(this.generatePQButton_Click);
            // 
            // groupBoxRSA
            // 
            this.groupBoxRSA.Controls.Add(this.numericL);
            this.groupBoxRSA.Controls.Add(this.labelL);
            this.groupBoxRSA.Controls.Add(this.numericK);
            this.groupBoxRSA.Controls.Add(this.labelK);
            this.groupBoxRSA.Controls.Add(this.button2);
            this.groupBoxRSA.Controls.Add(this.button1);
            this.groupBoxRSA.Controls.Add(this.label2);
            this.groupBoxRSA.Controls.Add(this.cipherRTB);
            this.groupBoxRSA.Controls.Add(this.label1);
            this.groupBoxRSA.Controls.Add(this.textRTB);
            this.groupBoxRSA.Location = new System.Drawing.Point(12, 83);
            this.groupBoxRSA.Name = "groupBoxRSA";
            this.groupBoxRSA.Size = new System.Drawing.Size(358, 295);
            this.groupBoxRSA.TabIndex = 5;
            this.groupBoxRSA.TabStop = false;
            this.groupBoxRSA.Text = "RSA";
            // 
            // numericL
            // 
            this.numericL.Location = new System.Drawing.Point(146, 28);
            this.numericL.Maximum = new decimal(new int[] {
            12,
            0,
            0,
            0});
            this.numericL.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericL.Name = "numericL";
            this.numericL.Size = new System.Drawing.Size(70, 20);
            this.numericL.TabIndex = 9;
            this.numericL.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.numericL.ValueChanged += new System.EventHandler(this.numericL_ValueChanged);
            // 
            // labelL
            // 
            this.labelL.AutoSize = true;
            this.labelL.Location = new System.Drawing.Point(124, 30);
            this.labelL.Name = "labelL";
            this.labelL.Size = new System.Drawing.Size(16, 13);
            this.labelL.TabIndex = 8;
            this.labelL.Text = "L:";
            // 
            // numericK
            // 
            this.numericK.Location = new System.Drawing.Point(32, 28);
            this.numericK.Maximum = new decimal(new int[] {
            12,
            0,
            0,
            0});
            this.numericK.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericK.Name = "numericK";
            this.numericK.Size = new System.Drawing.Size(70, 20);
            this.numericK.TabIndex = 7;
            this.numericK.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericK.ValueChanged += new System.EventHandler(this.numericK_ValueChanged);
            // 
            // labelK
            // 
            this.labelK.AutoSize = true;
            this.labelK.Location = new System.Drawing.Point(9, 30);
            this.labelK.Name = "labelK";
            this.labelK.Size = new System.Drawing.Size(17, 13);
            this.labelK.TabIndex = 6;
            this.labelK.Text = "K:";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(195, 237);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(156, 52);
            this.button2.TabIndex = 5;
            this.button2.Text = "Decrypt";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(9, 237);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(156, 52);
            this.button1.TabIndex = 4;
            this.button1.Text = "Encrypt";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 155);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Ciphertext (text):";
            // 
            // cipherRTB
            // 
            this.cipherRTB.Location = new System.Drawing.Point(8, 171);
            this.cipherRTB.Name = "cipherRTB";
            this.cipherRTB.ReadOnly = true;
            this.cipherRTB.Size = new System.Drawing.Size(345, 62);
            this.cipherRTB.TabIndex = 2;
            this.cipherRTB.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(86, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Text (ciphertext):";
            // 
            // textRTB
            // 
            this.textRTB.Location = new System.Drawing.Point(9, 81);
            this.textRTB.Name = "textRTB";
            this.textRTB.Size = new System.Drawing.Size(345, 62);
            this.textRTB.TabIndex = 0;
            this.textRTB.Text = "";
            // 
            // buttonShow
            // 
            this.buttonShow.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonShow.Location = new System.Drawing.Point(15, 384);
            this.buttonShow.Name = "buttonShow";
            this.buttonShow.Size = new System.Drawing.Size(355, 23);
            this.buttonShow.TabIndex = 6;
            this.buttonShow.Text = "Show generated public key and private key";
            this.buttonShow.UseVisualStyleBackColor = true;
            this.buttonShow.Click += new System.EventHandler(this.buttonShow_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 434);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(183, 39);
            this.label3.TabIndex = 7;
            this.label3.Text = "RSA Encryption & Decryption with GUI\nMatica Florian-Alexandru (935)\nDaineanu Alex" +
    "andru (933)";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 482);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.buttonShow);
            this.Controls.Add(this.groupBoxRSA);
            this.Controls.Add(this.generatePQButton);
            this.Controls.Add(this.labelQ);
            this.Controls.Add(this.textBoxQ);
            this.Controls.Add(this.labelP);
            this.Controls.Add(this.textBoxP);
            this.Name = "Form1";
            this.Text = "RSA";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBoxRSA.ResumeLayout(false);
            this.groupBoxRSA.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericL)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericK)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxP;
        private System.Windows.Forms.Label labelP;
        private System.Windows.Forms.Label labelQ;
        private System.Windows.Forms.TextBox textBoxQ;
        private System.Windows.Forms.Button generatePQButton;
        private System.Windows.Forms.GroupBox groupBoxRSA;
        private System.Windows.Forms.Button buttonShow;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RichTextBox cipherRTB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RichTextBox textRTB;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericL;
        private System.Windows.Forms.Label labelL;
        private System.Windows.Forms.NumericUpDown numericK;
        private System.Windows.Forms.Label labelK;
    }
}

