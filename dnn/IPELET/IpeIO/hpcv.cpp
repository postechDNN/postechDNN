// --------------------------------------------------------------------
// Ipelet for creating Half Angle Line
// --------------------------------------------------------------------
#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipereference.h"
#include "ipebase.h"
#include "ipeshape.h"
//#include "ipeIO.h"

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

  Page *page = data->iPage;
  int sel = page->primarySelection();
  if (sel < 0) {
    helper->message("No selection");
    return false;
  }
  int n=page->count();
	ret.clear();
	//copy mark(points)
	for(int i=0;i<n;i++){
		//check the type is reference
		if(page->object(i)->type()!=Object::EReference) continue;
		Reference *ref=page->object(i)->asReference();

		//check it is mark
		String name = ref->name().string(); 
		if(!(name.substr(0,4)==String("mark"))) continue;

		ret.push_back(ref->position());
	}

	if (ret.size()==0) {
		helper->message("No mark selection");
		return false;
	}

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

  double kokzix = a;
  double kokziy = 0;

  hps.clear();
  Vector tempv;
  tempv.x = kokzix + cmx;
  tempv.y = kokziy + cmy;
  hps.push_back(tempv);
  tempv.x = -kokzix + cmx;
  tempv.y = kokziy + cmy;
  hps.push_back(tempv);

  double tx = 0;
  double ty = 0;
  //(x-cmx)2/a2 - (y-cmy)2/b2 = 1
  for (int i = 0; i < 10000; i++) {
    kokzix += 0.3;
    kokziy = sqrt(((pow(kokzix,2)/pow(a,2))-1)*pow(b,2));
    tempv.x = kokzix + cmx;
    tempv.y = kokziy + cmy;
    hps.push_back(tempv);//1sabun
    tempv.x = -kokzix + cmx;
    tempv.y = kokziy + cmy;
    hps.push_back(tempv);//2sa
    tempv.x = -kokzix + cmx;
    tempv.y = -kokziy + cmy;
    hps.push_back(tempv);//3sa
    tempv.x = kokzix + cmx;
    tempv.y = -kokziy + cmy;
    hps.push_back(tempv);//4sa
  }

  Attribute tAtr(true,String("mark/disk(sx)"));
  for (int i = 0; i < 40000; i++) {
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