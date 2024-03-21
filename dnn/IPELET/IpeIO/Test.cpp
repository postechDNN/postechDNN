// Ipelet for finding convex hull using graham scan

#include "ipelet.h"
#include "ipepage.h"
#include "ipeIO.h"

#include <iostream>
#include <vector>
#include <random>

using namespace ipe;
using namespace std;

class IO_Test_Ipelet : public Ipelet {
public:
	virtual int ipelibVersion() const {return IPELIB_VERSION;}
	virtual bool run(int, IpeletData *data, IpeletHelper *helper);
private:

};

bool IO_Test_Ipelet::run(int, IpeletData *data, IpeletHelper *helper)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-100,100);

	vector<Vector> p_tmp;
	vector<const SubPath*> sp_tmp;
	vector<CurveSegment> cs_tmp;

	IPEIO points;
	IPEIO polys;
	IPEIO segs;
	
	points.set_color("red");
	polys.set_color(0,255,0);
	segs.set_color(0,0,255);

	segs.set_arrow(false);
	segs.set_dash(3);
	segs.set_pen(3);

	polys.set_pen(2);

	Get_polygons(data,helper,sp_tmp);
	Get_points(data,helper,p_tmp);
	Get_segments(data,helper,true,cs_tmp);
	int i=0;
	for (auto pt:p_tmp){
		pt.x+=dis(gen);
		pt.y+=dis(gen);
		points.set_pts_size((i%4)-2);
		points.set_pts_style(i%5);
		i+=1;
		points.Draw_point(data,helper,pt);
	}
	
	for (auto poly:sp_tmp){
		auto poly_v = SubPath2Vec(poly,false);
		std::vector<Vector> new_poly;
		for (auto v:poly_v){
			v.x+=dis(gen);
			v.y+=dis(gen);
			new_poly.push_back(v);
		}
		polys.set_fill("blue",i%2);
		i+=1;
		polys.Draw_polygon(data,helper,new_poly,true);
	}
	for(auto seg:cs_tmp){
		Curve *c = new Curve;
		for(int i=1;i<seg.countCP();i++){
			Vector tmp1 = seg.cp(i-1), tmp2 = seg.cp(i);
			tmp1.x+=dis(gen);
			tmp2.x+=dis(gen);
			tmp1.y+=dis(gen);
			tmp2.y+=dis(gen);
			c->appendSegment(tmp1,tmp2); 
		}
		for(int i=0;i<c->countSegments();i++){
			segs.Draw_segment(data,helper,c->segment(i));
		}
	}
	return true;
}

IPELET_DECLARE Ipelet *newIpelet()
{
  return new IO_Test_Ipelet;
}