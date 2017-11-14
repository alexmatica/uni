using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PKC_Lab1
{
    public partial class Form1 : Form
    {
        List<Button> alphabet = new List<Button>();
        string defaultAlphabet = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string msgText = " abcdefghijklmnopqrstuvwxyz";
        bool[] lettersEnabled = new bool[27];
        int[,] encKey = new int[2,2];
        int usedAlphabet = 27;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < 27; i++)
                lettersEnabled[i] = true;

            int topStart = alphabetGB.Top + 3;
            int leftStart = alphabetGB.Left;
            int line = 0, column = 0;
            int bWidth = alphabetGB.Width / 9 - 3;
            int bHeight = alphabetGB.Height / 3 - 10;

            for (int i=0; i<27; i++)
            {
                if (i == 9 || i == 18){
                    line += 1;
                    column = 0;
                }
                Button b = new Button();
                b.Text = defaultAlphabet[i].ToString();
                b.Font = new Font(b.Font, FontStyle.Bold);
                b.BackColor = Color.Green;
                b.Top = topStart + line * bHeight;
                b.Left = leftStart + column * bWidth;
                column += 1;
                b.Width = bWidth;
                b.Height = bHeight;
                b.Click += new EventHandler(this.customHandler);
                alphabet.Add(b);
                this.alphabetGB.Controls.Add(b);
            }

            errorProvider1.SetError(keyGB, "Key cannot contain empty values");
            encryptButton.Enabled = decryptButton.Enabled = false;

        }

        private void customHandler(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            int letterIndex = defaultAlphabet.IndexOf(b.Text);
            tb1.Text = tb2.Text = tb3.Text = tb4.Text = "";
            inputTB.Clear();
            resultTB.Clear();
            errorProvider2.Clear();
            if (b.BackColor == Color.Green)
            {
                b.BackColor = Color.Gray;
                lettersEnabled[letterIndex] = false;
                usedAlphabet--;
            }
            else
            {
                lettersEnabled[letterIndex] = true;
                b.BackColor = Color.Green;
                usedAlphabet++;
            }
        }

        private void validateKey(object sender, EventArgs e)
        {
            TextBox crtTb = (TextBox)sender;

            if (crtTb.Text == "")
            {
                errorProvider1.SetError(keyGB,"Values must not be empty!");
                encryptButton.Enabled = decryptButton.Enabled = false;
                return;
            }

            int keyVal;
            if (Int32.TryParse(crtTb.Text,out keyVal))
            {
                
                errorProvider1.Clear();

                if (keyVal < 0 || keyVal > 26 || lettersEnabled[keyVal] == false)
                {
                    errorProvider1.SetError(keyGB, "Key must be in range 0-26 and it must obey the chosen alphabet!");
                    encryptButton.Enabled = decryptButton.Enabled = false;
                    return;
                }

                switch (crtTb.Name)
                {
                    case "tb1":
                        encKey[0,0] = keyVal;
                        break;
                    case "tb2":
                        encKey[0, 1] = keyVal;
                        break;
                    case "tb3":
                        encKey[1, 0] = keyVal;
                        break;
                    case "tb4":
                        encKey[1, 1] = keyVal;
                        break;
                }
            }
            else
            {
                errorProvider1.SetError(keyGB, "Values in the matrix must be integers.");
                encryptButton.Enabled = decryptButton.Enabled = false;
            }

            if (checkKeyValues())
            {
                encryptButton.Enabled = decryptButton.Enabled = true;
                errorProvider1.Clear();
            }
        }

        private bool checkKeyValues()
        {
            if (tb1.Text != "" && tb2.Text != "" && tb3.Text != "" && tb4.Text != "")
            {
                string[] texts = { tb1.Text, tb2.Text, tb3.Text, tb4.Text };
                foreach(string s in texts)
                {
                    int k;
                    if (Int32.TryParse(s, out k)){
                        if (k<0 || k>26 || !lettersEnabled[k])
                        {
                            errorProvider1.SetError(keyGB, "Key must be in range 0-26 and it must obey the chosen alphabet!");
                            return false;
                        }
                    }
                    else
                    {
                        errorProvider1.SetError(keyGB, "Key must contain integer values!");
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        private void inputTB_TextChanged(object sender, EventArgs e)
        {
            if (inputTB.Text.Length == 0)
            {
                errorProvider2.Clear();
                return;
            }
            string regexStr = "[^";
            for (int i=0; i<27; i++)
            {
                if (lettersEnabled[i])
                    regexStr += msgText[i];
            }
            regexStr += "]+";
            Regex regex = new Regex(regexStr);
            Match match = regex.Match(inputTB.Text.ToString());
            if (match.Success)
            {
                errorProvider2.SetError(inputTB,"Letters do not obey the alphabet! Use lowercase letters only!");
                encryptButton.Enabled = decryptButton.Enabled = false;
            }
            else
            {
                errorProvider2.Clear();
                if (errorProvider1.GetError(keyGB).Length == 0)
                    encryptButton.Enabled = decryptButton.Enabled = true;
            }
        }

        static int GCD(int a, int b)
        {
            while (b != 0)
            {
                int r = a % b;
                a = b;
                b = r;
            }
            return a;
        }

        static int modInverseNaive(int a, int m)
        {
            if (a < 0)
                a = m + a;
            a = a % m;
            //MessageBox.Show("modular " + a);
            for (int i=1; i<m; i++)
            {
                if ((a * i) % m == 1)
                    return i;
            }
            return 0;
        }

        private void encryptButton_Click(object sender, EventArgs e)
        {
            int determinant = encKey[0, 0] * encKey[1, 1] - encKey[0, 1] * encKey[1, 0];
            if (GCD(determinant, usedAlphabet) != 1)
            {
                MessageBox.Show("Encryption function is not injective! DET = " + determinant.ToString() + ", n = " + usedAlphabet.ToString());
                return;
            }
            if(inputTB.Text.Length % 2 != 0)
            {
                MessageBox.Show("Text length is not divisible by 2!");
                return;
            }

            string s = "";
            for (int i=0; i<inputTB.Text.Length-1; i+=2)
            {
                int v1 = msgText.IndexOf(inputTB.Text[i]);
                int v2 = msgText.IndexOf(inputTB.Text[i+1]);
                //MessageBox.Show(v1.ToString() + " " + v2.ToString() + " " + encKey[0,0] + " "+ encKey[0,1] + " " + encKey[1,0] + " "+ encKey[1,1]);

                int v1_new = (v1 * encKey[0,0] + v2 * encKey[1,0]) % usedAlphabet;
                int v2_new = (v1 * encKey[0,1] + v2 * encKey[1,1]) % usedAlphabet;

                s += defaultAlphabet.ElementAt(v1_new);
                s += defaultAlphabet.ElementAt(v2_new);
                //MessageBox.Show(s);
            }
            resultTB.Text = s;
        }

        private void decryptButton_Click(object sender, EventArgs e)
        {
            int determinant = encKey[0, 0] * encKey[1, 1] - encKey[0, 1] * encKey[1, 0];
            if (GCD(determinant, usedAlphabet) != 1)
            {
                MessageBox.Show("Encryption function is not injective! DET = " + determinant.ToString() + ", n = " + usedAlphabet.ToString());
                return;
            }
            if (inputTB.Text.Length % 2 != 0)
            {
                MessageBox.Show("Text length is not divisible by 2!");
                return;
            }
            if (determinant == 0)
            {
                MessageBox.Show("Determinant of the key is 0; cannot compute inverse!");
                return;
            }

            int modInv = modInverseNaive(determinant, usedAlphabet);
            //MessageBox.Show(modInv.ToString());
            int[,] inverse = new int[2, 2];
            inverse[0, 0] = encKey[1, 1];
            inverse[1, 1] = encKey[0, 0];
            inverse[0, 1] = (usedAlphabet-encKey[0, 1]);
            inverse[1, 0] = (usedAlphabet-encKey[1, 0]);
            //MessageBox.Show(inverse[0, 0] + " " + inverse[0, 1] + " " + inverse[1, 0] + " " + inverse[1, 1]);
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    inverse[i, j] = (inverse[i, j] * modInv) % usedAlphabet;
            //MessageBox.Show(inverse[0, 0] + " " + inverse[0, 1] + " " + inverse[1, 0] + " " + inverse[1, 1]);
            string s = "";
            for (int i = 0; i < inputTB.Text.Length - 1; i += 2)
            {
                int v1 = msgText.IndexOf(inputTB.Text[i]);
                int v2 = msgText.IndexOf(inputTB.Text[i + 1]);
                //MessageBox.Show(v1.ToString() + " " + v2.ToString() + " " + encKey[0,0] + " "+ encKey[0,1] + " " + encKey[1,0] + " "+ encKey[1,1]);

                int v1_new = (v1 * inverse[0, 0] + v2 * inverse[1, 0]) % usedAlphabet;
                int v2_new = (v1 * inverse[0, 1] + v2 * inverse[1, 1]) % usedAlphabet;

                s += defaultAlphabet.ElementAt(v1_new);
                s += defaultAlphabet.ElementAt(v2_new);
                //MessageBox.Show(s);
            }
            resultTB.Text = s;
        }
    }
}
