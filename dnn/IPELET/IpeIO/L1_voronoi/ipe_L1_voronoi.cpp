// Ipelet for L1 voronoi diagram

#include "ipelet.h"
#include "ipepage.h"
#include "../ipeIO.h"
#include "L1voronoi.h"
#include <vector>


class IO_Test_Ipelet : public ipe::Ipelet {
public:
	virtual int ipelibVersion() const {return IPELIB_VERSION;}
	virtual bool run(int, IpeletData *data, IpeletHelper *helper);
private:

};

bool IO_Test_Ipelet::run(int, IpeletData *data, IpeletHelper *helper)
{
	std::vector<Vector> p_tmp;
	std::vector<const SubPath*> sp_tmp;
	std::vector<CurveSegment> cs_tmp;

	IPEIO points;
	IPEIO polys;
	IPEIO segs;
	
	points.set_color("red");
	polys.set_color(0,255,0);
	segs.set_color(0,0,255);

	Get_polygons(data,helper,sp_tmp);
	Get_points(data,helper,p_tmp);
	Get_segments(data,helper,false,cs_tmp);


	std::vector<Point> sites;
	for(auto p : p_tmp)
		sites.push_back(Point(p.x,p.y));

	L1_voronoi voronoi(sites);
	std::vector<Edge> lines = voronoi.getBoundary();
	//ipe::Curve *c = new ipe::Curve;
	for(auto line : lines){
		Point org = line.getOrigin(), dest = line.getDest();
		ipe::Vector v1(org.getx(),org.gety());
		ipe::Vector v2(dest.getx(), dest.gety());
		segs.Draw_segment(data,helper,v1,v2);
	}
	/*
	for(int i =0 ; i < c->countSegments();i++)
		segs.Draw_segment(data,helper,c->segment(i));
*/
	return true;
}

IPELET_DECLARE Ipelet *newIpelet()
{
  return new IO_Test_Ipelet;
}