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

	String ipe_v1("v9");
	String ipe_v2("v12");
	helper->getString("Vertex ID",ipe_v1);
	helper->getString("Vertex ID",ipe_v2);


	int i =0;
	char *v1_id = new char[ipe_v1.size()+1];		//크기에 영향을 받는다고 ??
	char *v2_id = new char[ipe_v2.size()+1];
	
	for(i = 0 ; i < ipe_v1.size(); i++){
		v1_id[i] = ipe_v1[i];
	}
	v1_id[i] = '\0';
	Vertex *v1 = dcel.searchVertex(v1_id);


	for(i = 0 ; i < ipe_v2.size(); i++){
		v2_id[i] = ipe_v2[i];
	}
	v2_id[i] = '\0';
	Vertex *v2 = dcel.searchVertex(v2_id);
	
	//char buf[10];
	//sprintf(buf,"%d",ipe_v1.size());
	//helper->messageBox(v2_id,0,1);
	dcel.addEdge(v1,v2);

	char temp_fname[30]="C:\\ipe-7.2.17\\temp.txt";
	fp = fopen(temp_fname,"w");
	dcel.DCELtotext(fp);
	fclose(fp);


	delete v1_id;
	delete v2_id;

	double width  = 2; // dcel.getRmost()->getx() - dcel.getLmost()->getx();
	double height = 2; // dcel.getTmost()->gety() - dcel.getBmost()->gety();
	double ipe_width = 60, ipe_height = 60;
	double ipe_x = 100, ipe_y = 100;

	for(auto f_p : *(dcel.getFaces())){
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