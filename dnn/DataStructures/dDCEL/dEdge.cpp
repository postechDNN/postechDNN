#include "dEdge.h"
#include "dPoint.h"
#include "dUtil.cpp"
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

dEdge::dEdge(){
    this->s = new dPoint();
    this->t = new dPoint();
}

dEdge::dEdge(dPoint* _s, dPoint* _t) {
	assert(_s!=NULL && _t!=NULL);
	assert(_s->getd()==_t->getd());
	this->s = _s;
	this->t = _t;
}

dEdge::dEdge(dEdge* _e) {
	assert(_e!=NULL);
	this->s = _e->get_s();
	this->t = _e->get_t();
}

dEdge::~dEdge() {

}

bool dEdge::operator==(const dEdge _e){
	return (*(this->get_t()) == _e.get_t()) && (*(this->get_s()) == _e.get_s()) || 
    (*(this->get_t()) == _e.get_s()) && (*(this->get_s()) == _e.get_t());
}

bool dEdge::on(dPoint* p) const{
    const int d = s->getd();

    //if p exist at the out of hypercube, it cannot be on edge
    for(int i=0;i<d;i++){
    	if (!is_between(this->s->getc(i),this->t->getc(i),p->getc(i)))
    		return false;
    }

    auto dif1 = pts_dif(this->s,p);
    auto dif2 = pts_dif(p,this->t);

    return same_vec(dif1,dif2);
}

dPoint* dEdge::crossing(const dEdge* _e, bool closed) const{
    //endpoint intersection
    if (closed){
    	if (this->on(_e->get_s()))
    		return new dPoint(_e->get_s());
        if (this->on(_e->get_t()))
        	return new dPoint(_e->get_t());
        if (_e->on(this->get_s()))
        	return new dPoint(this->get_s());
        if (_e->on(this->get_t()))
        	return new dPoint(this->get_t());
    }
    if(!dCoplanar(this->get_s(),this->get_t(),_e->get_s(),_e->get_t())) return NULL;

    //only consider interior intersection
    const int d = this->get_s()->getd();
    assert(d>2);

    double s1_x,s1_y,s2_x,s2_y;
    s1_x = this->get_t()->getc(0)-this->get_s()->getc(0);
    s1_y = this->get_t()->getc(1)-this->get_s()->getc(1);
    s2_x = _e->get_t()->getc(0)-_e->get_s()->getc(0);
    s2_y = _e->get_t()->getc(1)-_e->get_s()->getc(1);
    //parallel case
	if (abs(s1_x*s2_y-s2_x*s1_y)<ERR) return NULL;

	double s,t;
	s = (-s1_y * (this->get_s()->getc(0) - _e->get_s()->getc(0)) + 
		s1_x * (this->get_s()->getc(1) - _e->get_s()->getc(1))) / 
		(-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (this->get_s()->getc(1) - _e->get_s()->getc(1)) - 
    	s2_y * (this->get_s()->getc(0) - _e->get_s()->getc(0))) / (-s2_x * s1_y + s1_x * s2_y);

    //no solution
    if (!is_between(0.0,1.0,s) || !is_between(0.0,1.0,t)) return NULL;
    for (int i = 2; i<d; i++){
    	//calculate s,t satisfying s*v1+(1-s)*v2=t*v3+(1-t)*v4
    	const double s1 = this->get_s()->getc(i);
    	const double t1 = this->get_t()->getc(i);

    	const double s2 = _e->get_s()->getc(i);
    	const double t2 = _e->get_t()->getc(i);

    	if (abs(s1*s+(1-s)*t1-s2*t+(1-t)*t2)>=ERR) return NULL;
    }
	return pts_fcn(this->get_s(), this->get_t(), 
		[&s, &t](double v1, double v2){return s * v1 + (1 - s)*v2; }
	);
    
}

bool dEdge::is_crossing(const dEdge* _e,bool c) const{
	auto tmp = this->crossing(_e,c);
	if (tmp==NULL) return false;
	delete tmp;
	return true;
}
dPoint* dEdge::get_s() const{
	return this->s;
}

dPoint* dEdge::get_t() const{
	return this->t;
}

void dEdge::set_s(dPoint* _p) {
	assert(_p!=NULL);
	this->s = _p;
}

void dEdge::set_t(dPoint* _p) {
	assert(_p!=NULL);
	this->t = _p;
}

double dEdge::length(const int p) const{
	//consider L_p distance
	assert(p>=0);
    
    auto pdif=pts_dif(this->s,this->t);
	//L_inf
	if (p==0){
		/*auto ret=abs(s->getc(0)-t->getc(0));
		for(auto i=1;i<d;i++) ret = max(ret,abs(s->getc(i)-t->getc(i)));
		return ret;*/
		return *max_element(pdif->cbegin(), pdif->cend());
	}
	//o/w
	long double l = 0;
	for(auto it = pdif->cbegin();it!=pdif->cend();it++){
    	l+=pow(static_cast<long double>(abs(*it)),static_cast<long double>(p));
    	assert(l>=0);//overflow
    }
    return static_cast<double>(pow(l,static_cast<long double>(1.0/p)));
}
