#include "DCEL.h"
#include "Point.h"
#include "../AVLTree/AVLTree.h"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

Vertex::Vertex() : Point<double>() {
	this->incidentEdge = nullptr;
}

Vertex::Vertex(HEdge *_e) : Point<double>() {
	this->incidentEdge = _e;
}

Vertex::Vertex(Point<double> *_p) : Point<double>(_p) {
	this->incidentEdge = nullptr;
}

Vertex::Vertex(Point<double> *_p, HEdge *_e) : Point(_p) {
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
	struct Eventtype;
	struct Edgetype {
		HEdge* he;
		double* curx;
		struct Eventtype *cd, *cu;
		bool operator<(struct Edgetype _e) {
			Point<double> ts = *this->he->gets();
			Point<double> tt = *this->he->gett();
			Point<double> es = *_e.he->gets();
			Point<double> et = *_e.he->gets();
			double ty, ey;
			if (ts.getx() == tt.getx())
				ty = ts.gety();
			else
				ty = (tt.gety() - ts.gety()) / (tt.getx() - ts.getx())*(*curx - ts.getx()) + ts.gety();
			if (es.getx() == et.getx())
				ey = es.gety();
			else
				ey = (et.gety() - es.gety()) / (et.getx() - es.getx())*(*curx - es.getx()) + es.gety();
			if (ty == ey) {
				if (ts.getx() == tt.getx())
					return true;
				else if (es.getx() == et.getx())
					return false;
				else {
					ty = (tt.gety() - ts.gety()) / (tt.getx() - ts.getx())*(*curx + 1 - ts.getx()) + ts.gety();
					ey = (et.gety() - es.gety()) / (et.getx() - es.getx())*(*curx + 1 - es.getx()) + es.gety();
					return ty < ey;
				}
			}
			else
				return ty < ey;
		}
		bool operator==(struct Edgetype _e) {
			return *he == *_e.he;
		}
	};
	struct Eventtype {
		struct Edgetype *e1, *e2;
		Point<double>* eventPoint;
		Event ty;
		bool operator<(struct Eventtype _e) {
			if (*eventPoint == *_e.eventPoint) {
				if (ty == CROSS) {
					return true;
				}
				else if (_e.ty == CROSS) {
					return false;
				}
				else if ((ty == _e.ty) && (ty == START)) {
					if (e1->he->gets()->getx() == e1->he->gett()->getx())
						return false;
					else if (_e.e1->he->gets()->getx() == e1->he->gett()->getx())
						return true;
					else
						return (e1->he->gett()->gety() - eventPoint->gety()) / (e1->he->gett()->getx() - eventPoint->getx()) < (e1->he->gett()->gety() - eventPoint->gety()) / (e1->he->gett()->getx() - eventPoint->getx());
				}
				else if (ty == START)
					return false;
				else if (_e.ty == START) {
					return true;
				}
				else {
					if (e1->he->gets()->getx() == e1->he->gett()->getx())
						return false;
					else if (_e.e1->he->gets()->getx() == e1->he->gett()->getx())
						return true;
					else
						return (e1->he->gett()->gety() - eventPoint->gety()) / (e1->he->gett()->getx() - eventPoint->getx()) > (e1->he->gett()->gety() - eventPoint->gety()) / (e1->he->gett()->getx() - eventPoint->getx());
				}
			}
			else if (eventPoint->getx() == _e.eventPoint->getx())
				return eventPoint->gety() < eventPoint->gety();
			else
				return eventPoint->getx() < eventPoint->getx();
		}
	};
	DCEL* merged = new DCEL();
	AVLTree<struct Eventtype> events;
	AVLTree<struct Edgetype> curedges;
	double curx;
	for (int i = 0; i < this->hedges->size(); i++) {
		HEdge* he = (*this->hedges)[i];
		Edgetype* het = new Edgetype();
		AVLTreeNode<struct Eventtype> *henode = new AVLTreeNode<struct Eventtype>();
		henode->value.e1 = het;
		henode->value.e2 = nullptr;
		henode->value.ty = START;
		het->curx = &curx;
		if (he->gets()->getx() == he->gett()->getx())
			if (he->gets()->gety() > he->gett()->gety())
				het->he = he->getTwin();
			else
				het->he = he;
		else
			if (he->gets()->getx() < he->gett()->getx())
				het->he = he;
			else
				het->he = he->getTwin();
		henode->value.eventPoint = het->he->gets();
		events.insert(henode);
	}
	for (int i = 0; i < _d->hedges->size(); i++) {
		HEdge* he = (*_d->hedges)[i];
		Edgetype* het = new Edgetype();
		AVLTreeNode<struct Eventtype> *henode = new AVLTreeNode<struct Eventtype>();
		henode->value.e1 = het;
		henode->value.e2 = nullptr;
		henode->value.ty = START;
		het->curx = &curx;
		if (he->gets()->getx() == he->gett()->getx())
			if (he->gets()->gety() > he->gett()->gety())
				het->he = he->getTwin();
			else
				het->he = he;
		else
			if (he->gets()->getx() < he->gett()->getx())
				het->he = he;
			else
				het->he = he->getTwin();
		henode->value.eventPoint = het->he->gets();
		events.insert(henode);
	}
	Vertex* curv = nullptr;
	while (!events.isEmpty()) {
		AVLTreeNode<struct Eventtype>*curevent = events.pop();
		if (!(*curv == *curevent->value.eventPoint)) {
			curv = new Vertex(curevent->value.eventPoint);
			merged->getVertices()->push_back(curv);
			curx = curv->getx();
		}
		switch (curevent->value.ty) {
		case START:
			AVLTreeNode<struct Edgetype>*curedge = new AVLTreeNode<struct Edgetype>();
			HEdge*hedge = new HEdge(curevent->value.e1->he->gets(), curevent->value.e1->he->gett());
			curevent->value.e1->he = hedge;
			hedges.insert(hedge);
			if (curv->getIncidentEdge()) {
				hedge->getTwin()->setNext(curv->getIncidentEdge());
				hedge->setPrev(curv->getIncidentEdge()->getPrev());
				curv->getIncidentEdge()->getPrev()->setNext(hedge);
				curv->getIncidentEdge()->setPrev(hedge->getTwin());
			}
			else 
				curv->setIncidentEdge(hedge);
			curedge->value = *curevent->value.e1;
			curedge->value.cd = nullptr;
			curedge->value.cu = nullptr;
			curedges.insert(curedge);
			AVLTreeNode<struct Edgetype>*upedge = curedges.getLeftNode(curedge->value);
			if (upedge) {
				Point<double>* cp = hedge->crossing(upedge->value.he, false);
				if (cp) {
					if (upedge->value.cd) {
						events.pop(*upedge->value.cd);
						upedge->value.cd->e2->cu = nullptr;
						delete(upedge->value.cd);
					}
					AVLTreeNode<struct Eventtype>*ce = new AVLTreeNode<struct Eventtype>();
					ce->value.e2 = &curedge->value;
					ce->value.e1 = &upedge->value;
					ce->value.eventPoint = cp;
					ce->value.ty = CROSS;
					upedge->value.cd = &ce->value;
					events.insert(ce);
				}
			}
			AVLTreeNode<struct Edgetype>*downedge = curedges.getRightNode(curedge->value);
			if (downedge) {
				Point<double>* cp = hedge->crossing(downedge->value.he, false);
				if (cp) {
					if (downedge->value.cu) {
						events.pop(*downedge->value.cu);
						downedge->value.cu->e1->cd = nullptr;
						delete(downedge->value.cu);
					}
					AVLTreeNode<struct Eventtype>*ce = new AVLTreeNode<struct Eventtype>();
					ce->value.e1 = &curedge->value;
					ce->value.e2 = &downedge->value;
					ce->value.eventPoint = cp;
					ce->value.ty = CROSS;
					events.insert(ce);
				}
			}
			break;
		case END:
			if (curv->getIncidentEdge()) {
				curevent->value.e1->he->getTwin()->setNext(curv->getIncidentEdge());
				curevent->value.e1->he->setPrev(curv->getIncidentEdge()->getPrev());
				curv->getIncidentEdge()->getPrev()->setNext(curevent->value.e1->he);
				curv->getIncidentEdge()->setPrev(curevent->value.e1->he->getTwin());
			}
			else
				curv->setIncidentEdge(curevent->value.e1->he);
			AVLTreeNode<struct Edgetype>*upedge = curedges.getLeftNode(curedge->value);
			AVLTreeNode<struct Edgetype>*downedge = curedges.getRightNode(curedge->value);
			if (upedge&&downedge) {
				Point<double>* cp = downedge->value.he->crossing(upedge->value.he, false);
				if (cp) {
					AVLTreeNode<struct Eventtype>*ce = new AVLTreeNode<struct Eventtype>();
					ce->value.e1 = &upedge->value;
					ce->value.e2 = &downedge->value;
					ce->value.eventPoint = cp;
					ce->value.ty = CROSS;
					events.insert(ce);
				}
			}
			delete(curevent->value.e1);
			break;
		case CROSS:
			HEdge *he1, *he2;
			he1 = new HEdge(curevent->value.eventPoint, curevent->value.e1->he->gett());
			he2 = new HEdge(curevent->value.eventPoint, curevent->value.e2->he->gett());
			curevent->value.e1->he->sett(curevent->value.eventPoint);
			curevent->value.e2->he->sett(curevent->value.eventPoint);
			AVLTreeNode<struct Eventtype> *e1, *e2, *s1, *s2;
			e1 = new AVLTreeNode<struct Eventtype>();
			e2 = new AVLTreeNode<struct Eventtype>();
			s1 = new AVLTreeNode<struct Eventtype>();
			s2 = new AVLTreeNode<struct Eventtype>();
			e1->value.eventPoint = curevent->value.eventPoint;
			e2->value.eventPoint = curevent->value.eventPoint;
			s1->value.eventPoint = curevent->value.eventPoint;
			s2->value.eventPoint = curevent->value.eventPoint;
			e1->value.ty = END;
			e2->value.ty = END;
			s1->value.ty = START;
			s2->value.ty = START;
			s1->value.e1 = curevent->value.e1;
			s2->value.e1 = curevent->value.e2;
			e1->value.e1 = new struct Edgetype();
			e1->value.e2 = nullptr;
			e1->value.e1->curx = &curx;
			e1->value.e1->he = he1;
			e1->value.e1->cd = nullptr;
			e1->value.e1->cu = nullptr;
			e2->value.e1 = new struct Edgetype();
			e2->value.e2 = nullptr;
			e2->value.e2->curx = &curx;
			e2->value.e2->he = he2;
			e2->value.e2->cd = nullptr;
			e2->value.e2->cu = nullptr;
			events.insert(e1);
			events.insert(e2);
			events.insert(s1);
			events.insert(s2);
			break;
		default:
			break;
		}
		delete(curevent);
	}
	Face* of = new Face();
	(*merged->getHedges())[0]->setIncidentFace(of);
	return merged;
}
