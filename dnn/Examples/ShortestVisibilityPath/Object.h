#pragma once

#include <vector>
#include <string>

#include <GL/gl.h> 
#include <GL/glu.h>

#pragma comment(lib, "opengl32.lib" )

class OGL_Point {
public:
	OGL_Point();
	OGL_Point(double x, double y);
	OGL_Point(double x, double y, double z);
	~OGL_Point();
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

class OGL_Vertex {
public:
	OGL_Vertex();
	OGL_Vertex(double x, double y, double z);
	~OGL_Vertex();
	std::string getName();
	OGL_Point& getPos();
	void setName(std::string n);
	void setPos(double x, double y);
	void setPos(double x, double y, double z);

	bool isCustom;
	GLfloat color[3];
	GLfloat width;

private:
	std::string name;
	OGL_Point pos;
};

class OGL_Edge {
public:
	OGL_Edge();
	OGL_Edge(OGL_Point s, OGL_Point e);
	~OGL_Edge();
	std::string getName();
	OGL_Point& getStartP();
	OGL_Point& getEndP();
	std::string getFace();
	std::string getTwin();
	std::string getNext();
	void setName(std::string n);
	void setStartP(OGL_Point P);
	void setEndP(OGL_Point P);
	void setFace(std::string f);
	void setTwin(std::string t);
	void setNext(std::string n);

	bool isCustom;
	GLfloat color[3];
	GLfloat width;

private:
	std::string name;
	std::vector<OGL_Point> endpoints;
	std::string face;
	std::string twin;
	std::string next;
};

class OGL_Face{
public:
	OGL_Face();
	~OGL_Face();
	std::string getName();
	int getSize();
	int getInnerEdgeSize();
	OGL_Point& getPoint(int idx);
	bool isInner();
	void addPoint(OGL_Point p);
	void addInnerEdge(std::string edge);
	void setName(std::string n);
	void setInner(bool i);
	std::string getInnerEdge(int idx);

	bool isCustom;
	GLfloat color[3];
	GLfloat width;

private:
	std::string name;
	std::vector<OGL_Point> vertices;
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
	int getPathNum();
	OGL_Vertex& getVertex(int idx);
	OGL_Edge& getEdge(int idx);
	OGL_Face& getFace(int idx);
	OGL_Edge& getPath(int idx);
	void addVertex();
	void addVertex(OGL_Vertex v);
	void addEdge();
	void addEdge(OGL_Edge e);
	void addFace();
	void addFace(OGL_Face f);
	void addPath();
	void addPath(OGL_Edge e);

	void setDrawVertices(bool b);
	void setDrawEdges(bool b);
	void setDrawFaces(bool b);
	void setDrawPath(bool b);
	void setDrawLine(bool b);
	bool getDrawVertices();
	bool getDrawEdges();
	bool getDrawFaces();
	bool getDrawPath();
	bool getDrawLine();

	void updateNorm(int dimension);
	void getNorm(double trans[], double strat[], int dimension);

	void setNorm(int tx, int ty, int width, int height);

	void total_clear();

private:
	double normTrans[3];
	double normStrat[3];
	bool drawVertices;
	bool drawEdges;
	bool drawFaces;
	bool drawPath;
	bool drawLine;
	std::vector<OGL_Vertex> vertices;
	std::vector<OGL_Edge> edges;
	std::vector<OGL_Face> faces;
	std::vector<OGL_Edge> path;
};

