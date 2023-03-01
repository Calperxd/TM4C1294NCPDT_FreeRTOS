using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Timers;
using System.Windows.Threading;
using System.Threading;
using System.IO;
using CsvHelper;
using System.Globalization;

namespace WindowsFormsApp1
{

    public partial class Form1 : Form
    {
        private const string V = "Amostra";
        private WMPLib.WindowsMediaPlayer player = new WMPLib.WindowsMediaPlayer();
        private string dataIn;
        List<string> list = new List<string>();
        private int teste = 0;

        public Form1()
        {
            InitializeComponent();
            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(100);
            timer.Tick += timer_Tick;
            timer.Start();

        }
        void timer_Tick(object sender, EventArgs e)
        {
            player.settings.volume = scrllVolume.Value;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            cbPort.Items.AddRange(ports);
            btnClose.Enabled = false;
            btnDownload.Enabled = false;
            btnAmostragem.Enabled = false;
            txtTime.Enabled = false;
            btnPlus.Enabled = false;
            btnMinus.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try 
            {
                serialPort1.PortName = cbPort.Text;
                serialPort1.BaudRate = 921600;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.StopBits = StopBits.One;
                serialPort1.Open();
                if (serialPort1.IsOpen)
                {
                    btnClose.Enabled = true;
                    btnDownload.Enabled = true;
                    btnOpen.Enabled = false;
                    btnAmostragem.Enabled = true;
                    txtTime.Enabled = true;
                    btnPlus.Enabled = true;
                    btnMinus.Enabled = true;
                    cbPort.Enabled = false;
                }
                
            }
            catch(Exception err)
            {
                MessageBox.Show(err.Message);
            }

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                btnClose.Enabled = false;
                btnDownload.Enabled = false;
                btnOpen.Enabled = true;
                btnAmostragem.Enabled = false;
                txtTime.Enabled = false;
                btnPlus.Enabled = false;
                btnMinus.Enabled = false;
                cbPort.Enabled = true;
                serialPort1.Close();
            }
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            player.URL = "C:\\Users\\Galc_\\OneDrive\\UTFPR\\TCC\\GUI\\WindowsFormsApp1\\white-noise.mp3";
            player.controls.play();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            player.controls.stop();
        }

        private void btnAmostragem_Click(object sender, EventArgs e)
        {
            list.Clear();
            serialPort1.Write("#");
            int time = Int32.Parse(txtTime.Text);
            serialPort1.Write(time.ToString("00"));
        }

        private void btnPlus_Click(object sender, EventArgs e)
        {
            serialPort1.Write("$");
        }

        private void btnMinus_Click(object sender, EventArgs e)
        {
            serialPort1.Write("%");
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            dataIn = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(ShowData));
        }

        private void ShowData(object sender, EventArgs e)
        {
            list.Add(dataIn);
        }

        private void btnDownload_Click(object sender, EventArgs e)
        {
            string strExeFilePath = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string strWorkPath = System.IO.Path.GetDirectoryName(strExeFilePath);
            teste++;
            for (int i = 1; i < list.Count; i++)
            {
                File.AppendAllText(strWorkPath + "\\Amostra" + teste.ToString() + ".csv", list.ToArray().ElementAt(i));
            }
        }
    }
}
