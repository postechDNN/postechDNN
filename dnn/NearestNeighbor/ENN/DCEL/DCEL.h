#pragma once

#include "Edge.h"
#include "Point.h"
#include <vector>
#include <string>

class Vertex;
class HEdge;
class Face;

class Vertex : public Point {
protected:
	std::string key;
	HEdge* incidentEdge; //The origin of incident edge is itself.
public:
	Vertex();
	Vertex(double, double);
	Vertex(HEdge*);
	Vertex(const Point&);
	Vertex(Point&, HEdge*);
	~Vertex();

	std::string getKey();
	void setKey(const std::string&); 
	void setIncidentEdge(HEdge*);
	HEdge* getIncidentEdge(); 
	Point getPoint();
};

class HEdge : public Edge {
protected:
	std::string key; 
	Vertex* origin;
	HEdge* next, *prev, * twin;
	Face* incidentFace; //Incident face is the face which lies on the left to the half edge.
public:
	HEdge();
	HEdge(Vertex*, Vertex*);
	~HEdge();

	std::string getKey();
	void setKey(const std::string&);
	Vertex* getOrigin();
	void setOrigin(Vertex*);
	HEdge* getNext();
	void setNext(HEdge*);
	HEdge* getPrev();
	void setPrev(HEdge*);
	HEdge* getTwin();
	void setTwin(HEdge*);
	Face* getIncidentFace();
	void setIncidentFace(Face*);
	Edge getEdge();
};

class Face {
protected:
	std::string key;
	HEdge* outer;	//If outer is null pointer, it is outmost face in DCEL.
	std::vector<HEdge*> inners;
public:
	Face();
	~Face();

	std::string getKey();
	void setKey(const std::string&);
	bool isOutMost();
	void setOuter(HEdge*);
	HEdge* getOuter();
	std::vector<HEdge*> getInners();
	void addInner(HEdge*);
	//void setInners(std::vector<HEdge*>*);
	std::vector<HEdge *> getOutHEdges();
	std::vector<HEdge*> getInnerHEdges();
};

class DCEL {
protected:
	int num_faces;
	int num_hedges;
	int num_vertices;
	std::vector<Face*> faces;
	std::vector<HEdge*> hedges;	//we store one hedge for each edges.
	std::vector<Vertex*> vertices;
	Vertex* lmost;
	Vertex* tmost;
	Vertex* bmost;
	Vertex* rmost;
public:
	DCEL();
	~DCEL();
	std::vector<Face*> getFaces();
	void setFaces(std::vector<Face*>);
	std::vector<HEdge*> getHedges();
	void setHedges(std::vector<HEdge*>);
	std::vector<Vertex*> getVertices();
	void setVertices(std::vector<Vertex*>);

	//void pushFace(Face*);
	//void pushHEdge(HEdge*);
	//void pushVertex(Vertex*);

	Vertex* getLmost();
	Vertex* getRmost();
	Vertex* getTmost();
	Vertex* getBmost();

	void addVertex(Point&, const std::string&);
	void addEdge(Vertex*, Vertex*);
	void deleteEdge(HEdge*);
	HEdge* searchHedge(const std::string& key);
	Vertex* searchVertex( const std::string& key);
	Face* searchFace( const std::string& key);
	std::vector<HEdge*> getOutgoingHEdges(Vertex*);
	std::vector<HEdge*> getIncomingHEdges(Vertex*);
	int inPolygon(std::vector<HEdge*>, Point);
	DCEL merge(DCEL&);
};
