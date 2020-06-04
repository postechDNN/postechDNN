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

Face* HEdge::getIncidentFace() {
	return this->incidentFace;
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

void DCEL::addEdge(Vertex* _v1, Vertex* _v2) {
	HEdge* e = new HEdge(_v1, _v2);
	this->vertices->push_back(_v1);
	this->vertices->push_back(_v2);
	this->hedges->push_back(e);
	double min = 10E6;
	HEdge* nearest_e = nullptr;
	for (std::vector<HEdge*>::iterator i = this->getHedges()->begin(); i != this->getHedges()->end(); i++) {
		HEdge* he = *i;
		double s_x = he->gets()->getx();
		double s_y = he->gets()->gety();
		double t_x = he->gett()->getx();
		double t_y = he->gett()->gety();
		double x = _v1->getx();
		double y = _v1->gety();
		double _y = (t_y - s_y) * (x - s_x) / (t_x - s_x) + s_y;
		if (min > abs(y - _y) && ((t_y - _y) * (s_y - _y) <= 0)) {
			min = abs(y - _y);
			nearest_e = he;
		}
	}

	if (nearest_e->getIncidentFace() == nearest_e->getTwin()->getIncidentFace()) {
		e->setIncidentFace(nearest_e->getIncidentFace());
	}
	else {
		double s_x = nearest_e->gets()->getx();
		double s_y = nearest_e->gets()->gety();
		double t_x = nearest_e->gett()->getx();
		double t_y = nearest_e->gett()->gety();
		double x = _v1->getx();
		double y = _v1->gety();
		double temp = (s_x * t_y + t_x * y + x * s_y) - (s_x * y + t_x * s_y + x * t_y);
		if (temp > 0) {
			e->setIncidentFace(nearest_e->getIncidentFace());
		} else if (temp < 0) {
			e->setIncidentFace(nearest_e->getTwin()->getIncidentFace());
		}
		//temp==0
	}

	if (lmost == nullptr) {
		if ((_v1->getx() >= _v2->getx()) && (_v1->gety() >= _v2->gety())) {
			lmost = _v2;
			rmost = _v1;
			tmost = _v1;
			bmost = _v2;
		} else if ((_v1->getx() >= _v2->getx()) && (_v1->gety() < _v2->gety())) {
			lmost = _v2;
			rmost = _v1;
			tmost = _v2;
			bmost = _v1;
		} else if ((_v1->getx() < _v2->getx()) && (_v1->gety() >= _v2->gety())) {
			lmost = _v1;
			rmost = _v2;
			tmost = _v1;
			bmost = _v2;
		} else if ((_v1->getx() < _v2->getx()) && (_v1->gety() < _v2->gety())) {
			lmost = _v1;
			rmost = _v2;
			tmost = _v2;
			bmost = _v1;
		}
	} else {
		if (lmost->getx() > _v1->getx()) {
			lmost = _v1;
		}
		if (lmost->getx() > _v2->getx()) {
			lmost = _v2;
		}
		if (rmost->getx() < _v1->getx()) {
			rmost = _v1;
		}
		if (rmost->getx() < _v2->getx()) {
			rmost = _v2;
		}
		if (tmost->gety() < _v1->gety()) {
			tmost = _v1;
		}
		if (tmost->gety() < _v2->gety()) {
			tmost = _v2;
		}
		if (bmost->gety() > _v1->gety()) {
			bmost = _v1;
		}
		if (bmost->gety() > _v2->gety()) {
			bmost = _v2;
		}
	}

}

void DCEL::deleteEdge(HEdge* _e) {
	Vertex* _v1 = _e->getOrigin();
	Vertex* _v2 = _e->getTwin()->getOrigin();
	for (std::vector<Vertex*>::iterator i = this->getVertices()->begin(); i != this->getVertices()->end();) {
		Vertex* _v = *i;
		if ((_v == _v1) || (_v == _v2)) {
			i = this->getVertices()->erase(i);
		} else {
			i++;
		}
	}
	for (std::vector<HEdge*>::iterator i = this->getHedges()->begin(); i != this->getHedges()->end();) {
		HEdge* _ee = *i;
		if (_ee == _e) {
			i = this->getHedges()->erase(i);
		} else {
			i++;
		}
	}
	if ((lmost == _v1) || (lmost == _v2)) {
		double min_x = 0;
		for (std::vector<Vertex*>::iterator i = this->getVertices()->begin(); i != this->getVertices()->end(); i++) {
			Vertex* _v = *i;
			if (min_x > _v->getx()) {
				min_x = _v->getx();
				lmost = _v;
			}
		}
	}
	if ((lmost == _v1) || (lmost == _v2)) {
		double max_x = 0;
		for (std::vector<Vertex*>::iterator i = this->getVertices()->begin(); i != this->getVertices()->end(); i++) {
			Vertex* _v = *i;
			if (max_x < _v->getx()) {
				max_x = _v->getx();
				rmost = _v;
			}
		}
	}
	if ((lmost == _v1) || (lmost == _v2)) {
		double min_y = 0;
		for (std::vector<Vertex*>::iterator i = this->getVertices()->begin(); i != this->getVertices()->end(); i++) {
			Vertex* _v = *i;
			if (min_y > _v->gety()) {
				min_y = _v->gety();
				bmost = _v;
			}
		}
	}
	if ((lmost == _v1) || (lmost == _v2)) {
		double max_y = 0;
		for (std::vector<Vertex*>::iterator i = this->getVertices()->begin(); i != this->getVertices()->end(); i++) {
			Vertex* _v = *i;
			if (max_y < _v->gety()) {
				max_y = _v->gety();
				tmost = _v;
			}
		}
	}
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
