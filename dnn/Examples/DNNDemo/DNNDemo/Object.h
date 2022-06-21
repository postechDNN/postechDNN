#pragma once

#include <vector>
#include <string>

class Point {
public:
	Point();
	Point(double x, double y);
	Point(double x, double y, double z);
	~Point();
	double getX();
	double getY();
	double getZ();
	
	void setX(double x);
	void setY(double y);
	void setZ(double z);
private:
	double x;
	double y;
	double z;
};

class Vertex {
public:
	Vertex();
	~Vertex();
	std::string getName();
	Point& getPos();
	void setName(std::string n);
	void setPos(double x, double y);
	void setPos(double x, double y, double z);
private:
	std::string name;
	Point pos;
};

class Edge {
public:
	Edge();
	Edge(Point s, Point e);
	~Edge();
	std::string getName();
	Point& getStartP();
	Point& getEndP();
	std::string getFace();
	std::string getTwin();
	std::string getNext();
	void setName(std::string n);
	void setStartP(Point P);
	void setEndP(Point P);
	void setFace(std::string f);
	void setTwin(std::string t);
	void setNext(std::string n);
private:
	std::string name;
	std::vector<Point> endpoints;
	std::string face;
	std::string twin;
	std::string next;
};

class Face{
public:
	Face();
	~Face();
	std::string getName();
	int getSize();
	int getInnerEdgeSize();
	Point& getPoint(int idx);
	bool isInner();
	void addPoint(Point p);
	void addInnerEdge(std::string edge);
	void setName(std::string n);
	void setInner(bool i);
	std::string getInnerEdge(int idx);
private:
	std::string name;
	std::vector<Point> vertices;
	std::vector<std::string> innerEdge;
	bool inner;
};

class Object{
public:
	Object();
	~Object();
	int getVerticsNum();
	int getEdgesNum();
	int getFacesNum();
	Vertex& getVertex(int idx);
	Edge& getEdge(int idx);
	Face& getFace(int idx);
	void addVertex();
	void addEdge();
	void addFace();
	void addFace(Face f);

	void setDrawVertices(bool b);
	void setDrawEdges(bool b);
	void setDrawFaces(bool b);
	bool getDrawVertices();
	bool getDrawEdges();
	bool getDrawFaces();

	void updateNorm(int dimension);
	void getNorm(double trans[], double strat[], int dimension);

	void readDCEL(CString path, int dimension);

private:
	double normTrans[3];
	double normStrat[3];
	bool drawVertices;
	bool drawEdges;
	bool drawFaces;
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Face> faces;
};

