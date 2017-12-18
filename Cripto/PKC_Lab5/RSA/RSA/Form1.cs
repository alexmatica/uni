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

namespace RSA
{
    public partial class Form1 : Form
    {
        Int64         phi;
        PrimesPair  primesPair;
        PublicKey   publicKey;
        Int64         privateKey;
        bool        generated = false;
        bool        isPrimeP = false;
        bool        isPrimeQ = false;
        bool        numbersChanged = false;

        static string pattern = "[^a-z ]";
        Regex       textValidRegex = new Regex(pattern, RegexOptions.None);

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private static bool isPrime(long n)
        {
            if (n < 2 || n > 2 && n % 2 == 0)
                return false;
            for (long d = 3; d * d <= n; d += 2)
                if (n % d == 0)
                    return false;
            return true;
        }

        private void generatePrimesPair()
        {
            Random rnd = new Random();
            long maxPQ = (long)Math.Pow(27.0, (double)numericL.Value / 2.0);
            long minPQ = (long)Math.Pow(27.0, (double)numericK.Value / 2.0);

            long P, Q;
            do
            {
                P = LongRandom(minPQ, maxPQ, rnd);
            } while (!isPrime(P));

            do
            {
                Q = LongRandom(minPQ, maxPQ, rnd);
            } while (!isPrime(Q) || P == Q);

            this.primesPair = new PrimesPair(P, Q);
            generated = false;
            isPrimeP = isPrimeQ = true;
            numbersChanged = false;
        }

        long modInverse(long a, long n)
        {
            long i = n, v = 0, d = 1;
            while (a > 0)
            {
                long t = i / a, x = a;
                a = i % x;
                i = x;
                x = d;
                d = v - t * x;
                v = x;
            }
            v %= n;
            if (v < 0) v = (v + n) % n;
            return v;
        }

        long gcdSimple(long a, long b)
        {
            while (b != 0)
            {
                long r = a % b;
                a = b;
                b = r;
            }

            return a;
        }

        private void generatePQButton_Click(object sender, EventArgs e)
        {
            generatePrimesPair();
            textBoxP.Text = this.primesPair.P.ToString();
            textBoxQ.Text = this.primesPair.Q.ToString();
            generateKeyPair();
        }

        long LongRandom(long min, long max, Random rand)
        {
            byte[] buf = new byte[8];
            rand.NextBytes(buf);
            long longRand = BitConverter.ToInt64(buf, 0);

            return (Math.Abs(longRand % (max - min)) + min);
        }

        private void generateKeyPair()
        {
            if (generated == false)
            {
                Int64 n = this.primesPair.P * this.primesPair.Q;
                this.phi = (this.primesPair.P - 1) * (this.primesPair.Q - 1);
                
                Int64 e = 1;
                Random rnd = new Random();
                do
                {
                    e = LongRandom(1, phi, rnd);
                    
                } while (gcdSimple(e, phi) != 1);

                this.publicKey = new PublicKey(n, e);
                this.privateKey = modInverse(e, phi);
                generated = true;
            }
        }

        private void buttonShow_Click(object sender, EventArgs e)
        {
            inputPrimes();
            if (generated == false)
            {
                MessageBox.Show("No key generated so far! Please input some distinct primes for P and Q!");
            }
            else
            {
                MessageBox.Show("Public key: (" + this.publicKey.n + ", " + this.publicKey.e + ")\n" +
                    "Private key: " + this.privateKey);
            }
        }

        private void checkText(string text, bool forP)
        {
            int result;
            if (Int32.TryParse(text, out result))
            {
                if (isPrime(result))
                {
                    if (forP)
                        isPrimeP = true;
                    else
                        isPrimeQ = true;
                }
                else
                {
                    if (forP)
                        isPrimeP = false;
                    else
                        isPrimeQ = false;
                }
            }
            else
            {
                MessageBox.Show("The input value must be an integer number!");
            }
        }

        private void inputPrimes()
        {
            if (numbersChanged)
            {
                generated = false;
                checkText(textBoxP.Text, true);
                checkText(textBoxQ.Text, false);

                if (isPrimeP && isPrimeQ)
                {
                    this.primesPair = new PrimesPair(Int32.Parse(textBoxP.Text), Int32.Parse(textBoxQ.Text));
                    generateKeyPair();
                    numbersChanged = false;
                }
            }
        }

        private void textBoxP_TextChanged(object sender, EventArgs e)
        {
            numbersChanged = true;
            generated = false;
        }

        private void textBoxQ_TextChanged(object sender, EventArgs e)
        {
            generated = false;
            numbersChanged = true;
        }

