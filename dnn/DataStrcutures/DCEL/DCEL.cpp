#include "DCEL.h"
#include "Point.h"
#include "../AVLTree/AVLTree.h"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

Vertex::Vertex() : Point() {
	this->incidentEdge = nullptr;
}

Vertex::Vertex(HEdge *_e) : Point() {
	this->incidentEdge = _e;
}

Vertex::Vertex(Point *_p) : Point(_p) {
	this->incidentEdge = nullptr;
}

Vertex::Vertex(Point *_p, HEdge *_e) : Point(_p) {
	this->incidentEdge = _e;
}

Vertex::~Vertex() {

}

void Vertex::setIncidentEdge(HEdge *_e) {
	this->incidentEdge = _e;
}

HEdge* Vertex::getIncidentEdge() {
	return this->incidentEdge;
}

HEdge::HEdge() : Edge() {
	this->origin = nullptr;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
}

HEdge::HEdge(Vertex *_v1, Vertex *_v2) : Edge(_v1, _v2) {
	this->origin = _v1;
	this->twin = new HEdge();
	this->twin->origin = _v2;
	this->twin->twin = this;
	this->incidentFace = nullptr;
	this->next = this->twin;
	this->prev = this->twin;
	this->twin->next = this;
	this->twin->prev = this;
	this->twin->s = _v2;
	this->twin->t = _v1;
}

HEdge::~HEdge() {
	if (!this->twin) {
		delete(this->twin);
	}
}

Vertex* HEdge::getOrigin() {
	return this->origin;
}

HEdge* HEdge::getNext() {
	return this->next;
}

HEdge* HEdge::getPrev() {
	return this->prev;
}

HEdge* HEdge::getTwin() {
	return this->twin;
}

void HEdge::setIncidentFace(Face *_f) {
	this->incidentFace = _f;
}

Face::Face() {
	this->outer = nullptr;
	this->inners = new std::vector<HEdge*>();
}

Face::~Face() {
	delete(this->inners);
}

bool Face::isOutMost() {
	return !outer;
}

void Face::setOuter(HEdge *_e) {
	this->outer = _e;
}

HEdge* Face::getOuter() {
	return this->outer;
}

std::vector<HEdge*>* Face::getInners() {
	return this->inners;
}

DCEL::DCEL() {
	this->faces = new std::vector<Face*>();
	this->hedges = new std::vector<HEdge*>();
	this->vertices = new std::vector<Vertex*>();
	lmost = nullptr;
	tmost = nullptr;
	bmost = nullptr;
	rmost = nullptr;
	Face *of = new Face();
	this->faces->push_back(of);
}

DCEL::~DCEL() {
	delete(this->faces);
	delete(this->hedges);
	delete(this->vertices);
}

std::vector<Face*>* DCEL::getFaces() {
	return this->faces;
}

std::vector<HEdge*>* DCEL::getHedges() {
	return this->hedges;
}

std::vector<Vertex*>* DCEL::getVertices() {
	return this->vertices;
}

DCEL* DCEL::mergeDCEL(DCEL* _d) {
	enum Event {
		START,
		END,
		CROSS
	};
	struct Edgetype {
		HEdge* he;
		double* curx;
		bool operator<(struct Edgetype _e) {
			Point ts = *this->he->gets();
			Point tt = *this->he->gett();
			Point es = *_e.he->gets();
			Point et = *_e.he->gets();
			double ty, ey;
			if (ts.getx() == tt.getx())
				ty = ts.gety();
			else
				ty = (tt.gety() - ts.gety()) / (tt.getx() - ts.getx())*(*curx - ts.getx()) + ts.gety();
			if (es.getx() == et.getx())
				ey = es.gety();
			else
				ey = (et.gety() - es.gety()) / (et.getx() - es.getx())*(*curx - es.getx()) + es.gety();
		}
		bool operator==(struct Edgetype _e) {
			return *he == *_e.he;
		}
	};
	struct Eventtype {
		struct AVLTreeNode<struct Edgetype>*e1, *e2;
		Vertex* eventPoint;
		enum Event ty;
		bool operator<(struct Eventtype _e) {

		}
	};
	DCEL* merged = new DCEL();
	std::vector<HEdge*> hedgevec = *merged->hedges;
	hedgevec.insert(hedgevec.begin(), this->hedges->begin(), this->hedges->end());
	hedgevec.insert(hedgevec.begin(), _d->hedges->begin(), _d->hedges->end());

}