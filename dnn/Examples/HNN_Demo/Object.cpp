#include "pch.h"
#include "Object.h"

#include <map>
#include <set>
#include <fstream>
#include <queue>

#define PADDING 0.1

// Point
OGL_Point::OGL_Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
OGL_Point::OGL_Point(double x, double y) {
	this->x = x;
	this->y = y;
	this->z = 0;
}
OGL_Point::OGL_Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
OGL_Point::~OGL_Point() {

}
double OGL_Point::getX() {
	return this->x;
}
double OGL_Point::getY() {
	return this->y;
}
double OGL_Point::getZ() {
	return this->z;
}
void OGL_Point::setX(double x) {
	this->x = x;
}
void OGL_Point::setY(double y) {
	this->y = y;
}
void OGL_Point::setZ(double z) {
	this->z = z;
}

// Vertex
OGL_Vertex::OGL_Vertex() {
	this->isCustom = false;
}
OGL_Vertex::OGL_Vertex(double x, double y, double z) {
	this->setPos(x, y, z);
	this->isCustom = false;
}
OGL_Vertex::~OGL_Vertex() {
	
}
std::string OGL_Vertex::getName() {
	return this->name;
}
OGL_Point& OGL_Vertex::getPos() {
	return this->pos;
}
void OGL_Vertex::setName(std::string n) {
	this->name = n;
}
void OGL_Vertex::setPos(double x, double y) {
	this->pos.setX(x);
	this->pos.setY(y);
}
void OGL_Vertex::setPos(double x, double y, double z) {
	this->pos.setX(x);
	this->pos.setY(y);
	this->pos.setZ(z);
}

// Edge
OGL_Edge::OGL_Edge() {
	OGL_Point p;
	endpoints.push_back(p);
	endpoints.push_back(p);
	this->isCustom = false;
}
OGL_Edge::OGL_Edge(OGL_Point s, OGL_Point e) {
	endpoints.push_back(s);
	endpoints.push_back(e);
	this->isCustom = false;
}
OGL_Edge::~OGL_Edge() {
	this->endpoints.clear();
}
std::string OGL_Edge::getName() {
	return this->name;
}
OGL_Point& OGL_Edge::getStartP() {
	return this->endpoints[0];
}
OGL_Point& OGL_Edge::getEndP() {
	return this->endpoints[1];
}
std::string OGL_Edge::getFace() {
	return this->face;
}
std::string OGL_Edge::getTwin() {
	return this->twin;
}
std::string OGL_Edge::getNext() {
	return this->next;
}
void OGL_Edge::setName(std::string n) {
	this->name = n;
}
void OGL_Edge::setStartP(OGL_Point P) {
	this->endpoints[0] = P;
}
void OGL_Edge::setEndP(OGL_Point P) {
	this->endpoints[1] = P;
}
void OGL_Edge::setFace(std::string f) {
	this->face = f;
}
void OGL_Edge::setTwin(std::string t) {
	this->twin = t;
}
void OGL_Edge::setNext(std::string n) {
	this->next = n;
}

// Face
OGL_Face::OGL_Face() {
	this->isCustom = false;
}
OGL_Face::~OGL_Face() {
	this->vertices.clear();
}
std::string OGL_Face::getName() {
	return this->name;
}
int OGL_Face::getSize() {
	return this->vertices.size();
}
int OGL_Face::getInnerEdgeSize() {
	return this->innerEdge.size();
}
OGL_Point& OGL_Face::getPoint(int idx) {
	return this->vertices[idx];
}
bool OGL_Face::isInner() {
	return this->inner;
}
void OGL_Face::addPoint(OGL_Point p) {
	this->vertices.push_back(p);
}
void OGL_Face::addInnerEdge(std::string edge) {
	this->innerEdge.push_back(edge);
}
void OGL_Face::setName(std::string n) {
	this->name = n;
}
void OGL_Face::setInner(bool i) {
	this->inner = i;
}
std::string OGL_Face::getInnerEdge(int idx) {
	return this->innerEdge[idx];
}

// Object
Object::Object() {

}
Object::~Object() {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
	this->path.clear();
}

void Object::total_clear() {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
	this->path.clear();
}

