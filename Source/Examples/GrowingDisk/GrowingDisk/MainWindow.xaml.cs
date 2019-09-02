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
using Microsoft.Win32;
using System.IO;
using System.Globalization;
using System.Diagnostics;

namespace GrowingDisk
{
    
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {

        const int Button2slider=10;
        List<myCircle> mcircle;
        static List<Double> timeTable;
        double secondtime;
        bool drawn;

        private static void StandardOutputReceiver(object sendingProcess, DataReceivedEventArgs outLine)
        {

            // Receives the child process' standard output
            if (!string.IsNullOrEmpty(outLine.Data))
            {
                string [] tokens=outLine.Data.Split();
                double t = Double.Parse(tokens[0], CultureInfo.InvariantCulture);
                timeTable.Add(t);
                
            }
        }
        private void DrawLine()
        {


        }

        private void getEliminationTime(List<myCircle> input)
        {
            ProcessStartInfo startinfo = new ProcessStartInfo("./growing.exe");
            startinfo.Arguments = "test";
            startinfo.CreateNoWindow = true;
            startinfo.UseShellExecute = false;
            startinfo.RedirectStandardInput = true;
            startinfo.RedirectStandardOutput = true;
            Process p = new Process();
            p.StartInfo = startinfo;
            p.OutputDataReceived += StandardOutputReceiver;
            p.Start();
            p.BeginOutputReadLine();
            p.StandardInput.WriteLine(input.Count);
            for(int i = 0; i < input.Count; i++)
            {
                p.StandardInput.WriteLine("" + input[i].x + " " + input[i].y + " " + input[i].v);
            }
            p.WaitForExit();
            
            while (timeTable.Count != input.Count) ;

            double maxtime;
            maxtime = 0;
            secondtime = 0;
            foreach (double t in timeTable)
            {
                if (t > maxtime)
                {
                    secondtime = maxtime;
                    maxtime = t;
                }
                else if (t > secondtime)
                {
                    secondtime = t;
                }
            }
            secondtime *= 1.1;

            return ;
        }

        private void drawCircle(double time)
        {
            if (drawn == false)
            {

                
                for (int i = 0; i < mcircle.Count; i++)
                {
                    myCircle mc = mcircle[i];
                    Ellipse me = new Ellipse
                    {
                        HorizontalAlignment = HorizontalAlignment.Left,
                        VerticalAlignment = VerticalAlignment.Top,
                        Name = "circle" + mc.id,
                        Visibility = Visibility.Visible,
                        Height = 2*mc.v * time,
                        Width = 2*mc.v * time,
                        

                    };

                    SolidColorBrush blackBrush = new SolidColorBrush();
                    blackBrush.Color = Colors.Black;
                    me.Stroke = blackBrush;

                    me.Margin = new Thickness(mc.x-(mc.v*time), mc.y - (mc.v * time),0,0);
                    Panel.SetZIndex(me, 2);
                    Sketch.Children.Add(me);
                    
                }
                drawn = true;
            }
            else
            {
                for(int i=0;i<mcircle.Count;i++) 
                {
                    myCircle mc = mcircle[i];
                    Ellipse me =(Ellipse)Sketch.Children[1+i];
                    
                    if ( timeTable[i]< time) me.Visibility=Visibility.Hidden;
                    else
                    {
                        me.Height = 2*mc.v * time;
                        me.Width = 2*mc.v * time;
                        me.Margin = new Thickness(mc.x - (mc.v * time), mc.y - (mc.v * time), 0, 0);
                        me.Visibility = Visibility.Visible;
                    }
                }

            }
        }


        private List<myCircle> pointParsing(StreamReader sr)
        {
            string line;
            string[] tokens;
 

          

             while ((line = sr.ReadLine()) != null)
             {
                 double x, y,  v;
                 
                 tokens=line.Split();
                 x = Double.Parse(tokens[0], CultureInfo.InvariantCulture);
                 y = Double.Parse(tokens[1], CultureInfo.InvariantCulture);
                 v = Double.Parse(tokens[2], CultureInfo.InvariantCulture);
                 mcircle.Add(new myCircle(x,y,v) );
                  
            }

      
            return mcircle;
        }

        private void removeCircle()
        {
            foreach (myCircle mc in mcircle)
            {
                Ellipse me = (Ellipse)Sketch.FindName("circle" + mc.id);
                Sketch.Children.Remove(me);
            }
            drawn = false;
            mcircle.Clear();
            timeTable.Clear();
        }

        public MainWindow()
        {
            mcircle = new List<myCircle>();
            timeTable = new List<double>();
            drawn = false;
            InitializeComponent();
        }

        private void BackGroundButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Image files (*.png;*jepg;*.jpg;*.bmp)|*.png;*jepg;*.jpg;*.bmp";
            if (openFileDialog.ShowDialog() == true)
            {
                MapIamge.Source = new BitmapImage(new Uri(openFileDialog.FileName));
            }

        }


        private void PointLoad_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            
            if (openFileDialog.ShowDialog() == true)
            {
                if (drawn) removeCircle();
                StreamReader sr = new StreamReader(openFileDialog.FileName);
                mcircle = pointParsing(sr);
                getEliminationTime(mcircle);
                //mcircle.Sort();
                timeSlider.Value = 0;
                drawCircle(0);
            }
        }

        private void ScrollDown_Click(object sender, RoutedEventArgs e)
        {
            if (timeSlider.Value >= Button2slider)
                timeSlider.Value -= Button2slider;
            else timeSlider.Value = 0;

        }

        private void ScrollUp_Click(object sender, RoutedEventArgs e)
        {
            if (timeSlider.Value <= timeSlider.Maximum-Button2slider)
                timeSlider.Value += Button2slider;
            else timeSlider.Value = timeSlider.Maximum;
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            double t = (secondtime * timeSlider.Value) / timeSlider.Maximum;
            drawCircle(t);
        }
    }
}
