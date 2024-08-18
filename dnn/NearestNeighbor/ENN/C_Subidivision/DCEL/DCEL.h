#pragma once

#include "Edge.h"
#include "Point.h"
#include <vector>
#include <string>
#include <map>

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

class HEdge {
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
	void setInners(std::vector<HEdge*>*);
	std::vector<HEdge *> getOutHEdges();
	std::vector<HEdge*> getInnerHEdges();
	std::vector<Vertex*> getOutVertices();
	std::vector<Vertex*> getInnerVertices(int);
	std::vector<HEdge*> getInnerHEdges(int);
	bool inFace(Point p, bool is_open=true);
};

class DCEL {
protected:
	std::string key;
	std::map<std::string, Face*> faces;
	std::map<std::string, HEdge*> hedges;	//we store one hedge for each edges.
	std::map<std::string, Vertex*> vertices;
public:
	DCEL(std::string key ="__default__");
	DCEL(std::vector<Point>&, std::vector<std::vector<int>>&,std::string key ="__default__");
	DCEL(std::vector<std::pair<std::string,Point>>&, std::vector<std::vector<int>>&, std::string key = "__default__");
	~DCEL();
	std::vector<Face*> getFaces();
	void setFaces(std::vector<Face*>);
	std::vector<HEdge*> getHedges();
	void setHedges(std::vector<HEdge*>);
	std::vector<Vertex*> getVertices();
	void setVertices(std::vector<Vertex*>);
	std::string getKey();
	void setKey(const std::string&);

	HEdge* getHedge(const std::string& key);
	Vertex* getVertex( const std::string& key);
	Face* getFace( const std::string& key);
	
	std::vector<HEdge*> getOutgoingHEdges(Vertex*);
	std::vector<HEdge*> getIncomingHEdges(Vertex*);
	int inPolygon(std::vector<HEdge*>, Point);
	DCEL merge(DCEL&, std::string sep="_");
	void clear();
};
