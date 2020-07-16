#include "ipelet.h"
#include "ipepage.h"
#include "../../IpeIO/ipeIO.h"
#include "../../../DataStrcutures/DCEL/DCEL.h"
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

	String file_name = helper->getParameter("f_name");
	char f_name[100] = {0,};
	for(int i = 0 ; i < file_name.size(); i++){
		f_name[i] = file_name[i];
	}

	FILE* fp = fopen(f_name,"r");
	DCEL dcel(fp);
	fclose(fp);

	double width  = 2; // dcel.getRmost()->getx() - dcel.getLmost()->getx();
	double height = 2; // dcel.getTmost()->gety() - dcel.getBmost()->gety();
	double ipe_width = 60, ipe_height = 60;
	double ipe_x = 100, ipe_y = 100;

	for(auto f_p : *dcel.getFaces()){
		for(auto e_p : *f_p->getInners()){
			if (e_p == nullptr) continue;
			auto cur = e_p;
			do{
				Vertex org = *cur->getOrigin();
				Vertex dest = *cur->getNext()->getOrigin();
				ipe::Vector v1(org.getx()/width*ipe_width +ipe_x,org.gety()/height * ipe_height +ipe_y);
				ipe::Vector v2(dest.getx()/width*ipe_width +ipe_x,dest.gety()/height * ipe_height +ipe_y);
				segs.Draw_segment(data,helper,v1,v2);
				cur = cur->getNext();
			}while(cur != e_p);
		}

		auto e_p = f_p->getOuter();
		if(e_p == nullptr) continue;
		auto cur = e_p;
		do{
			Vertex org  = *cur->getOrigin();
			Vertex dest = *cur->getNext()->getOrigin();
			ipe::Vector v1(org.getx() / width * ipe_width + ipe_x, org.gety() / height * ipe_height + ipe_y);
			ipe::Vector v2(dest.getx() / width * ipe_width + ipe_x, dest.gety() / height * ipe_height + ipe_y);
			segs.Draw_segment(data,helper,v1,v2);
			cur = cur->getNext();
		}while(cur != e_p);
	}

	return true;
}

IPELET_DECLARE Ipelet *newIpelet()
{
  return new IO_Test_Ipelet;
}