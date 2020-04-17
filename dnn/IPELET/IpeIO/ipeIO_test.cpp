// Ipelet for finding convex hull using graham scan

#include "ipelet.h"
#include "ipepage.h"
#include "ipeIO.h"

#include <iostream>
#include <vector>

#include <sstream>

#include "ShortestPathSimplePolygon\Point.h"
#include "ShortestPathSimplePolygon\run.cpp"

using namespace std;
ipe::Vector make_point(Point &p);
vector<ipe::Vector> make_poly(vector<Point> &pts);
class IO_Test_Ipelet : public ipe::Ipelet {
public:
	virtual int ipelibVersion() const {return ipe::IPELIB_VERSION;}
	virtual bool run(int, ipe::IpeletData *data, ipe::IpeletHelper *helper);
private:

};

ipe::Vector make_point(Point &p){
	return Vector(p.get_x(),p.get_y());
}
vector<ipe::Vector> make_poly(vector<Point> &pts){
	vector<ipe::Vector> ret;
	for (auto it = pts.begin();it!=pts.end();it++)
		ret.push_back(make_point(*it));
	return ret;
}
bool IO_Test_Ipelet::run(int, ipe::IpeletData *data, ipe::IpeletHelper *helper)
{
	vector<Vector> p_tmp;
	vector<SubPath*> sp_tmp;
	vector<Point> input, test_p;
	Get_polygons(data,helper,sp_tmp);
	Get_points(data,helper,p_tmp);
	for (auto it = p_tmp.begin();it!=p_tmp.end();it++)
		test_p.push_back(Point((*it).x,(*it).y));
	auto poly = SubPath2Vec(sp_tmp[0],false);
	for (auto it = poly.begin();it!=poly.end();it++)
		input.push_back(Point((*it).x,(*it).y));
	auto ret =_run(input,test_p);

	for (auto it = ret.begin(); it!=ret.end();it++){
		auto tmp=*it;
		if (tmp.size()==0) continue;
		if (tmp.size()==1) {
			auto ttmp = make_point(tmp[0]);
			Draw_point(data,helper,ttmp);
		}
		else {
			auto ttmp = make_poly(tmp);
			Draw_polygon(data,helper,ttmp,false);
		}
	}
}

IPELET_DECLARE Ipelet *newIpelet()
{
  return new IO_Test_Ipelet;
}