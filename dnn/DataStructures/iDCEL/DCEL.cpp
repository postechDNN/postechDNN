#include "DCEL.h"
#include "Edge.h"
#include "Point.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>


Vertex::Vertex(Point* _p) {
	this->point = _p;
	this->incidentEdge = nullptr;
	this->valid = true;
}

Vertex::~Vertex() {

}

void Vertex::setIncidentEdge(HEdge* _e) {
	this->incidentEdge = _e;
}

HEdge* Vertex::getIncidentEdge() {
	return this->incidentEdge;
}

void Vertex::setValid(bool _v) {
	this->valid = _v;
}

bool Vertex::isValid() {
	return valid;
}

Point* Vertex::getPoint() {
	return this->point;
}

HEdge::HEdge() {
	this->origin = nullptr;
	this->twin = nullptr;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->obs = false;
	this->edge = false;
}

HEdge::HEdge(Vertex* _v1, Vertex* _v2, bool _o) {
	this->origin = _v1;
	this->twin = new HEdge();
	this->twin->origin = _v2;
	this->twin->twin = this;
	this->obs = _o;
	this->twin->obs = _o;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->edge = new Edge(_v1->getPoint(), _v2->getPoint());
}

HEdge::~HEdge() {
	if (!this->twin) {
		this->twin->twin = nullptr;
		delete(this->twin);
	}
}

