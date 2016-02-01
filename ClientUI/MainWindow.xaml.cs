using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using CLRclient;

namespace ClientUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private
        //ManagedClient user;
        public MainWindow()
        {
            InitializeComponent();
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            TextSearchR.Text = "";
            ManagedClient user = new ManagedClient();
            int port = Convert.ToInt32(Tport.Text);
            int LN = Convert.ToInt32(TLN.Text);
            string Msg = TCN.Text;
            for (int i = 0; i < l1.Items.Count; i++)
                Msg +="||"+l1.Items[i];
            for (int i = 0; i < KeyWord.Items.Count; i++)
                Msg += "||#" + KeyWord.Items[i];
            Random rd = new Random();
            int rand = rd.Next();
            port += rand % 100;
            string tport = port.ToString();
            user.start(port, 5, 0, LN, Msg,tport);
            string useit = user.getResult();
            string big = DateTime.Now.ToString();
            string[] vect = useit.Split('?');
            string display = "";
            //for (int i = 1; i < vect.Length; i++)
            //{
            //    string[] vect2 = vect[i].Split('&');
            //    display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
            //}
            List<string> listArr = new List<string>();
            for (int i = 1; i < vect.Length; i++)
            {
                string[] vect2 = vect[i].Split('&');
                display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
                listArr.Add(vect2[1]);
            }
            string[] time = listArr.ToArray();
            int amount = 0;
            int largest = 0;
            int shortest = 0;
            for (int i = 0; i < time.Length; i++)
            {
                int each = Convert.ToInt32(time[i]);
                if (each > largest)
                    largest = each;
                if (i == 0)
                    shortest = largest;
                if (each < shortest)
                    shortest = each;
                amount += each;
            }

            int average = amount / time.Length;
            string Saverage = average.ToString();
            STT4.Text = shortest.ToString()+"ms";
            LGT_Copy2.Text = largest.ToString()+"ms";
            Saverage += "ms";
            UAV2.Text = Saverage;
            TextSearchR.Text = display;
        }
        private void b1_Click(object sender, RoutedEventArgs e)
        {
            KeyWord.Items.Add(T1.Text);
        }

        private void b2_Click(object sender, RoutedEventArgs e)
        {
            if (KeyWord.Items.Count != 0)
            {
                KeyWord.Items.Remove(KeyWord.Items[KeyWord.Items.Count-1]);
            }
        }

        private void b4_Click(object sender, RoutedEventArgs e)
        {
            if(l1.Items.Count!=0)
            {
                l1.Items.Remove(l1.Items[l1.Items.Count - 1]);
            }
        }
        private void b3_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog fd = new Microsoft.Win32.OpenFileDialog();
            fd.Title = "Open Files";
            fd.Multiselect = true;
            string str1 = System.IO.Directory.GetCurrentDirectory();
            int pos = str1.LastIndexOf("\\");
            str1 = str1.Substring(0, pos) + "\\TEST - 副本\\Text";
            fd.InitialDirectory = str1;
            fd.Filter = "All Files (*.*)|*.*|Text Files (.txt)|*.txt|H Files (.h)|*.h|CPP Files (.cpp)|*.cpp";
            if ((bool)fd.ShowDialog().GetValueOrDefault())
            {
                System.Collections.ArrayList fileList = new System.Collections.ArrayList();
                foreach (string file in fd.FileNames)
                {
                    fileList.Add(file);
                    l1.Items.Add(file);
                }
            }
        }
        private void SB1_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog Ad = new Microsoft.Win32.OpenFileDialog();
            Ad.Title = "Open Files";
            Ad.Multiselect = true;
            string str1 = System.IO.Directory.GetCurrentDirectory();
            int pos = str1.LastIndexOf("\\");
            str1 = str1.Substring(0, pos) + "\\TEST - 副本\\TESTT";
            Ad.InitialDirectory = str1;
            Ad.Filter = "All Files (*.*)|*.*|H Files (.h)|*.h|CPP Files (.cpp)|*.cpp";
            if ((bool)Ad.ShowDialog().GetValueOrDefault())
            {
                System.Collections.ArrayList fileList2 = new System.Collections.ArrayList();
                foreach (string file in Ad.FileNames)
                {
                    fileList2.Add(file);
                    SFB.Items.Add(file);
                }
            }
        }
        private void SB2_Click(object sender, RoutedEventArgs e)
        {
            if (SFB.Items.Count != 0)
            {
                SFB.Items.Remove(SFB.Items[SFB.Items.Count - 1]);
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            SResult.Text = "";
            ManagedClient user = new ManagedClient();
            int port = Convert.ToInt32(Sport.Text);
            int LN = Convert.ToInt32(SLN.Text);
            string Msg = SCN.Text;
            for (int i = 0; i < SFB.Items.Count; i++)
                Msg += "||" + SFB.Items[i];
            Random rd = new Random();
            int rand = rd.Next();
            port += rand % 100;
            string sport = port.ToString();
            user.start(port, 6, 0, LN, Msg, sport);
            string useit = user.getResult();
            string big = DateTime.Now.ToString();
            string[] vect = useit.Split('?');
            string display = "";
            //for (int i = 1; i < vect.Length; i++)
            //{
            //    string[] vect2 = vect[i].Split('&');
            //    display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
            //}
            List<string> listArr = new List<string>();
            for (int i = 1; i < vect.Length; i++)
            {
                string[] vect2 = vect[i].Split('&');
                display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
                listArr.Add(vect2[1]);
            }
            string[] time = listArr.ToArray();
            int amount = 0;
            int largest = 0;
            int shortest = 0;
            for (int i = 0; i < time.Length; i++)
            {
                int each = Convert.ToInt32(time[i]);
                if (each > largest)
                    largest = each;
                if (i == 0)
                    shortest = largest;
                if (each < shortest)
                    shortest = each;
                amount += each;
            }

            int average = amount / time.Length;
            string Saverage = average.ToString();
            STT3.Text = shortest.ToString()+"ms";
            LGT_Copy1.Text = largest.ToString()+"ms";
            Saverage += "ms";
            UAV_Copy1.Text = Saverage;
            SResult.Text = display;
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            SResult.Text = "";
            ManagedClient user = new ManagedClient();
            int port = Convert.ToInt32(Dport.Text);
            int LN = Convert.ToInt32(DLN.Text);
            string Msg = "";
            for (int i = 0; i < Filebox.Items.Count; i++)
            {
                if(i==0)
                    Msg += Filebox.Items[i];
                else
                    Msg +="||" + Filebox.Items[i]; 
            }
            Random rd = new Random();
            int rand = rd.Next();
            port += rand % 100;
            string sport = port.ToString();
            user.start(port, 2, 0, LN, Msg, sport);
            string useit = user.getResult();
            string big = DateTime.Now.ToString();
            string[] vect = useit.Split('?');
            string display = "";
            //for (int i = 1; i < vect.Length; i++)
            //{
            //    string[] vect2 = vect[i].Split('&');
            //    display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
            //}
            List<string> listArr = new List<string>();
            for (int i = 1; i < vect.Length; i++)
            {
                string[] vect2 = vect[i].Split('&');
                display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
                listArr.Add(vect2[1]);
            }
            string[] time = listArr.ToArray();
            int amount = 0;
            int largest = 0;
            int shortest = 0;
            for (int i = 0; i < time.Length; i++)
            {
                int each = Convert.ToInt32(time[i]);
                if (each > largest)
                    largest = each;
                if (i == 0)
                    shortest = largest;
                if (each < shortest)
                    shortest = each;
                amount += each;
            }

            int average = amount / time.Length;
            string Saverage = average.ToString();
            STT2.Text = shortest.ToString()+"ms";
            LGT_Copy.Text = largest.ToString()+"ms";       
            Saverage += "ms";
            UAV_Copy.Text = Saverage;
            ShowRe.Text = display;
        }

        private void DB1_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog Dd = new Microsoft.Win32.OpenFileDialog();
            Dd.Title = "Open Files";
            Dd.Multiselect = true;
            string str1 = System.IO.Directory.GetCurrentDirectory();
            int pos = str1.LastIndexOf("\\");
            str1 = str1.Substring(0, pos) + "\\TEST - 副本\\test";
            Dd.InitialDirectory = str1;
            Dd.Filter = "All Files (*.*)|*.*|H Files (.h)|*.h|CPP Files (.cpp)|*.cpp";
            if ((bool)Dd.ShowDialog().GetValueOrDefault())
            {
                System.Collections.ArrayList fileList3 = new System.Collections.ArrayList();
                foreach (string file in Dd.FileNames)
                {
                    fileList3.Add(file);
                    Filebox.Items.Add(file);
                }
            }
        }
        private void DB2_Click(object sender, RoutedEventArgs e)
        {
            if (Filebox.Items.Count != 0)
            {
                Filebox.Items.Remove(Filebox.Items[Filebox.Items.Count - 1]);
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog Gd = new Microsoft.Win32.OpenFileDialog();
            Gd.Title = "Open Files";
            Gd.Multiselect = true;
            string str1 = System.IO.Directory.GetCurrentDirectory();
            int pos = str1.LastIndexOf("\\");
            str1 = str1.Substring(0, pos) + "\\TEST - 副本\\test";
            Gd.InitialDirectory = str1;
            Gd.Filter = "All Files (*.*)|*.*|H Files (.h)|*.h|CPP Files (.cpp)|*.cpp";
            if ((bool)Gd.ShowDialog().GetValueOrDefault())
            {
                System.Collections.ArrayList fileList4 = new System.Collections.ArrayList();
                foreach (string file in Gd.FileNames)
                {
                    fileList4.Add(file);
                    FileBox2.Items.Add(file);
                }
            }
        }

        private void DB3_Click(object sender, RoutedEventArgs e)
        {
            if (FileBox2.Items.Count != 0)
            {
                FileBox2.Items.Remove(FileBox2.Items[FileBox2.Items.Count - 1]);
            }
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            SResult.Text = "";
            ManagedClient user = new ManagedClient();
            int port = Convert.ToInt32(Uport.Text);
            int LN = Convert.ToInt32(ULN.Text);
            string Msg = "";
            for (int i = 0; i < FileBox2.Items.Count; i++)
            {
                if (i == 0)
                    Msg += FileBox2.Items[i];
                else
                    Msg += "||" + FileBox2.Items[i];
            }
            Random rd = new Random();
            int rand = rd.Next();
            port += rand % 100;
            string uport = port.ToString();
            user.start(port, 4, 0, LN, Msg, uport);
            string useit = user.getResult();
            string big = DateTime.Now.ToString();
            string[] vect = useit.Split('?');
            List<string> listArr = new List<string>();
            string display = "";
            for (int i = 1; i < vect.Length; i++)
            {
                string[] vect2 = vect[i].Split('&');
                display = display + vect2[0] + "\n---Duration: " + vect2[1] + "ms\n---Endtime: " + big + "." + vect2[2] + "\n\n";
                listArr.Add(vect2[1]);
            }
            string[] time = listArr.ToArray();
            int amount = 0;
            int largest = 0;
            int shortest = 0;
            for (int i = 0; i < time.Length;i++ )
            {
                int each = Convert.ToInt32(time[i]);

                if (each > largest)
                    largest = each;
                if (i == 0)
                    shortest = largest;
                if (each < shortest)
                    shortest = each;
                amount += each;
            }
            int average = amount / time.Length;
            string Saverage = average.ToString();
            Saverage+="ms";
            UAV.Text = Saverage;
            STT1.Text = shortest.ToString()+"ms";
            LGT.Text = largest.ToString()+"ms";
            ShowResult1.Text = display;
        }
    }
}
