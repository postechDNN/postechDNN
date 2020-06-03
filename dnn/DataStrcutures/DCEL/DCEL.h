#pragma once
#include "Edge.h"
#include "Point.h"
#include <vector>

class Vertex;
class HEdge;
class Face;

class Vertex : public Point {
protected:
	HEdge *incidentEdge;
public:
	Vertex();
	Vertex(HEdge*);
	Vertex(Point*);
	Vertex(Point*,HEdge*);
	~Vertex();
	void setIncidentEdge(HEdge*);
	HEdge* getIncidentEdge();
};

class HEdge : public Edge {
protected:
	Vertex *origin;
	HEdge *next, *prev, *twin;
	Face *incidentFace;
public:
	HEdge();
	HEdge(Vertex*, Vertex*);
	~HEdge();
	Vertex* getOrigin();
	HEdge* getNext();
	HEdge* getPrev();
	HEdge* getTwin();
	void setIncidentFace(Face*);
};

class Face {
protected:
	HEdge *outer;
	std::vector<HEdge*> *inners;
public:
	Face();
	~Face();
	bool isOutMost();
	void setOuter(HEdge*);
	HEdge* getOuter();
	std::vector<HEdge*>* getInners();
};

class DCEL {
private:
	std::vector<Face*> *faces;
	std::vector<HEdge*> *hedges;	//we store one hedge for each edges.
	std::vector<Vertex*> *vertices;
	Vertex* lmost;
	Vertex* tmost;
	Vertex* bmost;
	Vertex* rmost;
public:
	DCEL();
	~DCEL();
	std::vector<Face*>* getFaces();
	std::vector<HEdge*>* getHedges();
	std::vector<Vertex*>* getVertices();
	void addEdge(Vertex*, Vertex*);
	void deleteEdge(HEdge*);
	DCEL* mergeDCEL(DCEL*);
};