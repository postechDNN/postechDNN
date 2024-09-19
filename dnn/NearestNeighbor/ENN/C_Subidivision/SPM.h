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

class HArcEdge {
private:
	Edge* lineSegment;
	SPMEdge* HyperbolaArc;
	bool isArc;
public:

};

class PlaneSweepEvent {
private:
	enum Event_Type { START, END, INTERSECTION };
	Point p;
	HArcEdge* edge1;
	HArcEdge* edge2;
	Event_Type type;
public:
	bool isStartEP() {
		Point s;
		Point t;

		if (this->type == ARC) {
			s = this->HyperbolaArc->getHyperbola().gets();
			t = this->HyperbolaArc->getHyperbola().gett();
		}
		else {
			s = this->lineSegment->gets();
			t = this->lineSegment->gett();
		}

		Point* start = nullptr;
		if (s.gety() > t.gety()) {
			start = &s;
		}
		else if (s.gety() == t.gety()) {
			if (s.getx() > t.getx()) start = &s;
			else start = &t;
		}
		else {
			start = &t;
		}
		if (this->p == *start) return true;
		else return false;
	}

	bool isPositiveSlope() {
		Point t;
		if (this->type == ARC) {
			t = this->HyperbolaArc->getHyperbola().gett();
		}
		else {
			t = this->lineSegment->gett();
		}
		if (this->p.getx() > t.getx()) return true;
		else return false;
	}
	
	void setP(const Point& p) { this->p = p; }
	void setLineSegment(const Edge* hedge) { this->lineSegment = hedge; }
	void setHyperbolaArc(const SPMEdge* spmedge) { this->HyperbolaArc = spmedge; }
	void setType(bool t) {
		if (t) this->type = ARC;
		else this->type = OBS;
	}

	Point getP() { this->p = p; }
	Edge* getLineSegment() { return this->lineSegment; }
	SPMEdge* getHyperbolaArc() { return this->HyperbolaArc; }
	bool getType() {
		if (this->type == ARC) return true;
		else return false;
	}

	// Comparison operator to allow ordering in a priority queue (for the combined vertices SPM)
	bool operator<(const PlaneSweepEvent& cmpE) const {
		return this->p.gety() < cmpE.getP().gety();
	}

	Point intersection(PlaneSweepEvent& cmpE) {}
};