int Object::getVerticsNum() {
	return this->vertices.size();
}
int Object::getEdgesNum() {
	return this->edges.size();
}
int Object::getFacesNum() {
	return this->faces.size();
}
int Object::getPathNum() {
	return this->path.size();
}
OGL_Vertex& Object::getVertex(int idx) {
	return this->vertices[idx];
}
OGL_Edge& Object::getEdge(int idx) {
	return this->edges[idx];
}
OGL_Face& Object::getFace(int idx) {
	return this->faces[idx];
}
OGL_Edge& Object::getPath(int idx) {
	return this->path[idx];
}
void Object::addVertex() {
	OGL_Vertex v;
	this->vertices.push_back(v);
}
void Object::addVertex(OGL_Vertex v) {
	this->vertices.push_back(v);
}
void Object::addEdge() {
	OGL_Edge e;
	this->edges.push_back(e);
}
void Object::addEdge(OGL_Edge e) {
	this->edges.push_back(e);
}
void Object::addFace() {
	OGL_Face f;
	this->faces.push_back(f);
}
void Object::addFace(OGL_Face f) {
	this->faces.push_back(f);
}
void Object::addPath() {
	OGL_Edge e;
	this->path.push_back(e);
}
void Object::addPath(OGL_Edge e) {
	this->path.push_back(e);
}

void Object::setDrawObject(int i, bool b) {
	if (i < OBJECT_SIZE) {
		this->drawObjects[i] = b;
	}
	
}
bool Object::getDrawObject(int i) {
	if (i < OBJECT_SIZE) {
		return this->drawObjects[i];
	}
	else {
		return false;
	}
}

void Object::updateNorm(int dimension) {
	
	//double min = std::numeric_limits<double>::has_infinity;
	//double max = min*-1;
	double min = -std::numeric_limits<double>::lowest();
	double max = std::numeric_limits<double>::lowest();

	if (dimension > 0) {
		for (int i = 0; i < this->vertices.size(); i++) {
			if (max < vertices[i].getPos().getX()) {
				max = vertices[i].getPos().getX();
			}
			if (min > vertices[i].getPos().getX()) {
				min = vertices[i].getPos().getX();
			}
		}
		for (int i = 0; i < this->edges.size(); i++) {
			if (max < edges[i].getStartP().getX()) {
				max = edges[i].getStartP().getX();
			}
			if (max < edges[i].getEndP().getX()) {
				max = edges[i].getEndP().getX();
			}
			if (min > edges[i].getStartP().getX()) {
				min = edges[i].getStartP().getX();
			}
			if (min > edges[i].getEndP().getX()) {
				min = edges[i].getEndP().getX();
			}
		}

		this->normTrans[0] = (max + min) / 2;
		this->normStrat[0] = (max - min) / 2;
		this->normStrat[0] += this->normStrat[0] * PADDING;
	}

	if (dimension > 1) {
		min = std::numeric_limits<double>::has_infinity;
		max = min * -1;
		for (int i = 0; i < this->vertices.size(); i++) {
			if (max < vertices[i].getPos().getY()) {
				max = vertices[i].getPos().getY();
			}
			if (min > vertices[i].getPos().getY()) {
				min = vertices[i].getPos().getY();
			}
		}
		for (int i = 0; i < this->edges.size(); i++) {
			if (max < edges[i].getStartP().getY()) {
				max = edges[i].getStartP().getY();
			}
			if (max < edges[i].getEndP().getY()) {
				max = edges[i].getEndP().getY();
			}
			if (min > edges[i].getStartP().getY()) {
				min = edges[i].getStartP().getY();
			}
			if (min > edges[i].getEndP().getY()) {
				min = edges[i].getEndP().getY();
			}
		}

		this->normTrans[1] = (max + min) / 2;
		this->normStrat[1] = (max - min) / 2;
		this->normStrat[1] += this->normStrat[1] * PADDING;
	}

	if (dimension > 2) {
		min = std::numeric_limits<double>::has_infinity;
		max = min * -1;
		for (int i = 0; i < this->vertices.size(); i++) {
			if (max < vertices[i].getPos().getZ()) {
				max = vertices[i].getPos().getZ();
			}
			if (min > vertices[i].getPos().getZ()) {
				min = vertices[i].getPos().getZ();
			}
		}
		for (int i = 0; i < this->edges.size(); i++) {
			if (max < edges[i].getStartP().getZ()) {
				max = edges[i].getStartP().getZ();
			}
			if (max < edges[i].getEndP().getZ()) {
				max = edges[i].getEndP().getZ();
			}
			if (min > edges[i].getStartP().getZ()) {
				min = edges[i].getStartP().getZ();
			}
			if (min > edges[i].getEndP().getZ()) {
				min = edges[i].getEndP().getZ();
			}
		}

		this->normTrans[2] = (max + min) / 2;
		this->normStrat[2] = (max - min) / 2;
		this->normStrat[2] += this->normStrat[2] * PADDING;
	}
}

void Object::getNorm(double trans[], double strat[], int dimension) {
	for (int i = 0; i < dimension; i++) {
		trans[i] = this->normTrans[i];
		strat[i] = this->normStrat[i];
	}
}

void Object::setNorm(int tx, int ty, int width, int height) {
	this->normTrans[0] = tx;
	this->normTrans[1] = ty;
	this->normStrat[0] = width;
	this->normStrat[1] = height;
}

