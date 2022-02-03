#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipereference.h"
#include "ipepath.h"
#include "ipebase.h"
#include "ipeshape.h"
#include "ipeIO.h"
#include "ipeattributes.h"
#include "ipeobject.h"

#include <vector>
#include <string>

using namespace ipe;
using namespace std;

ipe::Vector applyTransformations(ipe::Vector p, ipe::Matrix transM);
ipe::Matrix applyTransformations(ipe::Matrix ellM, ipe::Matrix transM);

bool Get_poly_aux(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret, bool is_polygon, bool applyTrans = true){
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
		if(page->select(i)!=EPrimarySelected && page->select(i)!=ESecondarySelected) continue;
		Path *path=page->object(i)->asPath();
		const SubPath *sp = (path->shape()).subPath(0);
		//check the closedness
		if(is_polygon && !(sp->closed())) continue;
		if(!is_polygon && sp->closed()) continue;
		const Curve *cv = sp->asCurve();
		if(cv->countSegments()<2) continue;
		//insert polygon(subpath) into ret
		if (applyTrans) {
			//Get transformation matrix
			ipe::Matrix transM = page->object(i)->matrix();
			Curve* tempCurve = new Curve();
			for (int j = 0; j < cv->countSegments(); j++) {
				if ((cv->segment(j)).type() != CurveSegment::ESegment) {
					delete tempCurve;
					tempCurve = NULL;
					break;
				}
				ipe::Vector cp[2];
				std::vector<ipe::Vector> cps;
				cp[0] = applyTransformations(cv->segment(j).cp(0), transM);
				cp[1] = applyTransformations(cv->segment(j).cp(1), transM);
				tempCurve->appendSegment(cp[0], cp[1]);	
			}
			if (tempCurve != NULL) {
				ret.push_back(tempCurve);
			}
		}
		else {
			ret.push_back(sp);
		}
	}

	if (ret.size()==0) {
		helper->message("No selection");
		return false;
	}
	else return true;
}

bool Get_points(IpeletData *data,IpeletHelper *helper, vector<Vector> &ret, bool applyTrans){
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
		if(page->select(i)!=EPrimarySelected && page->select(i)!=ESecondarySelected) continue;
		Reference *ref=page->object(i)->asReference();

		//check it is mark
		String name = ref->name().string(); 
		if(!(name.substr(0,4)==String("mark"))) continue;

		//insert points into ret
		if (applyTrans) {
			ret.push_back(applyTransformations(ref->position(), page->object(i)->matrix()));
		}
		else {
			ret.push_back(ref->position());
		}
	}

	if (ret.size()==0) {
		helper->message("No mark selection");
		return false;
	}
	else return true;
}
bool Get_segments(IpeletData *data, IpeletHelper *helper, bool only_single_subpath, std::vector<CurveSegment> &ret, bool applyTrans){
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
		if(page->select(i)!=EPrimarySelected && page->select(i)!=ESecondarySelected) continue;
		Path *path=page->object(i)->asPath();
		const Shape sh = path->shape();
		//check the singularity
		if(only_single_subpath && !(sh.isSegment())) continue;
		if(sh.subPath(0)->closed()) continue;
		const Curve *cv = sh.subPath(0)->asCurve();
		//Get transformation matrix
		ipe::Matrix transM = page->object(i)->matrix();
		//insert segments into ret
		for(int j=0;j<cv->countSegments();j++){
			if((cv->segment(j)).type()!=CurveSegment::ESegment) continue;
			
			if (applyTrans) {
				ipe::Vector cp[2];
				cp[0] = applyTransformations(cv->segment(j).cp(0), transM);
				cp[1] = applyTransformations(cv->segment(j).cp(1), transM);
				Curve *tempCurve = new Curve();
				tempCurve->appendSegment(cp[0],cp[1]);
				ret.push_back(tempCurve->segment(0));
			}
			else {
				ret.push_back(cv->segment(j));
			}
		}
	}

	if (ret.size()==0) {
		helper->message("No segment selection");
		return false;
	}
	else return true;
}
bool Get_splines(IpeletData *data, IpeletHelper *helper,
	std::vector<CurveSegment> &ret, bool applyTrans){
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
		if(page->select(i)!=EPrimarySelected && page->select(i)!=ESecondarySelected) continue;
		Path *path=page->object(i)->asPath();
		const Shape sh = path->shape();
		if(sh.subPath(0)->closed()) continue;
		const Curve *cv = sh.subPath(0)->asCurve();

		//Get transformation matrix
		ipe::Matrix transM = page->object(i)->matrix();

		//insert splines into ret
		for(int j=0;j<cv->countSegments();j++){
			if((cv->segment(j)).type()!=CurveSegment::ESpline) continue;
			if (applyTrans) {
				std::vector<ipe::Vector> cps;
				for (int k = 0; k < cv->segment(j).countCP(); k++) {
					cps.push_back(applyTransformations(cv->segment(j).cp(k), transM));
				}
				Curve* tempCurve = new Curve();
				tempCurve->appendSpline(cps);
				ret.push_back(tempCurve->segment(0));
			}
			else {
				ret.push_back(cv->segment(j));
			}
		}
	}

	if (ret.size()==0) {
		helper->message("No segment selection");
		return false;
	}
	else return true;
}

