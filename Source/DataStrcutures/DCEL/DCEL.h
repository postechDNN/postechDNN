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

class HEdge{
protected:
	Vertex *origin;
	HEdge *next, *prev, *twin;
	Face *incidentFace;
public:
	HEdge();
	HEdge(Vertex*, Vertex*, Face*);
	~HEdge();
	Vertex* getOrigin();
	HEdge* getNext();
	HEdge* getPrev();
	HEdge* getTwin();
	void setIncidentFace(Face*);
	Edge* toEdge();
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
	void replaceInner(HEdge*, HEdge*);
	void addInner(HEdge*);
	void deleteInner(HEdge*);
	HEdge* getInnerBoundary(HEdge*);
};

class DCEL : public Face {	//Referring the outmost face.
public:

};