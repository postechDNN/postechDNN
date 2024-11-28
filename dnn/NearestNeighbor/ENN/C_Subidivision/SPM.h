#pragma once

#include <vector>
#include <string>
#include <map>
#include "Hyperbola.h"
#include "WF_propagation.h"
//#include "DCEL/DCEL.h"

class SFaces {
private:
	std::vector<HEdge*> edges;
public:
	SFaces(std::vector<HEdge*> cell, IOEdgesContainers* e) {
		for (auto nowInputWave : e->hedges) {

		}
	}
	~SFaces() {}
	
	std::vector<HEdge*> getEdges() const { return this->edges; }
};

class SPMEdge {
private:
	Hyperbola hyperbola;
	WF_generator v;
	WF_generator w;
public:
	SPMEdge() {}
	SPMEdge(Hyperbola hyperbola, WF_generator v , WF_generator w) : hyperbola(hyperbola), v(v), w(w) {}
	~SPMEdge() {}

	Hyperbola getHyperbola() const { return this->hyperbola; }
	WF_generator getV() const { return this->v; }
	WF_generator getW() const { return this->w; }

	void flip() {
		this->hyperbola.flip();
	}

	bool operator<(const SPMEdge& cmpE) const;
};

class HArcEdge {
private:
	Edge* lineSegment;
	Hyperbola* HyperbolaArc;
	bool isArc;

	std::string key;
	HArcEdge* next;
	HArcEdge* prev;
	HArcEdge* twin;
	std::vector<HArcEdge*> TNeighbor;
public:
	HArcEdge(){
		this->lineSegment = nullptr;
		this->HyperbolaArc = nullptr;
		this->isArc = false;
	}
	HArcEdge(HArcEdge* HAE) {
		if (HAE->getIsArc()) {
			this->lineSegment = nullptr;
			this->HyperbolaArc = new Hyperbola(HAE->getHyperbolaArc());
			this->isArc = true;
		}
		else {
			this->lineSegment = new Edge(HAE->getLineSegment()->gets(), HAE->getLineSegment()->gett());
			this->HyperbolaArc = nullptr;
			this->isArc = false;
		}
	}
	~HArcEdge(){}

	void sets(Point s) {
		if (this->isArc) this->HyperbolaArc->sets(s);
		else this->lineSegment->sets(s);
	}
	void sett(Point t) {
		if (this->isArc) this->HyperbolaArc->sett(t);
		else this->lineSegment->sett(t);
	}
	void setLineSegment(Edge* hedge) { this->lineSegment = hedge; }
	void setHyperbolaArc(Hyperbola* spmedge) { this->HyperbolaArc = spmedge; }
	void setIsArc(const bool isArc) { this->isArc = isArc; }
	void setNext(HArcEdge* next) { this->next = next; }
	void setPrev(HArcEdge* prev) { this->prev = prev; }
	void setTwin(HArcEdge* twin) { this->twin = twin; }
	void setKey(std::string key) { this->key = key; }
	void pushNeighbor(HArcEdge* nei) { this->TNeighbor.push_back(nei); }

	Point gets() const {
		if (this->isArc) return this->HyperbolaArc->gets();
		else return this->lineSegment->gets();
	}
	Point gett() const {
		if (this->isArc) return this->HyperbolaArc->gett();
		else return this->lineSegment->gett();
	}
	Edge* getLineSegment() const { return this->lineSegment; }
	Hyperbola* getHyperbolaArc() const { return this->HyperbolaArc; }
	bool getIsArc() const { return this->isArc; }
	HArcEdge* getNext() const { return this->next; }
	HArcEdge* getPrev() const { return this->prev; }
	HArcEdge* getTwin() const { return this->twin; }
	std::vector<HArcEdge*> getNeighbor() const { return this->TNeighbor; }


	bool isPositiveSlope() {
		Point s;
		Point t;
		if (this->isArc) {
			s = this->HyperbolaArc->gets();
			t = this->HyperbolaArc->gett();
		}
		else {
			s = this->lineSegment->gets();
			t = this->lineSegment->gett();
		}
		if (s.getx() > t.getx()) return true;
		else return false;
	}

	Point intersection(const HArcEdge& cmpE) {
		Point result;
		return result;
	}

	double angle(const HArcEdge& cmpE) {
		double result;
		return result;
	}

	bool operator<(const HArcEdge& cmpE) const {
		Point thisP;
		Point cmpP;
		if (this->isArc) thisP = this->HyperbolaArc->gets();
		else thisP = this->lineSegment->gets();
		if (cmpE.getIsArc()) thisP = this->HyperbolaArc->gets();
		else thisP = this->lineSegment->gets();
		return thisP.gety() < thisP.gety();
	}
};

class PlaneSweepEvent {
private:
	enum Event_Type { START, END, INTERSECTION };
	Point p;
	HArcEdge* edge1;
	HArcEdge* edge2;
	Event_Type type;

public:
	void setP(const Point& p) { this->p = p; }
	void setEdge1(HArcEdge* edge) { this->edge1 = edge; }
	void setEdge2(HArcEdge* edge) { this->edge2 = edge; }
	void setType(int idx) {
		if (idx == 0) this->type = START;
		else if (idx == 1) this->type = END;
		else this->type = INTERSECTION;
	}

	Point getP() const { return this->p; }
	HArcEdge* getEdge1() const { return this->edge1; }
	HArcEdge* getEdge2() const { return this->edge2; }
	int getType() const { return int(this->type); }

	// Comparison operator to allow ordering in a priority queue (for the combined vertices SPM)
	bool operator<(const PlaneSweepEvent& cmpE) const {
		return this->p.gety() < cmpE.getP().gety();
	}
};

class SPMDCEL {
private:
	std::map<std::string, HArcEdge*> hedges;
public:
	SPMDCEL(std::vector<HArcEdge*>);
	~SPMDCEL() {}
};

// Shortest path map class
class SPM {
	//variables
private:
	Vertex* src;
	WF_propagation& wfp;
	CS_Free& cs_free;
	std::vector<SimplePolygon> obs;
	std::vector<SPMEdge> edges;
	SPMDCEL* dcel;
	//functions
public:
	SPM(Vertex* s, WF_propagation& wfp, CS_Free& cs_free, std::vector<SimplePolygon>& obs);
	~SPM();

	void ComputeVertices(); // Compute the vertices of SPM inside each cell
	void CombineVertices(); // Plane sweep to assemble SPM edges
};