// --------------------------------------------------------------------
// Ipelet for creating Hyperbola
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
  vector<Vector> hps;

  Get_points(data, helper, ret);
  
  if (ret.size() !=1) {
    helper->message("Please select a center mark of hyperbola");
    return false;
  }

  String str1;
  if (!helper->getString("Enter a", str1))
    return false;
  int a = Lex(str1).getInt();

  String str2;
  if (!helper->getString("Enter b", str2))
    return false;
  int b = Lex(str2).getInt();

  double cmx = ret.at(0).x;
  double cmy = ret.at(0).y;

  double edgex = a;
  double edgey = 0;

  hps.clear();
  Vector tempv;
  tempv.x = edgex + cmx;
  tempv.y = edgey + cmy;
  hps.push_back(tempv);
  tempv.x = -edgex + cmx;
  tempv.y = edgey + cmy;
  hps.push_back(tempv);

  double tx = 0;
  double ty = 0;
  //(x-cmx)2/a2 - (y-cmy)2/b2 = 1
  for (int i = 0; i < 1000; i++) {
    edgex += 0.1;
    edgey = sqrt(((pow(edgex,2)/pow(a,2))-1)*pow(b,2));
    tempv.x = edgex + cmx;
    tempv.y = edgey + cmy;
    hps.push_back(tempv);//quadrant 1
    tempv.x = -edgex + cmx;
    tempv.y = edgey + cmy;
    hps.push_back(tempv);//quad 2
    tempv.x = -edgex + cmx;
    tempv.y = -edgey + cmy;
    hps.push_back(tempv);//quad 3
    tempv.x = edgex + cmx;
    tempv.y = -edgey + cmy;
    hps.push_back(tempv);//quad 4
  }

  Attribute tAtr(true,String("mark/disk(sx)"));
  for (int i = 0; i < 4000; i++) {
    Reference* pt = new Reference(data->iAttributes,tAtr,hps.at(i));
    data->iPage->append(ESecondarySelected,data->iLayer,pt);
  }
	//pt->setStroke(color_attr);
  helper->message("Created hyperbola!");
  return true;
}
// --------------------------------------------------------------------
IPELET_DECLARE Ipelet *newIpelet()
{
  return new HalfAngle;
}
// --------------------------------------------------------------------