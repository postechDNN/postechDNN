// Ipelet for finding convex hull using graham scan

#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipereference.h"
#include "ipepath.h"
#include "ipebase.h"
#include "ipeshape.h"
#include "ipeIO.h"

#include <vector>
#include <string>

using namespace ipe;
using namespace std;

bool Get_points(IpeletData *data,IpeletHelper *helper,vector<Vector> &ret){
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if(sel<0){
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
	else return true;
}
bool Get_segments(IpeletData *data, IpeletHelper *helper, bool only_single_subpath, std::vector<CurveSegment> &ret){
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if(sel<0){
		helper->message("No selection");
		return false;
	}
	int n=page->count();
	ret.clear();
	//copy segment(path)
	for(int i=0;i<n;i++){
		//check the object type is path
		if(page->object(i)->type()!=Object::EPath) continue;
		Path *path=page->object(i)->asPath();
		const Shape sh = path->shape();
		//check the singularity
		if(only_single_subpath && !(sh.isSegment())) continue;
		//insert segments into ret
		const Curve *cv = sh.subPath(0)->asCurve();
		for(int j=0;j<cv->countSegments();j++){
			ret.push_back(cv->segment(j));
		}
	}

	if (ret.size()==0) {
		helper->message("No segment selection");
		return false;
	}
	else return true;
}
bool Get_polygons(IpeletData *data,IpeletHelper *helper,vector<const SubPath*> &ret){
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if(sel<0){
		helper->message("No selection");
		return false;
	}
	int n=page->count();
	ret.clear();
	//copy polygon(subpath)
	for(int i=0;i<n;i++){
		//check the object type is path
		if(page->object(i)->type()!=Object::EPath) continue;
		Path *path=page->object(i)->asPath();
		const Curve *cv = (path->shape()).subPath(0)->asCurve();
		//check the closedness
		if(!(cv->closed())) continue;
		//insert polygon(subpath) into ret
		ret.push_back((path->shape()).subPath(0));
	}

	if (ret.size()==0) {
		helper->message("No polygon selection");
		return false;
	}
	else return true;
}

std::vector<Vector> SubPath2Vec(const SubPath *subpath, bool add_first){
	std::vector<Vector> ret;
	const Curve *cv = subpath->asCurve();
	for (int i=0; i<cv->countSegments();i++){
		const CurveSegment &tmp = cv->segment(i);
		ret.push_back(tmp.cp(0));
	}
	if (add_first) ret.push_back(cv->segment(0).cp(0));
	return ret;
}

Vector getFirst(CurveSegment cs){
	return cs.cp(0);
}
Vector getSecond(CurveSegment cs){
	return cs.cp(1);
}

bool IPEIO::Draw_point(IpeletData *data, IpeletHelper *helper, Vector p){
	Attribute tAtr(true,String("mark/disk(sx)"));
	Reference* pt = new Reference(data->iAttributes,tAtr,p);
	pt->setStroke(color_attr);
	data->iPage->append(ESecondarySelected,data->iLayer,pt);
	return true;
}
bool IPEIO::Draw_segment(IpeletData *data, IpeletHelper *helper, Vector first, Vector second){
	Curve *sp = new Curve;
	sp->appendSegment(first,second);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes,shape);
	obj->setStroke(color_attr);
	data->iPage->append(ESecondarySelected,data->iLayer,obj);
	return true;
}
bool IPEIO::Draw_segment(IpeletData *data, IpeletHelper *helper, const CurveSegment &cs){
	return Draw_segment(data,helper,getFirst(cs),getSecond(cs));
}
bool IPEIO::Draw_polygon(IpeletData *data, IpeletHelper *helper, std::vector<Vector> pts, bool closed){
	Curve *sp=new Curve;
	for(int i=0;i<pts.size()-1;i++){
		sp->appendSegment(pts[i],pts[i+1]);
	}
	sp->setClosed(closed);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes, shape);
	obj->setStroke(color_attr);
	data->iPage->append(ESecondarySelected, data->iLayer, obj);
	return true;
}
bool IPEIO::Draw_polygon(IpeletData *data, IpeletHelper *helper, SubPath *sp, bool closed){
	return Draw_polygon(data,helper,SubPath2Vec(sp,false),closed);
}

void IPEIO::set_color(string color){
	color_attr=color_attr.makeColor(color.c_str(),color_attr);
}
void IPEIO::set_color(int r,int g,int b){
	string s = to_string(r)+" "+to_string(g)+" "+to_string(b);
	set_color(s);
}