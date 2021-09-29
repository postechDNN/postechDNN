#pragma once
#include "Edge.h"
#include "Point.h"
#include <fstream>
#include <sstream>
#include <vector>

class Vertex;
class HEdge;
class Face;

class Vertex {
protected:
	Point* point;
	HEdge* incidentEdge;
	bool valid;
public:
	Vertex(Point*);
	~Vertex();

	void setIncidentEdge(HEdge*);
	HEdge* getIncidentEdge();
	void setValid(bool);
	bool isValid();
	Point* getPoint();
};

class HEdge{
protected:
	Vertex *origin;
	HEdge *next, *prev, *twin;
	Face* incidentFace;
	bool obs;
	Edge* edge;
public:
	HEdge();
	HEdge(Vertex*, Vertex*, bool);
	~HEdge();

	Vertex* getOrigin();
	HEdge* getNext();
	void setNext(HEdge*);
	HEdge* getPrev();
	void setPrev(HEdge*);
	HEdge* getTwin();
	Face* getIncidentFace();
	void setIncidentFace(Face*);
	bool getIsObs();
	Edge* getEdge();
};

class Face {
protected:
	HEdge* outer;

public:
	Face();
	~Face();

	void setOuter(HEdge*);
	HEdge* getOuter();
};

class DCEL {
private:
	std::vector<Vertex*>* vertices;
public:
	DCEL(std::vector<Point*>*);
	~DCEL();
	bool addEdge(int, int);
};
