#include "DCEL.h"
#include "Point.h"
#include <algorithm>
#include <iterator>

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

Vertex::~Vertex(){
	if (!this->incidentEdge) {
		while (this->incidentEdge != this->incidentEdge->getTwin()->getNext())
			delete(this->incidentEdge->getTwin()->getNext());
		delete(this->incidentEdge);
	}
}

void Vertex::setIncidentEdge(HEdge *_e) {
	this->incidentEdge = _e;
}

HEdge* Vertex::getIncidentEdge() {
	return this->incidentEdge;
}

HEdge::HEdge() {
	this->origin = nullptr;
	this->incidentFace = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
	this->twin = nullptr;
}

HEdge::HEdge(Vertex *_v1, Vertex *_v2, Face *_f) {
	this->origin = _v1;
	this->incidentFace = _f;
	this->twin = new HEdge();
	this->next = this->twin;
	this->prev = this->twin;
	this->twin->origin = _v2;
	this->twin->incidentFace = _f;
	this->twin->twin = this;
	this->twin->next = this;
	this->twin->prev = this;
	if (!_v1->getIncidentEdge())
		_v1->setIncidentEdge(this);
	if (!_v2->getIncidentEdge())
		_v2->setIncidentEdge(this->twin);
}

HEdge::~HEdge() {
	if (!this->twin) {
		if (this->origin->getIncidentEdge() == this) {
			if (this->twin->next == this) {
				this->origin->setIncidentEdge(nullptr);
				delete(this->origin);
			}
			else {
				this->origin->setIncidentEdge(this->twin->next);
				this->prev->next = this->twin->next;
				this->twin->next->prev = this->prev;
			}
		}
		if (this->twin->origin->getIncidentEdge() == this->twin) {
			if (this->next == this->twin) {
				this->twin->origin->setIncidentEdge(nullptr);
				delete(this->twin->origin);
			}
			else {
				this->twin->origin->setIncidentEdge(this->next);
				this->next->prev = this->twin->prev;
				this->twin->prev->next = this->next;
			}
		}
		if (this->incidentFace != this->twin->incidentFace) {
			Face *f = this->twin->incidentFace->isOutMost() ? this->twin->incidentFace : this->incidentFace;
			HEdge *e = this->next;
			do {
				e->incidentFace = f;
				e = e->next;
			} while (e != this->next);
			f->setOuter(this->next);
		}
		else {
			if (this->next == this->twin&&this->prev == this->twin) {
				this->incidentFace->deleteInner(this);
				this->incidentFace->deleteInner(this->twin);
			}
			else {
				this->incidentFace->replaceInner(this, this->next);
				this->incidentFace->replaceInner(this->twin, this->twin->next);
				if (this->next!=this->twin&&!this->incidentFace->getInnerBoundary(this->next))
					this->incidentFace->addInner(this->next);
				if (this->twin->next!=this&&!this->incidentFace->getInnerBoundary(this->twin->next))
					this->incidentFace->addInner(this->twin->next);
			}
		}
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

HEdge* HEdge::getPrev() {
	return this->prev;
}

HEdge* HEdge::getTwin() {
	return this->twin;
}

void HEdge::setIncidentFace(Face *_f) {
	this->incidentFace = _f;
}

Edge* HEdge::toEdge() {
	return new Edge(this->origin, this->next->getOrigin());
}

Face::Face() {
	this->outer = nullptr;
	this->inners = new std::vector<HEdge*>();
}

Face::~Face() {
}

bool Face::isOutMost() {
	return outer == nullptr;
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

void Face::replaceInner(HEdge* _f, HEdge* _t) {
	std::replace(this->inners->begin(), this->inners->end(), _f, _t);
}

void Face::addInner(HEdge* _e) {
	this->inners->push_back(_e);
}

void Face::deleteInner(HEdge* _e) {
	for (std::vector<HEdge*>::iterator i = this->inners->begin(); i != this->inners->end(); i++)
		if (*i == _e)
			this->inners->erase(i);
}

HEdge* Face::getInnerBoundary(HEdge* _e) {
	for (std::vector<HEdge*>::iterator i = this->inners->begin(); i != this->inners->end(); i++) {
		HEdge *s = *i;
		HEdge *e = *i;
		do {
			if (e == _e)
				return s;
			e = e->getNext();
		} while (e != s);
	}
	return nullptr;
}