using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GrowingDisk
{
    class myCircle
    {
        public double x, y, v;
        public double elimination_time;
        private static int Cid;
        public int id;

        int p ;
        public myCircle(double x=0,double y=0,double v=0, double elimination_time=-1) 
        {
            
            this.elimination_time= elimination_time;
            this.x = x;
            this.y = y;
            this.v = v;
            this.id = Cid;
            Cid++;
        }

        public int CompareTo(myCircle mc)
        {
            return elimination_time.CompareTo(mc.elimination_time);
        }

    }
}