        long modExp(long a, long b, long n)
        {
            long r = 1;
            do
            {
                if (b % 2 == 1)
                    r = (r * a) % n;
                b /= 2;
                a = (a * a) % n;
            } while (b > 0);
            return r;
        }

        List<Int64> makeNumerical(bool isCipher)
        {
            int k;
            if (isCipher)
            {
                k = (int)numericL.Value;
            }
            else
            {
                k = (int)numericK.Value;
            }

            byte[] asciiBytes = Encoding.ASCII.GetBytes(textRTB.Text);
            for (int i = 0; i < asciiBytes.Count(); i++)
            {
                if (asciiBytes[i] == 32)
                    asciiBytes[i] = 0;
                else
                    asciiBytes[i] -= 96;
            }
            List<Int64> numericalMessage = new List<Int64>();
            for (int i = 0; i < asciiBytes.Count(); i += k)
            {
                int pw = 1;
                Int64 numerical = 0;
                for (int j = i + k - 1; j >= i; j--)
                {
                    if (j >= asciiBytes.Count())
                    {
                        pw *= 27;
                        continue;
                    }
                    numerical += ((pw * asciiBytes[j]) % this.publicKey.n);
                    pw *= 27;
                }
                numericalMessage.Add(numerical);
            }

            return numericalMessage;
        }

        void doWork(bool encrypt, List<Int64> numericalMessage)
        {
            long exp;
            int l;
            if (encrypt)
            {
                exp = this.publicKey.e;
                l = (int)numericL.Value;
            }
            else
            {
                exp = this.privateKey;
                l = (int)numericK.Value;
            }

            byte[] encodedText = new byte[numericalMessage.Count * 3 + 1];
            int finalBytes = 0;

            for (int i = 0; i < numericalMessage.Count; i++)
            {
                numericalMessage[i] = modExp(numericalMessage[i], exp, this.publicKey.n);
                int count = l - 1;
                while (numericalMessage[i] > 0)
                {
                    encodedText[finalBytes + count] = (byte)(numericalMessage[i] % 27);
                    if (encodedText[finalBytes + count] > 0)
                        encodedText[finalBytes + count] += 64;
                    else
                        encodedText[finalBytes + count] = 95;

                    numericalMessage[i] /= 27;
                    count--;
                }
                while (count >= 0)
                {
                    encodedText[finalBytes + count] = 95;
                    count--;
                }
                finalBytes += l;
            }
            cipherRTB.Text = Encoding.ASCII.GetString(encodedText);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (generated == false)
            {
                MessageBox.Show("Must input distinct primes P and Q in order to generate a key!");
                return;
            }
            Match m = textValidRegex.Match(textRTB.Text);
            if (m.Success)
            {
                MessageBox.Show("Invalid text! Input must contain only lowercase letters from the English alphabet " +
                    "and the blank character (space)!");
            }
            else
            {
                //this.publicKey = new PublicKey(1643, 67);
                //phi = 1560;
                //this.privateKey = modInverse(67, phi);
                //MessageBox.Show(this.privateKey + " ");

                List<Int64> numericalMessage = makeNumerical(false);
                doWork(true, numericalMessage);
            }
        }

        private void numericK_ValueChanged(object sender, EventArgs e)
        {
            if ((int)numericK.Value >= (int)numericL.Value)
            {
                numericL.Value = numericK.Value + 1;
                MessageBox.Show("K must be smaller than L!");
            }
        }

        private void numericL_ValueChanged(object sender, EventArgs e)
        {
            if ((int)numericK.Value >= (int)numericL.Value)
            {
                numericL.Value = numericK.Value + 1;
                MessageBox.Show("K must be smaller than L!");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (generated == false)
            {
                MessageBox.Show("Must input distinct primes P and Q in order to generate a key!");
                return;
            }
            Match m = textValidRegex.Match(textRTB.Text);
            if (m.Success)
            {
                MessageBox.Show("Invalid text! Input must contain only lowercase letters from the English alphabet " +
                    "and the blank character (space)!");
            }
            else
            {
                //this.publicKey = new PublicKey(1643, 67);
                //phi = 1560;
                //this.privateKey = modInverse(67, phi);
                //MessageBox.Show(this.privateKey + " ");

                List<Int64> numericalMessage = makeNumerical(true);
                doWork(false, numericalMessage);
            }
        }
    }

    public struct PrimesPair
    {
        public Int64 P;
        public Int64 Q;

        public PrimesPair(Int64 _P, Int64 _Q)
        {
            P = _P;
            Q = _Q;
        }
    }

    public struct PublicKey
    {
        public Int64 n;
        public Int64 e;

        public PublicKey(Int64 _n, Int64 _e)
        {
            n = _n;
            e = _e;
        }
    }
}
