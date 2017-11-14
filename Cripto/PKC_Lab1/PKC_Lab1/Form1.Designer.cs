namespace PKC_Lab1
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
            this.components = new System.ComponentModel.Container();
            this.alphabetGB = new System.Windows.Forms.GroupBox();
            this.tb1 = new System.Windows.Forms.TextBox();
            this.tb2 = new System.Windows.Forms.TextBox();
            this.tb3 = new System.Windows.Forms.TextBox();
            this.tb4 = new System.Windows.Forms.TextBox();
            this.keyGB = new System.Windows.Forms.GroupBox();
            this.encryptButton = new System.Windows.Forms.Button();
            this.decryptButton = new System.Windows.Forms.Button();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.inputTB = new System.Windows.Forms.RichTextBox();
            this.resultTB = new System.Windows.Forms.RichTextBox();
            this.errorProvider2 = new System.Windows.Forms.ErrorProvider(this.components);
            this.keyGB.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider2)).BeginInit();
            this.SuspendLayout();
            // 
            // alphabetGB
            // 
            this.alphabetGB.Location = new System.Drawing.Point(13, 13);
            this.alphabetGB.Name = "alphabetGB";
            this.alphabetGB.Size = new System.Drawing.Size(496, 183);
            this.alphabetGB.TabIndex = 0;
            this.alphabetGB.TabStop = false;
            this.alphabetGB.Text = "Alphabet";
            // 
            // tb1
            // 
            this.tb1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb1.Location = new System.Drawing.Point(21, 22);
            this.tb1.Name = "tb1";
            this.tb1.Size = new System.Drawing.Size(48, 29);
            this.tb1.TabIndex = 1;
            this.tb1.TextChanged += new System.EventHandler(this.validateKey);
            // 
            // tb2
            // 
            this.tb2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb2.Location = new System.Drawing.Point(89, 22);
            this.tb2.Name = "tb2";
            this.tb2.Size = new System.Drawing.Size(47, 29);
            this.tb2.TabIndex = 2;
            this.tb2.TextChanged += new System.EventHandler(this.validateKey);
            // 
            // tb3
            // 
            this.tb3.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb3.Location = new System.Drawing.Point(21, 57);
            this.tb3.Name = "tb3";
            this.tb3.Size = new System.Drawing.Size(48, 29);
            this.tb3.TabIndex = 3;
            this.tb3.TextChanged += new System.EventHandler(this.validateKey);
            // 
            // tb4
            // 
            this.tb4.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb4.Location = new System.Drawing.Point(88, 57);
            this.tb4.Name = "tb4";
            this.tb4.Size = new System.Drawing.Size(48, 29);
            this.tb4.TabIndex = 4;
            this.tb4.TextChanged += new System.EventHandler(this.validateKey);
            // 
            // keyGB
            // 
            this.keyGB.Controls.Add(this.tb1);
            this.keyGB.Controls.Add(this.tb4);
            this.keyGB.Controls.Add(this.tb2);
            this.keyGB.Controls.Add(this.tb3);
            this.keyGB.Location = new System.Drawing.Point(13, 202);
            this.keyGB.Name = "keyGB";
            this.keyGB.Size = new System.Drawing.Size(154, 100);
            this.keyGB.TabIndex = 5;
            this.keyGB.TabStop = false;
            this.keyGB.Text = "Key Matrix (m=2)";
            // 
            // encryptButton
            // 
            this.encryptButton.Location = new System.Drawing.Point(283, 202);
            this.encryptButton.Name = "encryptButton";
            this.encryptButton.Size = new System.Drawing.Size(110, 100);
            this.encryptButton.TabIndex = 6;
            this.encryptButton.Text = "ENCRYPT";
            this.encryptButton.UseVisualStyleBackColor = true;
            this.encryptButton.Click += new System.EventHandler(this.encryptButton_Click);
            // 
            // decryptButton
            // 
            this.decryptButton.Location = new System.Drawing.Point(399, 202);
            this.decryptButton.Name = "decryptButton";
            this.decryptButton.Size = new System.Drawing.Size(110, 100);
            this.decryptButton.TabIndex = 7;
            this.decryptButton.Text = "DECRYPT";
            this.decryptButton.UseVisualStyleBackColor = true;
            this.decryptButton.Click += new System.EventHandler(this.decryptButton_Click);
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // inputTB
            // 
            this.inputTB.Location = new System.Drawing.Point(12, 325);
            this.inputTB.Name = "inputTB";
            this.inputTB.Size = new System.Drawing.Size(496, 126);
            this.inputTB.TabIndex = 8;
            this.inputTB.Text = "";
            this.inputTB.TextChanged += new System.EventHandler(this.inputTB_TextChanged);
            // 
            // resultTB
            // 
            this.resultTB.Location = new System.Drawing.Point(12, 457);
            this.resultTB.Name = "resultTB";
            this.resultTB.ReadOnly = true;
            this.resultTB.Size = new System.Drawing.Size(496, 126);
            this.resultTB.TabIndex = 9;
            this.resultTB.Text = "";
            // 
            // errorProvider2
            // 
            this.errorProvider2.ContainerControl = this;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 595);
            this.Controls.Add(this.resultTB);
            this.Controls.Add(this.inputTB);
            this.Controls.Add(this.decryptButton);
            this.Controls.Add(this.encryptButton);
            this.Controls.Add(this.keyGB);
            this.Controls.Add(this.alphabetGB);
            this.Name = "Form1";
            this.Text = "The Hill Cipher";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.keyGB.ResumeLayout(false);
            this.keyGB.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox alphabetGB;
        private System.Windows.Forms.TextBox tb1;
        private System.Windows.Forms.TextBox tb2;
        private System.Windows.Forms.TextBox tb3;
        private System.Windows.Forms.TextBox tb4;
        private System.Windows.Forms.GroupBox keyGB;
        private System.Windows.Forms.Button encryptButton;
        private System.Windows.Forms.Button decryptButton;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.RichTextBox resultTB;
        private System.Windows.Forms.RichTextBox inputTB;
        private System.Windows.Forms.ErrorProvider errorProvider2;
    }
}