bool Get_polygons(IpeletData *data,IpeletHelper *helper,
	vector<const SubPath*> &ret, bool applyTrans){
	return Get_poly_aux(data, helper, ret, true, applyTrans);
}

bool Get_polylines(IpeletData *data, IpeletHelper *helper,
	vector<const SubPath*> &ret, bool applyTrans){
	return Get_poly_aux(data, helper, ret, false, applyTrans);
}

bool Get_circles(IpeletData* data, IpeletHelper* helper,
	std::vector<ipe::Ellipse>& ret, bool applyTrans) {
	Page* page = data->iPage;
	int sel = page->primarySelection();
	if (sel < 0) {
		helper->message("No selection");
		return false;
	}
	int n = page->count();
	ret.clear();
	for (int i = 0; i < n; i++) {
		//check the object type is path
		if (page->object(i)->type() != Object::EPath) continue;
		if (page->select(i) != EPrimarySelected && page->select(i) != ESecondarySelected) continue;
		Path* path = page->object(i)->asPath();
	
		const Shape sh = path->shape();
		if (sh.subPath(0)->type() != ipe::SubPath::EEllipse) continue;
		const ipe::Ellipse* ell = sh.subPath(0)->asEllipse();

		//Get transformation matrix
		ipe::Matrix transM = page->object(i)->matrix();

		if (applyTrans) {
			ipe::Matrix ellM = applyTransformations(ell->matrix(), transM);
			const ipe::Ellipse* trnasEll = new ipe::Ellipse(ellM);
			ret.push_back(*trnasEll);
		}
		else {
			ret.push_back(*ell);
		}
	}

	if (ret.size() == 0) {
		helper->message("No circle(eclipse) selection");
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
	ret.push_back((cv->segment(cv->countSegments()-1)).cp(1));
	if (add_first) ret.push_back(cv->segment(0).cp(0));
	return ret;
}

Vector getFirst(CurveSegment cs){
	return cs.cp(0);
}
Vector getSecond(CurveSegment cs){
	return cs.cp(1);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void IPEIO::reset_attr(int flag=FLAG_EMPTY){
	Attr_flag=flag;
}

void IPEIO::set_Attrs(Reference *obj){
	if (Attr_flag&FLAG_COLOR) obj->setStroke(color_attr);
	if (Attr_flag&FLAG_POINT_SIZE) obj->setSize(pts_size);
}

void IPEIO::set_Attrs(Path *obj){
	if (Attr_flag&FLAG_COLOR) obj->setStroke(color_attr);
	if (Attr_flag&FLAG_ARROW){
		if (farrow) obj->setAttribute(EPropFArrow,construct.ARROW_NORMAL());
		else obj->setAttribute(EPropRArrow,construct.ARROW_NORMAL());
	}
	if (Attr_flag&FLAG_DASH) obj->setDashStyle(dash);
	if (Attr_flag&FLAG_VALUE){
		for (int i=0;i<props.size();i++) obj->setAttribute(props[i],vals[i]);
	}
	if (Attr_flag&FLAG_PEN_WIDTH) obj->setAttribute(EPropPen,pen_width);
	if (Attr_flag&FLAG_FILL) {
		obj->setAttribute(EPropFillColor,fill);
		if (mode) obj->setPathMode(EFilledOnly);
		else obj->setPathMode(EStrokedAndFilled);
	}
}

bool IPEIO::Draw_point(IpeletData *data, IpeletHelper *helper, Vector p){
	Reference* pt = new Reference(data->iAttributes,pts_type,p);
	//pt->setStroke(color_attr);
	set_Attrs(pt);
	data->iPage->append(ESecondarySelected,data->iLayer,pt);
	return true;
}

bool IPEIO::Draw_segment(IpeletData *data, IpeletHelper *helper, Vector first, Vector second){
	Curve *sp = new Curve;
	sp->appendSegment(first,second);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes,shape);
	//obj->setStroke(color_attr);
	set_Attrs(obj);
	data->iPage->append(ESecondarySelected,data->iLayer,obj);
	return true;
}

bool IPEIO::Draw_segment(IpeletData *data, IpeletHelper *helper, const CurveSegment &cs){
	return Draw_segment(data,helper,getFirst(cs),getSecond(cs));
}

bool IPEIO::Draw_poly(IpeletData *data, IpeletHelper *helper, std::vector<Vector> pts, bool closed){
	Curve *sp=new Curve;
	for(int i=0;i<pts.size()-1;i++){
		sp->appendSegment(pts[i],pts[i+1]);
	}
	sp->setClosed(closed);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes, shape);
	//obj->setStroke(color_attr);
	set_Attrs(obj);
	data->iPage->append(ESecondarySelected, data->iLayer, obj);
	return true;
}

