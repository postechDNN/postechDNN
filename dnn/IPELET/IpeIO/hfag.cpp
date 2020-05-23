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

  if (ret.size() !=3) {
    helper->message("Please select 3 marks of the part you want to divide");
    return false;
  }

  double x2 = ret.at(0).x;
  double y2 = ret.at(0).y;
  double xc = ret.at(1).x;
  double yc = ret.at(1).y;
  double x1 = ret.at(2).x;
  double y1 = ret.at(2).y;

  double th1 = atan((y1-yc)/(x1-xc));
  double th2 = atan((y2-yc)/(x2-xc));
  double thn = (th1+th2)/2;
  double slope = tan(thn);

  //(y-yc) = slope(x-xc), set delta x = 100
  //ydelt = y-yc
  double xget = xc + 100;
  double yget = (slope * 100) + yc;
  
  Vector new_p;
  new_p.x = xget;
  new_p.y = yget;

  Curve *sp = new Curve;
	sp->appendSegment(ret.at(1),new_p);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes,shape);
	//obj->setStroke(color_attr);
	data->iPage->append(ESecondarySelected,data->iLayer,obj);
  helper->message("Created half angle line!");
  return true;
}
// --------------------------------------------------------------------
IPELET_DECLARE Ipelet *newIpelet()
{
  return new HalfAngle;
}
// --------------------------------------------------------------------