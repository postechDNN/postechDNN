#pragma once

#include <vector>
#include "WF_propagation.h"
//#include "DCEL/DCEL.h"

class Hyperbola {
private:
	Point s, t; // endpoint
	double a, b; // x^2/a^2 - y^2/b^2 = 1
public:
	Hyperbola() {}
	Hyperbola(Point s, Point t, double a, double b) : s(s), t(t), a(a), b(b) {}
	~Hyperbola() {}

	Point gets() { return this->s; }
	Point gett() { return this->t; }
	double geta() { return this->a; }
	double getb() { return this->b; }

	void flip() {
		Point temp = this->s;
		this->s = this->t;
		this->t = temp;
	}
};


class SFaces {
private:
public:
	SFaces(*HEdge) {}
	~SFaces() {}
};


class SPMEdge {
private:
	Hyperbola hyperbola;
	WF_generator v;
	WF_generator w;
public:
	SPMEdge() {}
	SPMEdge(Hyperbola hyperbola, WF_generator v , WF_generator w) hyperbola(hyperbola), v(v), w(w) {}
	~SPMEdge() {}

	Hyperbola getHyperbola() { return this->hyperbola; }
	WF_generator getV() { return this->v; }
	WF_generator getW() { return this->w; }

	void flip() {
		this->hyperbola.flip();
	}

	bool operator<(const SPMEdge& cmpE) const;
};

// Shortest path map class
class SPM{ 
//variables
private:
	Vertex* src;
	WF_propagation& wfp;
	std::vector<SimplePolygon> obs;
	std::vector<SPMEdge> edges;
	
//functions
public:
	SPM(Vertex* s, WF_propagation& wfp, std::vector<SimplePolygon>& obs);
	~SPM();

	void ComputingSPM(); // main function for computing SPM
	void ComputeVertices(); // compute the vertices of SPM inside each cell
	void CombinedVertices(); // Plane sweep to assemble SPM edges
};


class PlaneSweepEvent {
private:
	Point p;
	HEdge* lineSegment;
	SPMEdge* HyperbolaArc;
	enum Event_Type { ARC, OBS };
public:
	bool isStartEP() {
		Point s = this->edge->getEdge().gets();
		Point t = this->edge->getEdge().gett();
		Point* start = nullptr;
		if (s.sety() > t.sety()) {
			start = &s;
		}
		else if (s.sety() == t.sety()) {
			if (s.getx() > t.getx()) start = &s;
			else start = &t;
		}
		else {
			start = &t;
		}
		if (this->p == *start) return true;
		else return false;
	}
	
	// Comparison operator to allow ordering in a priority queue (for the combined vertices SPM)
	bool operator<(const PlaneSweepEvent& cmpE) const {
		//return t > other.t;
	}
};
