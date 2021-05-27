#include "dEdge.h"
#include <cmath>
#define ERR 1e-6

using namespace std;

dEdge::dEdge(){
    this->s = new dPoint();
    this->t = new dPoint();
}

dEdge::dEdge(dPoint* _s, dPoint* _t) {
	this->s = _s;
	this->t = _t;
}

dEdge::dEdge(dEdge* _e) {
	this->s = _e->get_s();
	this->t = _e->get_t();
}

dEdge::~dEdge() {

}

bool dEdge::operator==(dEdge _e) {
	return (*(this->get_t()) == _e.get_t()) && (*(this->get_s()) == _e.get_s()) || 
    (*(this->get_t()) == _e.get_s()) && (*(this->get_s()) == _e.get_t());
}

bool dEdge::on(dPoint* p) {
    //need the definition of n-dim point
}

dPoint* dEdge::crossing(dEdge* _e, bool closed = true) {
    //need the definition of n-dim point

}

dPoint* dEdge::gets() {
	return this->s;
}

dPoint* dEdge::gett() {
	return this->t;
}

void dEdge::set_s(dPoint* _p) {
	this->s = _p;
}

void dEdge::set_t(dPoint* _p) {
	this->t = _p;
}

double dEdge::length() {
	double l = 0;
    //need the definition of n-dim point
    return sqrt(l);
}
