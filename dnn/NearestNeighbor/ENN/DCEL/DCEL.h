#pragma once
#include "Edge.h"
#include "Point.h"
#include <vector>

class Vertex;
class HEdge;
class Face;

class Vertex : public Point {
protected:
	char* vertex_key;
	HEdge* incidentEdge;
public:
	Vertex();
	Vertex(HEdge*);
	Vertex(Point&);
	Vertex(Point&, HEdge*);
	~Vertex();

	char* getVertexKey();
	void setVertexKey(const char*);
	void setIncidentEdge(HEdge*);
	HEdge* getIncidentEdge();

};

class HEdge : public Edge {
protected:
	char* hedge_key;
	Vertex* origin;
	HEdge* next, * prev, * twin;
	Face* incidentFace;
public:
	HEdge();
	HEdge(Vertex*, Vertex*);
	//HEdge(Point&, Point&);
	~HEdge();

	char* getHedgeKey();
	void setHedgeKey(const char*);
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
};

class Face {
protected:
	char* face_key;
	HEdge* outer;
	std::vector<HEdge*> inners;
public:
	Face();
	~Face();

	char* getFaceKey();
	void setFaceKey(const char*);
	bool isOutMost();
	void setOuter(HEdge*);
	HEdge* getOuter();
	std::vector<HEdge*> getInners();
	void addInner(HEdge*);
	//void setInners(std::vector<HEdge*>*);
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
	//DCEL(FILE*);
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

	void addEdge(Vertex*, Vertex*);
	void addVertex(Point&, const char*);
	void deleteEdge(HEdge*);
	//DCEL* mergeDCEL(DCEL*);
	HEdge* searchHedge(const char* key);
	Vertex* searchVertex(const char* key);
	Face* searchFace(const char* key);
	std::vector<HEdge*> getOutgoingHEdges(Vertex*);
	std::vector<HEdge*> getIncomingHEdges(Vertex*);
	int inPolygon(std::vector<HEdge*>, Point);
};
