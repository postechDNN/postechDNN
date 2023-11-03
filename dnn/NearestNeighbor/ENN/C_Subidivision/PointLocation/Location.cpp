#include"Location.h"
#include <algorithm>
#include <cassert>
#include <iostream>

typedef pair<HEdge*, bool> Event;

double keyx(Event e) {
	double s = e.first->getOrigin()->getx();
	double t = e.first->getTwin()->getOrigin()->getx();
	return e.second ? min(s, t) : max(s, t);
}

double keyy(Event e) {
	double s = e.first->getOrigin()->gety();
	double t = e.first->getTwin()->getOrigin()->gety();
	return e.second ? min(s, t) : max(s, t);
}

bool cmpx(Event e1, Event e2) {
	double ke1 = keyx(e1);
	double ke2 = keyx(e2);
	if (ke1 == ke2) {
		if (e1.second == e2.second)
			return false; // weak comparison
		else
			return e1.second;
	}
	return ke1 < ke2;
}

bool cmpy(Event e1, Event e2) {
	double ke1 = keyy(e1);
	double ke2 = keyy(e2);
	if (ke1 == ke2) {
		if (e1.second == e2.second)
			return false; // weak comparison
		else
			return e1.second;
	}
	return ke1 < ke2;
}

double CEdge::constant = 0.0;
bool CEdge::is_vertical = true;

CEdge::CEdge() : HEdge() {
	ref = NULL;
}
CEdge::CEdge(HEdge& he) : HEdge(he) {
	ref = &he;
}
CEdge::CEdge(Point* p1, Point* p2) : HEdge(new Vertex(*p1), new Vertex(*p2)) {
	ref = NULL;
}

bool CEdge::operator<(CEdge& other) {
	double s1a, s1b, t1a, t1b, s2a, s2b, t2a, t2b;
	Vertex* s1 = getOrigin();
	Vertex* s2 = other.getOrigin();
	Vertex* t1 = getTwin()->getOrigin();
	Vertex* t2 = other.getTwin()->getOrigin();

	s1a = getCoord(s1, true);
	s1b = getCoord(s1, false);
	t1a = getCoord(t1, true);
	t1b = getCoord(t1, false);
	s2a = getCoord(s2, true);
	s2b = getCoord(s2, false);
	t2a = getCoord(t2, true);
	t2b = getCoord(t2, false);

	if (getTwin()->getKey().compare(other.getKey()) == 0) {
		if (is_vertical)
			return s1a > s2a;
		else
			return s1a < s2a;
	}

	double p1 = s1b, p2 = s2b;
	if (s1a != t1a) {
		double tmp = (CEdge::constant - s1a) / (t1a - s1a);
		p1 = (1. - tmp) * s1b + tmp * t1b;
	}
	  
	if (s2a != t2a) {
		double tmp = (CEdge::constant - s2a) / (t2a - s2a);
		p2 = (1. - tmp) * s2b + tmp * t2b;
	}
	return p1 < p2;
}

bool CEdge::operator==(CEdge& other) {
	return getKey().compare(other.getKey()) == 0;
}

double CEdge::getCoord(Vertex* p, bool parallel) {
	if (is_vertical == parallel)
		return p->getx();
	else
		return p->gety();
}

void CEdge::setConstantLine(double constant, bool is_vertical) {
	CEdge::constant = constant;
	CEdge::is_vertical = is_vertical;
}

double CEdge::getConstant() {
	return CEdge::constant;
}

HEdge* CEdge::getRef() {
	return ref;
}

Location::Location(DCEL* dcel) {
	for (Face* f : dcel->getFaces()) {
		if (f->isOutMost()) {
			outmost = f;
			break;
		}
	}

	vector<Vertex*> vs = dcel->getVertices();
	Vertex* leftmost = vs[0];
	Vertex* bottommost = vs[0];
	double minx = leftmost->getx();
	double miny = bottommost->gety();

	for (auto v : vs) {
		if (minx > v->getx()) {
			minx = v->getx();
			leftmost = v;
		}
		if (miny > v->gety()) {
			miny = v->gety();
			bottommost = v;
		}
	} 

	vector<HEdge*> hes = dcel->getHedges();
	//Make events
	vector<Event> events;
	for (auto he : hes) {
		events.push_back({ he, true });
		events.push_back({ he, false });
	}

	CEdge::setConstantLine(leftmost->getx(), true);
	std::sort(events.begin(), events.end(), cmpx);
	for (auto e : events) {

		if (e.second)
			v_slab.insert(CEdge(*e.first));
		else
			v_slab.remove(CEdge(*e.first));
		xcoords.push_back(keyx(e));
	}

	CEdge::setConstantLine(leftmost->gety(), false);
	std::sort(events.begin(), events.end(), cmpy);
	for (auto e : events) {

		if (e.second)
			h_slab.insert(CEdge(*e.first));
		else
			h_slab.remove(CEdge(*e.first));
		ycoords.push_back(keyy(e));
	}
}

int Location::getVersion(Point* p, bool is_vertical) {
	vector<double>& coords = is_vertical ? xcoords : ycoords;
	P_PBST<CEdge>& slab = is_vertical ? v_slab : h_slab;

	int start = 1;
	int end = slab.lastVersion() + 1;
	int prev_ver = 0;
	int ver = (start + end) / 2;

	double c = CEdge::getConstant();

	if (c < coords[0]) {
		return 0; // p is on the outmost face.
	}

	while (start < end) {
		ver = (start + end) / 2;

		if (prev_ver == ver)
			break;

		if (c < coords[ver - 1])
			end = ver;
		else
			start = ver;

		prev_ver = ver;
	}

	return ver;
}

Face* Location::locate(Point* p) {
	CEdge::setConstantLine(p->getx(), true);
	int ver = getVersion(p, true);

	CEdge pe(p, p);
	CEdge e;
	if (v_slab.searchLEQ(ver, pe, e))
		return e.getIncidentFace();
	else
		return outmost;
}

HEdge* Location::ortho_ray(Point* p, int dir) {
	assert(0 <= dir && dir <= 3);

	bool is_vertical = (dir == Location::N) || (dir == Location::S) ? true : false;
	P_PBST<CEdge>& slab = is_vertical ? v_slab : h_slab;
	
	if (is_vertical)
		CEdge::setConstantLine(p->getx(), true);
	else
		CEdge::setConstantLine(p->gety(), false);

	int ver = getVersion(p, is_vertical);

	CEdge pe(p, p);
	CEdge e;

	if (dir == Location::N || dir == Location::E) {
		if (slab.searchGEQ(ver, pe, e)) {
			return e.getRef();
		}
	}
	else {
		if (slab.searchLEQ(ver, pe, e))
			return e.getRef();
	}

	return NULL;
}