Vertex* HEdge::getOrigin() {
	return this->origin;
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

Face* HEdge::getIncidentFace() {
	return this->incidentFace;
}

void HEdge::setIncidentFace(Face* _f) {
	this->incidentFace = _f;
}

bool HEdge::getIsObs() {
	return this->obs;
}

Edge* HEdge::getEdge() {
	return this->edge;
}

Face::Face() {
	this->outer = nullptr;
}

Face::~Face() {
}


void Face::setOuter(HEdge* _e) {
	this->outer = _e;
}

HEdge* Face::getOuter() {
	return this->outer;
}


DCEL::DCEL(std::vector<Point*>* _vp) {
	this->vertices = new std::vector<Vertex*>();
	std::vector<Vertex*> upper, lower;
	for (int i = 0; i < _vp->size(); i++) {
		Point* curp = (*_vp)[i];
		Vertex* curv = new Vertex((*_vp)[i]);
		this->vertices->push_back(curv);
		while (true) {
			if (upper.size() < 2)
				break;
			Point p1 = *(upper[upper.size() - 1]->getPoint()) - *(upper[upper.size() - 2]->getPoint());
			Point p2 = *(curp) - *(upper[upper.size() - 2]->getPoint());
			long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
			if (dir > 0)
				upper.pop_back();
			else
				break;
		}
		while (true) {
			if (lower.size() < 2)
				break;
			Point p1 = *(lower[lower.size() - 1]->getPoint()) - *(lower[lower.size() - 2]->getPoint());
			Point p2 = *(curp)-*(lower[lower.size() - 2]->getPoint());
			long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
			if (dir < 0)
				lower.pop_back();
			else
				break;
		}
		upper.push_back(curv);
		lower.push_back(curv);
	}
	Face* outf = new Face();
	Face* upf = new Face();
	Face* lowf = new Face();
	HEdge* he = new HEdge(lower[0], lower[1], false);
	he->setIncidentFace(lowf);
	he->getTwin()->setIncidentFace(outf);
	lower[0]->setIncidentEdge(he);
	for (int i = 1; i < lower.size() - 1; i++) {
		HEdge* nhe = new HEdge(lower[i], lower[i + 1], false);
		nhe->setIncidentFace(lowf);
		nhe->getTwin()->setIncidentFace(outf);
		nhe->setPrev(he);
		he->setNext(nhe);
		nhe->getTwin()->setNext(he->getTwin());
		he->getTwin()->setPrev(nhe->getTwin());
		lower[i]->setIncidentEdge(nhe);
		he = nhe;
	}
	for (int i = upper.size() - 1; i > 0; i--) {
		HEdge* nhe = new HEdge(upper[i], upper[i - 1], false);
		nhe->setIncidentFace(upf);
		nhe->getTwin()->setIncidentFace(outf);
		nhe->setPrev(he);
		he->setNext(nhe);
		nhe->getTwin()->setNext(he->getTwin());
		he->getTwin()->setPrev(nhe->getTwin());
		upper[i]->setIncidentEdge(nhe);
		he = nhe;
	}
	HEdge*nhe = lower[0]->getIncidentEdge();
	lowf->setOuter(nhe);
	outf->setOuter(nhe->getTwin());
	upf->setOuter(he);

	std::vector<Vertex*> inner;
	inner.push_back(upper[0]);
	for (int i = 0; i < _vp->size(); i++) {
		if (!(*vertices)[i]->getIncidentEdge())
			inner.push_back((*vertices)[i]);
	}
	inner.push_back(upper[upper.size() - 1]);
	HEdge* ihe = new HEdge(inner[0], inner[1], false);
	ihe->setPrev(he);
	ihe->getTwin()->setNext(nhe);
	ihe->setIncidentFace(upf);
	ihe->getTwin()->setIncidentFace(lowf);
	he->setNext(ihe);
	nhe->setPrev(ihe->getTwin());
	for (int i = 1; i < inner.size() - 1; i++) {
		HEdge* iihe = new HEdge(inner[i], inner[i + 1], false);
		inner[i]->setIncidentEdge(iihe);
		iihe->setIncidentFace(upf);
		iihe->getTwin()->setIncidentFace(lowf);
		iihe->setPrev(ihe);
		ihe->setNext(iihe);
		iihe->getTwin()->setNext(ihe->getTwin());
		ihe->getTwin()->setPrev(iihe->getTwin());
		ihe = iihe;
	}
	ihe->setNext(upper[upper.size() - 1]->getIncidentEdge());
	upper[upper.size() - 1]->getIncidentEdge()->setPrev(ihe);
	ihe->getTwin()->setPrev(lower[lower.size() - 2]->getIncidentEdge());
	lower[lower.size() - 2]->getIncidentEdge()->setNext(ihe->getTwin());
	
	std::vector<Vertex*> st;
	st.push_back(upper[0]);
	int i = 1;
	int j = 1;
	Face* curf = upf;
	bool isin = true;
	while (i < upper.size() - 1 || j < inner.size() - 1) {
		bool nisin = !(*upper[i]->getPoint() <= *inner[j]->getPoint());
		Vertex* popv = st[st.size() - 1];
		st.pop_back();
		if (nisin) {
			if (isin) {
				while (!st.empty()) {
					Point p1 = *(popv->getPoint()) - *(st[st.size() - 1]->getPoint());
					Point p2 = *(inner[j]->getPoint()) - *(st[st.size() - 1]->getPoint());
					long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
					if (dir <= 0)
						break;
					HEdge* nhe = new HEdge(inner[j], st[st.size() - 1], false);
					nhe->setIncidentFace(curf);
					curf->setOuter(nhe);
					curf = new Face();
					nhe->getTwin()->setIncidentFace(curf);
					curf->setOuter(nhe->getTwin());
					HEdge* pt = inner[j]->getIncidentEdge()->getPrev()->getPrev();
					pt->getPrev()->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(pt->getPrev());
					pt->setPrev(nhe);
					nhe->setNext(pt);
					pt = pt->getNext();
					pt->getNext()->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(pt->getNext());
					pt->setNext(nhe);
					nhe->setPrev(pt);
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(popv);
				st.push_back(inner[j]);
			}
			else {
				Vertex* prev = popv;
				Face* nf = curf;
				HEdge* pt = popv->getIncidentEdge()->getTwin()->getNext()->getTwin();
				HEdge* nt = inner[j]->getIncidentEdge();
				while (!st.empty()) {
					HEdge* nhe = new HEdge(inner[j], popv, false);
					nhe->getTwin()->setIncidentFace(nf);
					nf->setOuter(nhe->getTwin());
					nf = new Face();
					nhe->setIncidentFace(nf);
					nf->setOuter(nhe);
					pt->getNext()->setPrev(nhe);
					nhe->setNext(pt->getNext());
					pt->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(pt);
					nt->getPrev()->setNext(nhe);
					nhe->setPrev(nt->getPrev());
					nt->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(nt);
					nt = nhe;
					pt = nt->getNext();
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(prev);
				st.push_back(inner[j]);
			}
			j++;
		}
		else {
			if (isin) {
				Vertex* prev = popv;
				Face* nf = curf;
				HEdge* pt = upper[i + 1]->getIncidentEdge();
				HEdge* nt = popv->getIncidentEdge();
				while (!st.empty()) {
					HEdge* nhe = new HEdge(upper[i], popv, false);
					nhe->setIncidentFace(nf);
					nf->setOuter(nhe);
					nf = new Face();
					nhe->getTwin()->setIncidentFace(nf);
					nf->setOuter(nhe->getTwin());
					pt->getNext()->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(pt->getNext());
					pt->setNext(nhe);
					nhe->setPrev(pt);
					nt->getPrev()->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(nt->getPrev());
					nt->setPrev(nhe);
					nhe->setNext(nt);
					pt = nhe->getTwin();
					nt = pt->getPrev();
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(prev);
				st.push_back(upper[i]);
			}
			else {
				while (!st.empty()) {
					Point p1 = *(popv->getPoint()) - *(st[st.size() - 1]->getPoint());
					Point p2 = *(upper[i]->getPoint()) - *(st[st.size() - 1]->getPoint());
					long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
					if (dir >= 0)
						break;
					HEdge* nhe = new HEdge(upper[i], st[st.size() - 1], false);
					nhe->getTwin()->setIncidentFace(curf);
					curf->setOuter(nhe->getTwin());
					curf = new Face();
					nhe->setIncidentFace(curf);
					curf->setOuter(nhe);
					HEdge* nt = upper[i + 1]->getIncidentEdge()->getNext()->getNext();
					nt->getNext()->setPrev(nhe);
					nhe->setNext(nt->getNext());
					nt->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(nt);
					nt = nt->getPrev();
					nt->getPrev()->setNext(nhe);
					nhe->setPrev(nt->getPrev());
					nt->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(nt);
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(popv);
				st.push_back(upper[i]);
			}
			i++;
		}
		isin = nisin;
	}
	if (isin) {
		HEdge* pt = inner[inner.size() - 2]->getIncidentEdge();
		st.pop_back();
		Vertex* cv = st[st.size() - 1];
		st.pop_back();
		while (!st.empty()) {
			HEdge* nhe = new HEdge(inner[inner.size() - 1], cv, false);
			nhe->setIncidentFace(curf);
			curf->setOuter(nhe);
			curf = new Face();
			nhe->getTwin()->setIncidentFace(curf);
			curf->setOuter(nhe->getTwin());
			pt->getNext()->setPrev(nhe->getTwin());
			nhe->getTwin()->setNext(pt->getNext());
			pt->setNext(nhe);
			nhe->setPrev(pt);
			pt = pt->getPrev();
			pt->getPrev()->setNext(nhe->getTwin());
			nhe->getTwin()->setPrev(pt->getPrev());
			pt->setPrev(nhe);
			nhe->setNext(pt);
			pt = nhe->getTwin();
			cv = st[st.size() - 1];
			st.pop_back();
		}
	}
	else {
		HEdge* nt = inner[inner.size() - 1]->getIncidentEdge();
		st.pop_back();
		Vertex* cv = st[st.size() - 1];
		st.pop_back();
		while (!st.empty()) {
			HEdge*nhe = new HEdge(inner[inner.size() - 1], cv, false);
			nhe->getTwin()->setIncidentFace(curf);
			curf->setOuter(nhe->getTwin());
			curf = new Face();
			nhe->setIncidentFace(curf);
			curf->setOuter(nhe);
			nt->getPrev()->setNext(nhe);
			nhe->setPrev(nt->getPrev());
			nt->setPrev(nhe->getTwin());
			nhe->getTwin()->setNext(nt);
			nt = nt->getNext();
			nt->getNext()->setPrev(nhe);
			nhe->setNext(nt->getNext());
			nt->setNext(nhe->getTwin());
			nhe->getTwin()->setPrev(nt);
			nt = nhe;
			cv = st[st.size() - 1];
			st.pop_back();
		}
	}
	i = 1;
	j = 1;
	curf = lowf;
	isin = true;
	st.push_back(lower[0]);
	while (i < lower.size() - 1 || j < inner.size() - 1) {
		bool nisin = !(*lower[i]->getPoint() <= *inner[j]->getPoint());
		Vertex* popv = st[st.size() - 1];
		st.pop_back();
		if (nisin) {
			if (isin) {
				while (!st.empty()) {
					Point p1 = *(popv->getPoint()) - *(st[st.size() - 1]->getPoint());
					Point p2 = *(inner[j]->getPoint()) - *(st[st.size() - 1]->getPoint());
					long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
					if (dir >= 0)
						break;
					HEdge* nhe = new HEdge(inner[j], st[st.size() - 1], false);
					nhe->getTwin()->setIncidentFace(curf);
					curf->setOuter(nhe->getTwin());
					curf = new Face();
					nhe->setIncidentFace(curf);
					curf->setOuter(nhe);
					HEdge* pt = inner[j]->getIncidentEdge()->getTwin()->getNext()->getNext();
					pt->getNext()->setPrev(nhe);
					nhe->setNext(pt->getNext());
					pt->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(pt);
					pt = pt->getPrev();
					pt->getPrev()->setNext(nhe);
					nhe->setPrev(pt->getPrev());
					pt->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(pt);
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(popv);
				st.push_back(inner[j]);
			}
			else {
				Vertex* prev = popv;
				Face* nf = curf;
				HEdge* nt = popv->getIncidentEdge();
				HEdge* pt = inner[j]->getIncidentEdge()->getTwin();
				while (!st.empty()) {
					HEdge* nhe = new HEdge(inner[j], popv, false);
					nhe->setIncidentFace(nf);
					nf->setOuter(nhe);
					nf = new Face();
					nhe->getTwin()->setIncidentFace(nf);
					nf->setOuter(nhe->getTwin());
					nt->getPrev()->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(nt->getPrev());
					nt->setPrev(nhe);
					nhe->setNext(nt);
					pt->getNext()->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(pt->getNext());
					pt->setNext(nhe);
					nhe->setPrev(nhe);
					pt = nhe->getTwin();
					nt = pt->getPrev();
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(prev);
				st.push_back(inner[j]);
			}
			j++;
		}
		else {
			if (isin) {
				Vertex* prev = popv;
				Face* nf = curf;
				HEdge* nt = lower[i]->getIncidentEdge();
				HEdge* pt = popv->getIncidentEdge()->getTwin();
				while (!st.empty()) {
					HEdge* nhe = new HEdge(lower[i], popv, false);
					nhe->getTwin()->setIncidentFace(nf);
					nf->setOuter(nhe);
					nf = new Face();
					nhe->setIncidentFace(nf);
					nf->setOuter(nhe);
					nt->getPrev()->setNext(nhe);
					nhe->setPrev(nt->getPrev());
					nt->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(nt);
					pt->getNext()->setPrev(nhe);
					nhe->setNext(pt->getNext());
					pt->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(pt);
					nt = nhe;
					pt = nt->getNext();
					popv = st[st.size() - 1];
					st.pop_back();
				}
				st.push_back(prev);
				st.push_back(lower[i]);
			}
			else {
				st.push_back(popv);
				st.push_back(lower[i]);
				while (!st.empty()) {
					Point p1 = *(popv->getPoint()) - *(st[st.size() - 1]->getPoint());
					Point p2 = *(lower[i]->getPoint()) - *(st[st.size() - 1]->getPoint());
					long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
					if (dir <= 0)
						break;
					HEdge* nhe = new HEdge(lower[i], st[st.size() - 1], false);
					nhe->setIncidentFace(curf);
					curf->setOuter(nhe);
					curf = new Face();
					nhe->getTwin()->setIncidentFace(curf);
					curf->setOuter(nhe->getTwin());
					HEdge *pt = lower[i]->getIncidentEdge()->getPrev()->getPrev();
					pt->getPrev()->setNext(nhe->getTwin());
					nhe->getTwin()->setPrev(pt->getPrev());
					pt->setPrev(nhe);
					nhe->setNext(pt);
					pt = pt->getNext();
					pt->getNext()->setPrev(nhe->getTwin());
					nhe->getTwin()->setNext(pt->getNext());
					pt->setNext(nhe);
					nhe->setPrev(pt);
					popv = st[st.size() - 1];
					st.pop_back();
				}
			}
			i++;
		}
		isin = nisin;
	}
	if (isin) {
		HEdge* nt = inner[inner.size() - 2]->getIncidentEdge()->getTwin();
		st.pop_back();
		Vertex* cv = st[st.size() - 1];
		st.pop_back();
		while (!st.empty()) {
			HEdge* nhe = new HEdge(inner[inner.size() - 1], cv, false);
			nhe->getTwin()->setIncidentFace(curf);
			curf->setOuter(nhe->getTwin());
			curf = new Face();
			nhe->setIncidentFace(curf);
			curf->setOuter(nhe);
			nt->getPrev()->setNext(nhe);
			nhe->setPrev(nt->getPrev());
			nt->setPrev(nhe->getTwin());
			nhe->getTwin()->setNext(nt);
			nt = nt->getNext();
			nt->getNext()->setPrev(nhe);
			nhe->setNext(nt->getNext());
			nt->setNext(nhe->getTwin());
			nhe->getTwin()->setPrev(nt);
			nt = nhe;
			cv = st[st.size() - 1];
			st.pop_back();
		}
	}
	else {
		HEdge* pt = inner[inner.size() - 1]->getIncidentEdge()->getTwin()->getNext()->getTwin();
		st.pop_back();
		Vertex* cv = st[st.size() - 1];
		st.pop_back();
		while (!st.empty()) {
			HEdge*nhe = new HEdge(inner[inner.size() - 1], cv, false);
			nhe->setIncidentFace(curf);
			curf->setOuter(nhe);
			curf = new Face();
			nhe->getTwin()->setIncidentFace(curf);
			curf->setOuter(nhe->getTwin());
			pt->getNext()->setPrev(nhe->getTwin());
			nhe->getTwin()->setNext(pt->getNext());
			pt->setNext(nhe);
			nhe->setPrev(pt);
			pt = pt->getPrev();
			pt->getPrev()->setNext(nhe->getTwin());
			nhe->getTwin()->setPrev(pt->getPrev());
			pt->setPrev(nhe);
			nhe->setNext(nhe);
			pt = nhe->getTwin();
			cv = st[st.size() - 1];
			st.pop_back();
		}
	}
}

DCEL::~DCEL() {

}

bool DCEL::addEdge(int i, int j) {
	if (!((*vertices)[i]->isValid() && (*vertices)[j]->isValid()))
		return false;
	return true;
}