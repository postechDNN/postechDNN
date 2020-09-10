// --------------------------------------------------------------------
// Ipelet for creating Half Angle Line
// --------------------------------------------------------------------
#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipereference.h"
#include "ipebase.h"
#include "ipeshape.h"
#include "../IpeIO/ipeIO.h"

#include <math.h>
#include <iostream>
#include <vector>
#define DRAW_LENG 200 //simply change half angle line length
using namespace ipe;
using namespace std;

// --------------------------------------------------------------------
class HalfAngle : public Ipelet {
public:
  virtual int ipelibVersion() const { return IPELIB_VERSION; }
  virtual bool run(int, IpeletData *data, IpeletHelper *helper);
};
// --------------------------------------------------------------------
bool HalfAngle::run(int, IpeletData *data, IpeletHelper *helper)
{
  vector<Vector> ret;
  int retsize = ret.size();

  Get_points(data, helper, ret);

  if (ret.size() < 3) {
    helper->message("Please make more than 3 marks");
    return false;
  }
  vector<Vector> new_ret;
  for(int i=0;i<3;i++){
    new_ret.push_back(ret[retsize-3+i]);
  }
  if (new_ret.size() !=3) {
    helper->message("Please select 3 marks of the part you want to divide");
    return false;
  }

  double x2 = new_ret.at(0).x;
  double y2 = new_ret.at(0).y;
  double xc = new_ret.at(1).x;
  double yc = new_ret.at(1).y;
  double x1 = new_ret.at(2).x;
  double y1 = new_ret.at(2).y;
  //x1이랑 x2가 같을 경우에 대비해야 한다

  double th1 = atan((y1-yc)/(x1-xc));
  double th2 = atan((y2-yc)/(x2-xc));
  double thn = (th1+th2)/2;
  double slope = tan(thn);

  Vector new_p;
  new_p.x = xc + DRAW_LENG;
  new_p.y = (slope * DRAW_LENG) + yc;

  Vector oppo_p;
  oppo_p.x = xc - DRAW_LENG;
  oppo_p.y = (slope * (-DRAW_LENG)) + yc;

  IPEIO io;
  io.Draw_segment(data, helper, oppo_p, new_p);

  helper->message("Created half angle line!");
  //ret.clear();
  return true;
}
// --------------------------------------------------------------------
IPELET_DECLARE Ipelet *newIpelet()
{
  return new HalfAngle;
}
// --------------------------------------------------------------------