#define _CRT_SECURE_NO_WARNINGS
#include "DCEL.h"
#include "./..//..//Algorithms/ESP_plane/Edge.h"
#include "./..//..//Algorithms/ESP_plane/Point.h"
//#include "SimplePolygon.h"
#include "Vector.h"
#include "AVLTree.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>
#define M_PI 3.14159265358979323846

int _default_v_key = 0;
int _default_e_key = 0;
int _default_f_key = 0;
int _default_dcel_key = 0;

// 

Vertex::Vertex() : Point() {
	this->key = "v_"+std::to_string(_default_v_key++); 
	this->incidentEdge = nullptr; 
}

Vertex::Vertex(double x, double y) : Point(x,y) {
	this->key = "v_"+std::to_string(_default_v_key++); 
	this->incidentEdge = nullptr; 
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(HEdge* _e) : Point() {
	this->key = "v_"+std::to_string(_default_v_key++); 
	this->incidentEdge = _e;
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(const Point& _p) : Point(_p) {
	this->key = "v_"+std::to_string(_default_v_key++); 
	this->incidentEdge = nullptr;
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(Point& _p, HEdge* _e) : Point(_p) {
	this->key = "v_"+std::to_string(_default_v_key++); 
	this->incidentEdge = _e;
}


Vertex::~Vertex() {
}

std::string Vertex::getKey() {
	return this->key;
}

void Vertex::setKey(const std::string& key) {
	this->key = key;
}

Point Vertex::getPoint(){
	return Point(*this);
}

void Vertex::setIncidentEdge(HEdge *_e) {
	this->incidentEdge = _e;
}

HEdge* Vertex::getIncidentEdge() {
	return this->incidentEdge;
}

HEdge::HEdge() {
	this->key = "e_"+std::to_string(_default_e_key++); 
	this->origin = nullptr;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
	this->type = true; // true - transparent, false - opaque
	this->length = -1;
}

HEdge::HEdge(Vertex* _v1, Vertex* _v2, bool _type) {
	this->key = "e_"+std::to_string(_default_e_key++); 
	this->origin = _v1;
	this->twin = new HEdge();
	this->twin->origin =_v2;
	this->twin->twin = this;
	this->incidentFace = nullptr;
	this->next = this->twin;
	this->prev = this->twin;
	this->twin->next = this;
	this->twin->prev = this;
	this->type = true; // true - transparent, false - opaque
	this->length = sqrt(pow(_v1->getX() - _v2->getX(), 2) + pow(_v1->getY() - _v2->getY(), 2));

	// HWI added. is it correct?
	this->origin->setIncidentEdge(this);
	this->twin->origin->setIncidentEdge(this->twin);
	//this->twin->s = *_v2;
	//this->twin->t = *_v1;
}

/*
HEdge::HEdge(Point& _p1, Point& _p2) {
	Vertex* v1 = new Vertex(_p1);
	Vertex* v2 = new Vertex(_p2);
	this->s = v1;
	this->t = v2;
	this->origin = v1;
	this->twin = new HEdge();
	this->twin->origin = v2;
	this->twin->twin = this;
	this->incidentFace = nullptr;
	this->next = this->twin;
	this->prev = this->twin;
	this->twin->next = this;
	this->twin->prev = this;
	this->twin->s = v2;
	this->twin->t = v1;
}
*/

//CAUTION: DO NOT DELETE TWIN
HEdge::~HEdge() {
	//if (this->twin)	
	//	delete this->twin;
}

std::string HEdge::getKey() {
	return this->key;
}

void HEdge::setKey(const std::string& key) {
	this->key = key;
}

Vertex* HEdge::getOrigin() {
	return this->origin;
}

void HEdge::setOrigin(Vertex* _v) {
	this->origin = _v;
}


HEdge* HEdge::getNext() {
	return this->next;
}

void HEdge::setNext(HEdge* _e) {
	this->next = _e;
}


HEdge* HEdge::getPrev() {
	return this->prev;
}

void HEdge::setPrev(HEdge* _e) {
	this->prev = _e;
}


HEdge* HEdge::getTwin() {
	return this->twin;
}

void HEdge::setTwin(HEdge* _e) {
	this->twin = _e;
}


Face* HEdge::getIncidentFace() {
	return this->incidentFace;
}

void HEdge::setIncidentFace(Face *_f) {
	this->incidentFace = _f;
}

//If there is no crossing, return nullptr
//Else, return a pointer of the point crossed. 
Point* HEdge::crossing(HEdge& _e, bool closed) {
	double x_1 = this->origin->getx();
	double y_1 = this->origin->gety();
	double x_2 = this->twin->origin->getx();
	double y_2 = this->twin->origin->gety();
	double x_3 = _e.origin->getx();
	double y_3 = _e.origin->gety();
	double x_4 = _e.twin->origin->getx();
	double y_4 = _e.twin->origin->gety();

	double d = (y_4 - y_3) * (x_2 - x_1) - (x_4 - x_3) * (y_2 - y_1);	// (x1,y1) ~ (x2,y2) and (x3,y3) ~ (x4,y4)
	double t = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);	// (x1,y1) ~ (x3,y3) and (x3,y3) ~ (x4,y4)
	double s = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);	// (x1,y1) ~ (x2,y2) and (x1,y1) ~ (x3,y3)
	if (std::abs(d) < ERR) {	//two line segment have same slope.
		if (std::abs(t) < ERR) {	//two line segment lies on same line.
			if (std::abs(x_1 - x_2) < ERR) {	//Vertical Line
				if (std::max(y_1, y_2) < std::min(y_3, y_4) || std::min(y_1, y_2) > std::max(y_3, y_4))	return nullptr;
				else if (!closed && (std::abs(std::max(y_1, y_2) - std::min(y_3, y_4)) < ERR || std::abs(std::min(y_1, y_2) - std::max(y_3, y_4)) < ERR)) return nullptr;
				else {	//Intersect!
					//return new Point(x_1,middle_point_of_4(y_1,y_2,y_3,y_4));
					std::pair<double, double> mid_pts = find_mid_points(y_1, y_2, y_3, y_4);
					// return new Edge(Point(x_1, mid_pts.first), Point(x_1, mid_pts.second));
					return new Point(x_1, mid_pts.second);
				}
			}
			else {
				if (std::max(x_1, x_2) < std::min(x_3, x_4) || std::min(x_1, x_2) > std::max(x_3, x_4))	return nullptr;
				else if (!closed && (std::abs(std::max(x_1, x_2) - std::min(x_3, x_4)) < ERR || std::abs(std::min(x_1, x_2) - std::max(x_3, x_4)) < ERR)) return nullptr;
				else {
					//double mid_x = middle_point_of_4(x_1,x_2,x_3,x_4);
					//return new Point(mid_x, (y_1 -y_2) / (x_1-x_2) *(mid_x - x_1) + y_1);
					std::pair<double, double> mid_pts = find_mid_points(x_1, x_2, x_3, x_4);
					// return new Edge(Point(mid_pts.first, (y_1 - y_2) / (x_1 - x_2) * (mid_pts.first - x_1) + y_1),
					//		Point(mid_pts.second, (y_1 - y_2) / (x_1 - x_2) * (mid_pts.second - x_1) + y_1));
					return new Point(mid_pts.second, (y_1 - y_2) / (x_1 - x_2) * (mid_pts.second - x_1) + y_1);
				}
			}
		}
		else return nullptr;
	}
	else {
		t = t / d;
		s = s / d;
		if (t > 1 || s > 1 || t < 0 || s < 0) {
			return nullptr;
		}
		else if ((std::abs(t) < ERR || std::abs(t - 1.) < ERR || std::abs(s) < ERR || std::abs(s - 1.) < ERR) && !closed) {
			return nullptr;
		}
		else {
			double x = (1 - t) * x_1 + t * x_2;
			double y = (1 - t) * y_1 + t * y_2;
			//Point* P = new Point(x, y);
			// return new Edge(Point(x, y), Point(x, y));
			return new Point(x, y);
		}
	}
}

Face::Face() {
	this->key = "f_"+std::to_string(_default_f_key++); 
	this->outer = nullptr;
}

Face::~Face() {
}

std::string Face::getKey() {
	return this->key;
}

void Face::setKey(const std::string& key) {
	this->key = key;
}


bool Face::isOutMost() {
	return !outer;
}

void Face::setOuter(HEdge* _e) {
	this->outer = _e;
}

HEdge* Face::getOuter() {
	return this->outer;
}

std::vector<HEdge*> Face::getInners() {
	return this->inners;
}

void Face::addInner(HEdge* _e) {
	this->inners.push_back(_e);
}

std::vector<HEdge *> Face::getOutHEdges(){
	std::vector<HEdge *> ret;
	if(this->outer == nullptr) // it is outmost face
		return ret;
	auto cur = this->outer;
	do{
		ret.push_back(cur);
		cur = cur->getNext();
	}while(cur != this->outer);

	return ret;
}

std::vector<HEdge*> Face::getInnerHEdges(){
	std::vector<HEdge *> ret;
	for(auto he_ptr:this->inners){
		auto cur =he_ptr;
		do{
			ret.push_back(cur);
			cur = cur->getNext();
		}while(cur != he_ptr);
	}
	return ret;
}

//CAUTION : VECTOR ACCESS AND DELETION 

DCEL::DCEL() {
	//this->num_faces = 0;
	//this->num_hedges = 0;
	//this->num_vertices = 0;
	this->key = "dcel_"+std::to_string(_default_dcel_key++); 

	Face *of = new Face();
	of->setOuter(nullptr);
	this->faces[of->getKey()] = of;
}

DCEL::~DCEL() {
	for(auto v : this->vertices)
		delete v.second;
	for(auto he: this->hedges)
		delete he.second;
	for(auto f : this->faces)
		delete f.second;
}

std::vector<Face*> DCEL::getFaces() {
	std::vector<Face*> ret;
	for(auto it:this->faces) ret.push_back(it.second);
	return ret;
}

void DCEL::setFaces(std::vector<Face*> _f) {
	for(auto f: this->faces){
		delete f.second;
	}
	this->faces.clear();
	for(auto f:_f){
		this->faces[f->getKey()]=f;
	}
	//this->faces = _f;
	//this->num_faces = _f.size();
}

std::vector<HEdge*> DCEL::getHedges() {
	std::vector<HEdge*> ret;
	for(auto it:this->hedges) ret.push_back(it.second);
	return ret;
}

void DCEL::setHedges(std::vector<HEdge*> _e) {
	for(auto e:_e){
		this->hedges[e->getKey()] = e;
	}
	//this->hedges = _e;
	//this->num_hedges = _e.size();
}

std::vector<Vertex*> DCEL::getVertices() {
	std::vector<Vertex*> ret;
	for(auto it:this->vertices) ret.push_back(it.second);
	return ret;
}

void DCEL::setVertices(std::vector<Vertex*> _v) {
	for(auto v:_v){
		this->vertices[v->getKey()]=v;
	}
	//this->vertices = _v;
	//this->num_vertices = _v.size();
}

std::string DCEL::getKey() {
	return this->key;
}

void DCEL::setKey(const std::string& key) {
	this->key = key;
}

/*
void DCEL::pushFace(Face* _f){
	this->faces.push_back(_f);
}
void DCEL::pushHEdge(HEdge* _he){
	this->hedges.push_back(_he);
}
void DCEL::pushVertex(Vertex* _v){
	this->vertices.push_back(_v);
}*/

//Vertex* DCEL::getLmost() { return this->lmost; }
//Vertex* DCEL::getRmost() { return this->rmost; }
//Vertex* DCEL::getTmost() { return this->tmost; }
//Vertex* DCEL::getBmost() { return this->bmost; }

//void DCEL::addVertex(Point& _p, const std::string& key){
//	Vertex *v = new Vertex(_p);
//	v->setKey(key);
//}

HEdge* DCEL::getHedge(const std::string& key) {
	auto it = this->hedges.find(key);
	if(it != this->hedges.end()) return (*it).second;
	else return nullptr;
	//for(auto it:this->hedges){
	//	if (it->getKey() ==key)
	//		return it;
	//}
	//return nullptr;
}

Edge HEdge::getEdge(){
	Point p1 = *(this->origin);
	Point p2 = *(this->twin->origin);

	return Edge(p1,p2);
}

Vertex* DCEL::getVertex(const std::string& key) {
	auto it = this->vertices.find(key);
	if(it != this->vertices.end()) return (*it).second;
	else return nullptr;

	//for(auto it:this->vertices){
	//	if(it->getKey() == key) return it;
	//}
	//return nullptr;

}

Face* DCEL::getFace(const std::string& key) {
	auto it = this->faces.find(key);
	if(it != this->faces.end()) return (*it).second;
	else return nullptr;

	//for(auto it:this->faces){
	//	if(it->getKey() == key) return it;
	//}
	//return nullptr;

}

std::vector<HEdge*> DCEL::getOutgoingHEdges(Vertex* v) {
	std::vector<HEdge*> output;
	HEdge* e_original = v->getIncidentEdge();
	HEdge* e = e_original;
	do {
		output.push_back(e);
		e = e->getTwin()->getNext();
	} while (e != e_original);
	return output;
}

std::vector<HEdge*> DCEL::getIncomingHEdges(Vertex* v) {
	std::vector<HEdge*> output;
	HEdge* e_original = v->getIncidentEdge();
	HEdge* e = e_original;
	do {
		output.push_back(e->getTwin());
		e->getTwin()->getNext();
	} while (e != e_original);
	return output;
}

//The point p is inside polygon return 1
//The point p is on boundary return 0
//The point p is outside polygon return -1
/*
int DCEL::inPolygon(std::vector<HEdge*> hedges, Point p) {
	double min_x = 1e10;
	double min_x2 = 1e10;
	int idx = -1;
	double x = p.getx(), y = p.gety();
	for (int i = 0; i < hedges.size(); i++) {
		Edge* _e = hedges[i];
		Point origin = _e->gets(), dest = _e->gett();

		if (origin.gety() > dest.gety()){	//y1 <= y2
			Point tmp = origin;
			origin = dest;
			dest = tmp;
		}
		double x1 = origin.getx(), y1 = origin.gety();
		double x2 = dest.getx(), y2 = dest.gety();

		//Point is on boundary line
		if (abs((y1 - y2) * (x - x1) - (y - y1) * (x1 - x2)) < 1e-6) {
			if ((std::min(y1, y2) <= y && y <= std::max(y1, y2)) && std::min(x1, x2) <= x && x <= std::max(x1, x2))
				return 0;
		}

		if (y1 - 1e-6 < y && y < y2 + 1e-6 && abs(y1 - y2) > 1e-6 && (y - y1) * (x2 - x1) > (x - x1)* (y2 - y1)) {//edge is right to point p
			double tmp = (y - y1) * (x1 - x2) / (y1 - y2) + x1;
			Point tmp_org = Point(x1, y1), tmp_dest = Point(x2, y2);
			if (abs(y - y2) < 1e-6) { tmp_org = Point(x2, y2); tmp_dest = Point(x1, y1); }
			//Normalize x2
			double normal_x2 = (tmp_dest - tmp_org).getx() / (tmp_dest - tmp_org).distance(tmp_dest - tmp_org) + tmp_org.getx(); //
			if (min_x > tmp || (abs(min_x - tmp) < 1e-6 && min_x2 > x2)) {
				min_x = tmp;
				min_x2 = x2;
				idx = i;
			}
		}
	}
	if (idx == -1) return -1;
	Point org = hedges[idx]->gets();
	Point dest = hedges[idx]->gett();
	if ((dest - org).gety() > 0)
		return -1;
	else return 1;
}
*/