bool IPEIO::Draw_poly(IpeletData *data, IpeletHelper *helper, SubPath *sp, bool closed){
	return Draw_poly(data,helper,SubPath2Vec(sp,false),closed);
}

bool IPEIO::Draw_spline(IpeletData *data, IpeletHelper *helper, std::vector<Vector> spline){
	Curve *sp=new Curve;
	sp->appendSpline(spline);
	Shape shape;
	shape.appendSubPath(sp);
	Path *obj = new Path(data->iAttributes, shape);
	//obj->setStroke(color_attr);
	set_Attrs(obj);
	data->iPage->append(ESecondarySelected, data->iLayer, obj);
	return true;
}

bool IPEIO::Draw_circle(IpeletData* data, IpeletHelper* helper, ipe::Ellipse circle) {
	ipe::Ellipse* ell = new ipe::Ellipse(circle.matrix());
	ipe::Shape shape;
	shape.appendSubPath(ell);
	Path* obj = new Path(data->iAttributes, shape);
	set_Attrs(obj);
	data->iPage->append(ESecondarySelected, data->iLayer, obj);
	return true;
}

void IPEIO::set_color(string color){
	color_attr=construct.makeColor(color.c_str(),color_attr);
	Attr_flag|=FLAG_COLOR;
}

void IPEIO::set_color(int r,int g,int b){
	set_color(to_string((float)r/255)+" "+to_string((float)g/255)+" "+to_string((float)b/255));
}

void IPEIO::set_pts_size(int size){ //normal:0  large:1  small:-1  tiny:-2
	String style;
	switch (size){
		case -2: style="tiny";break;
		case -1: style="small";break;
		case 1: style="large";break;
		default: style="normal";
	}
	pts_size=Attribute(true,style);
	Attr_flag|=FLAG_POINT_SIZE;
}

void IPEIO::set_pts_style(int type){ //disk:0 circle:1  square:2 box:3  cross:4
	String style="mark/";
	switch (type){
		case 1: style+="circle";break;
		case 2: style+="square";break;
		case 3: style+="box";break;
		case 4: style+="cross";break;
		default: style+="disk";
	}
	style+="(sx)";
	pts_type=Attribute(true,style);
}

void IPEIO::set_arrow(bool forward=true){
	Attr_flag|=FLAG_ARROW;
	farrow=forward;
}

void IPEIO::set_dash(int type){ // Dot:1  Dash:2  Dash&Dot:3  Dash&Dot&Dot:4  NO:other
	String style;
	switch (type){
		case 1: style="dotted";break;
		case 2: style="dashed";break;
		case 3: style="dash dotted";break;
		case 4: style="dash dot dotted";break;
		default: style="";
	}
	dash=construct.makeDashStyle(style);
	Attr_flag|=FLAG_DASH;
}

void IPEIO::set_pen(int type){ //normal:0  heavier:1  fat:2  ultrafat:3
	String style;
	switch (type){
		case 1: style="heavier";break;
		case 2: style="fat";break;
		case 3: style="ultrafat";break;
		default: style="normal";
	}
	pen_width=Attribute(true,style);
	Attr_flag|=FLAG_PEN_WIDTH;
}

void IPEIO::set_fill(std::string color,bool fillonly=false){
	fill=construct.makeColor(color.c_str(),color_attr);
	mode=fillonly;
	Attr_flag|=FLAG_FILL;
}

void IPEIO::set_fill(int r,int g,int b,bool fillonly=false){
	set_fill(to_string((float)r/255)+" "+to_string((float)g/255)+" "+to_string((float)b/255),fillonly);
}

void IPEIO::add_Attribute(Property prop, Attribute val){
	props.push_back(prop);
	vals.push_back(val);
}

ipe::Vector applyTransformations(ipe::Vector p, ipe::Matrix transM) {
	double dx, dy;
	dx = transM.a[0] * p.x + transM.a[2] * p.y + transM.a[4];
	dy = transM.a[1] * p.x + transM.a[3] * p.y + transM.a[5];
	return ipe::Vector(dx,dy);
}

ipe::Matrix applyTransformations(ipe::Matrix ellM, ipe::Matrix transM) {
	ipe::Matrix ret;
	for (int i = 0; i < 4; i+=2) {
		ret.a[i] = transM.a[0] * ellM.a[i] + transM.a[2] * ellM.a[i + 1];
		ret.a[i+1] = transM.a[1] * ellM.a[i] + transM.a[3] * ellM.a[i + 1];
	}
	ret.a[4] = transM.a[0] * ellM.a[4] + transM.a[2] * ellM.a[5] + transM.a[4];
	ret.a[5] = transM.a[1] * ellM.a[4] + transM.a[3] * ellM.a[5] + transM.a[5];
	return ret;
}