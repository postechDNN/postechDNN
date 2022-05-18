#pragma once

#include <vector>

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
	Point& getPos();
	void setPos(double x, double y);
	void setPos(double x, double y, double z);
private:
	Point pos;
};

class Edge {
public:
	Edge();
	Edge(Point s, Point e);
	~Edge();
	Point& getStartP();
	Point& getEndP();
	void setStartP(Point P);
	void setEndP(Point P);

private:
	std::vector<Point> endpoints;
};

class Face{
public:
	Face();
	~Face();
	int getSize();
	Point& getPoint(int idx);
	bool isInner();
	void addPoint(Point p);
	void setInner(bool i);
private:
	std::vector<Point> vertices;
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

	void setDrawVertices(bool b);
	void setDrawEdges(bool b);
	void setDrawFaces(bool b);
	bool getDrawVertices();
	bool getDrawEdges();
	bool getDrawFaces();

	void updateNorm(int dimension);
	void getNorm(double trans[], double strat[], int dimension);
	//void getNorm(double &trans, double &strat);

private:
	double normTrans[3];
	double normStrat[3];
	//double normTrans;
	//double normStrat;
	bool drawVertices;
	bool drawEdges;
	bool drawFaces;
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Face> faces;
};

