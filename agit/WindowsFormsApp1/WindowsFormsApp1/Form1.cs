using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        int status = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private unsafe void button1_Click(object sender, EventArgs e)
        {
            timer1.Start();

        }

        private unsafe void timer1_Tick(object sender, EventArgs e)
        {
            Bitmap bmp = new Bitmap(128, 128, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            Bitmap bmpA = new Bitmap(128, 128, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            Bitmap bmpB = new Bitmap(128, 128, System.Drawing.Imaging.PixelFormat.Format32bppRgb);

            Graphics g = Graphics.FromImage(bmpA);
            g.DrawString("A", new Font("Arial", 100), Brushes.Red, new Point(0, 0));
            this.pictureBox2.Image = bmpA;
            g = Graphics.FromImage(bmpB);
            g.DrawString("B", new Font("Arial", 100), Brushes.Blue, new Point(0, 0));
            this.pictureBox3.Image = bmpB;
            g.Flush();






            BitmapData bm = bmp.LockBits(new Rectangle(Point.Empty, bmp.Size), ImageLockMode.ReadWrite, bmp.PixelFormat);
            UInt32* ptr = (UInt32*)bm.Scan0.ToPointer();

            BitmapData bmA = bmpA.LockBits(new Rectangle(Point.Empty, bmpA.Size), ImageLockMode.ReadWrite, bmpA.PixelFormat);
            UInt32* ptrA = (UInt32*)bmA.Scan0.ToPointer();

            BitmapData bmB = bmpB.LockBits(new Rectangle(Point.Empty, bmpB.Size), ImageLockMode.ReadWrite, bmpB.PixelFormat);
            UInt32* ptrB = (UInt32*)bmB.Scan0.ToPointer();
            //przypisanie wskaźnika 
            //Scan0 - 1 bajt pierwszego piksela 

            switch(status) {

                case 0 :

                    for(int i = 0; i < 128 * 128; i++)
                    {
                        *ptr = *ptrA;
                        ptr++;
                        ptrA++;
                    }
                    break;
                case 2:

                    for(int i =0; i< 128*128; i++)
                    {
                        if(*ptrB == 0xFF0000FF)
                        {
                            *ptr = 0x00000000;
                                 
                        } else
                        {
                            *ptr = *ptrA;
                        }
                        ptr++;
                        ptrA++;
                        ptrB++;
                    }

                    break;
                case 3:

                    for (int i = 0; i < 128 * 128; i++)
                    {
                        *ptr = *ptrB;
                        ptr++;
                        ptrB++;
                    }
                        break;

                case 4:
                    for (int i = 0; i < 128 * 128; i++)
                    {
                        if (*ptrA == 0xFFFF0000)
                        {
                            *ptr = 0x00000000;

                        }
                        else
                        {
                            *ptr = *ptrB;
                        }
                        ptr++;
                        ptrA++;
                        ptrB++;
                    }
                    break;
                case 5:
                    

                    for (int i = 0; i < 128 * 128; i++)
                    {
                        if (*ptrA == *ptrB)
                        {
                            *ptr = 0x00000000;
                        }
                        else if ((*ptrA == 0xFFFF0000) && (*ptrB == 0xFF0000FF))
                        {
                            *ptr = (*ptrA + *ptrB) / 2;
                        }
                        else if ((*ptrA == 0xFFFF0000) && (*ptrB == 0x00000000))
                        {
                            *ptr = *ptrA;
                        }
                        else
                        {
                            *ptr = *ptrB;
                        }

                        ptr++;
                        ptrB++;
                        ptrA++;


                    }
                    break;

            }







            bmp.UnlockBits(bm);
            bmpA.UnlockBits(bmA);
            bmpB.UnlockBits(bmB);

            this.pictureBox1.Image = bmp;

            status++;

            if (status == 6) status = 0;
        }
    }
}
