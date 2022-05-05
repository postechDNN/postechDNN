#define _CRT_SECURE_NO_WARNINGS
#include "DCEL.h"
#include "Edge.h"
#include "Point.h"
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

Vertex::Vertex() : Point() {
	this->key = ""; 
	this->incidentEdge = nullptr; 
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(HEdge* _e) : Point() {
	this->key = ""; 
	this->incidentEdge = _e;
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(Point& _p) : Point(_p) {
	this->key = "";
	this->incidentEdge = nullptr;
}

//CAUTION: vertex is a origin of half edge, vertex key need to be defined.
Vertex::Vertex(Point& _p, HEdge* _e) : Point(_p) {
	this->key = "";
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

HEdge::HEdge() : Edge() {
	this->key = "";
	this->origin = nullptr;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
}

HEdge::HEdge(Vertex* _v1, Vertex* _v2) : Edge(*_v1,*_v2) {
	this->key = "";
	this->origin = _v1;
	this->twin = new HEdge();
	this->twin->origin =_v2;
	this->twin->twin = this;
	this->incidentFace = nullptr;
	this->next = this->twin;
	this->prev = this->twin;
	this->twin->next = this;
	this->twin->prev = this;
	this->twin->s = *_v2;
	this->twin->t = *_v1;
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

Face::Face() {
	this->key = "";
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

//CAUTION : VECTOR ACCESS AND DELETION 

DCEL::DCEL() {
	this->num_faces = 0;
	this->num_hedges = 0;
	this->num_vertices = 0;
	lmost = nullptr;
	tmost = nullptr;
	bmost = nullptr;
	rmost = nullptr;
	Face *of = new Face();
	this->faces.push_back(of);
}

DCEL::~DCEL() {
}

std::vector<Face*> DCEL::getFaces() {
	return this->faces;
}

void DCEL::setFaces(std::vector<Face*> _f) {
	this->faces = _f;
}

std::vector<HEdge*> DCEL::getHedges() {
	return this->hedges;
}

void DCEL::setHedges(std::vector<HEdge*> _e) {
	this->hedges = _e;
}

std::vector<Vertex*> DCEL::getVertices() {
	return this->vertices;
}

void DCEL::setVertices(std::vector<Vertex*> _v) {
	this->vertices = _v;
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

Vertex* DCEL::getLmost() { return this->lmost; }
Vertex* DCEL::getRmost() { return this->rmost; }
Vertex* DCEL::getTmost() { return this->tmost; }
Vertex* DCEL::getBmost() { return this->bmost; }

void DCEL::addVertex(Point& _p, const std::string& key){
	Vertex *v = new Vertex(_p);
	v->setKey(key);
}

HEdge* DCEL::searchHedge(const std::string& key) {
	for(auto it:this->hedges){
		if (it->getKey() ==key)
			return it;
		if(it->getTwin()->getKey() == key)
			return it;
	}

	//for (int i = 0; i < this->hedges.size(); i++) {
	//	if (strcmp((this->hedges)[i]->getHedgeKey(), key) == 0)
	//		return (this->hedges)[i];
	//	if(strcmp((this->hedges)[i]->getTwin()->getHedgeKey(), key) == 0)
	//		return (this->hedges)[i]->getTwin();
	//}
	return nullptr;
}

Edge HEdge::getEdge(){
	Point p1 = *(this->origin);
	Point p2 = *(this->twin->origin);

	return Edge(p1,p2);
}

Vertex* DCEL::searchVertex(const std::string& key) {
	for(auto it:this->vertices){
		if(it->getKey() == key) return it;
	}
	return nullptr;

}

Face* DCEL::searchFace(const std::string& key) {
	for(auto it:this->faces){
		if(it->getKey() == key) return it;
	}
	return nullptr;

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

DCEL DCEL::mergeDCEL(DCEL& op){
	std::vector<HEdge*> S1 = this->hedges;
	std::vector<HEdge*> S2 = op.getHedges();
	std::vector<HEdge*> ret_hedges; 
}
