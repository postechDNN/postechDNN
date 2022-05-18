#include "pch.h"
#include "Object.h"

#define PADDING 0.1

// Point
Point::Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
	this->z = 0;
}
Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Point::~Point() {

}
double Point::getX() {
	return this->x;
}
double Point::getY() {
	return this->y;
}
double Point::getZ() {
	return this->z;
}
void Point::setX(double x) {
	this->x = x;
}
void Point::setY(double y) {
	this->y = y;
}
void Point::setZ(double z) {
	this->z = z;
}

// Vertex
Vertex::Vertex() {

}
Vertex::~Vertex() {
	
}
Point& Vertex::getPos() {
	return this->pos;
}
void Vertex::setPos(double x, double y) {
	this->pos.setX(x);
	this->pos.setY(y);
}
void Vertex::setPos(double x, double y, double z) {
	this->pos.setX(x);
	this->pos.setY(y);
	this->pos.setZ(z);
}

// Edge
Edge::Edge() {
	Point p;
	endpoints.push_back(p);
	endpoints.push_back(p);
}
Edge::Edge(Point s, Point e) {
	endpoints.push_back(s);
	endpoints.push_back(e);
}
Edge::~Edge() {
	this->endpoints.clear();
}
Point& Edge::getStartP() {
	return this->endpoints[0];
}
Point& Edge::getEndP() {
	return this->endpoints[1];
}
void Edge::setStartP(Point P) {
	this->endpoints[0] = P;
}
void Edge::setEndP(Point P) {
	this->endpoints[1] = P;
}

// Face
Face::Face() {

}
Face::~Face() {
	this->vertices.clear();
}
int Face::getSize() {
	return this->vertices.size();
}
Point& Face::getPoint(int idx) {
	return this->vertices[idx];
}
bool Face::isInner() {
	return this->inner;
}
void Face::addPoint(Point p) {
	this->vertices.push_back(p);
}
void Face::setInner(bool i) {
	this->inner = i;
}

// Object
Object::Object() {

}
Object::~Object() {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
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
Vertex& Object::getVertex(int idx) {
	return this->vertices[idx];
}
Edge& Object::getEdge(int idx) {
	return this->edges[idx];
}
Face& Object::getFace(int idx) {
	return this->faces[idx];
}
void Object::addVertex() {
	Vertex v;
	this->vertices.push_back(v);
}
void Object::addEdge() {
	Edge e;
	this->edges.push_back(e);
}
void Object::addFace() {
	Face f;
	this->faces.push_back(f);
}
void Object::setDrawVertices(bool b) {
	this->drawVertices = b;
}
void Object::setDrawEdges(bool b) {
	this->drawEdges = b;
}
void Object::setDrawFaces(bool b) {
	this->drawFaces = b;
}
bool Object::getDrawVertices() {
	return this->drawVertices;
}
bool Object::getDrawEdges() {
	return this->drawEdges;
}
bool Object::getDrawFaces() {
	return this->drawFaces;
}

void Object::updateNorm(int dimension) {
	
	double min = std::numeric_limits<double>::has_infinity;
	double max = min*-1;
	

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

/*
void Object::updateNorm(int dimension) {

	double min = std::numeric_limits<double>::has_infinity;
	double max = min*-1;


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
		this->normTrans = (max + min) / 2;
		this->normStrat = (max - min) / 2;
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
		double temp1 = (max + min) / 2;
		double temp2 = (max - min) / 2;
		if (this->normStrat < temp2) {
			this->normTrans = temp1;
			this->normStrat = temp2;
		}
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
		double temp1 = (max + min) / 2;
		double temp2 = (max - min) / 2;
		if (this->normStrat < temp2) {
			this->normTrans = temp1;
			this->normStrat = temp2;
		}
	}
}
*/

void Object::getNorm(double trans[], double strat[], int dimension) {
	for (int i = 0; i < dimension; i++) {
		trans[i] = this->normTrans[i];
		strat[i] = this->normStrat[i];
	}
}
/*
void Object::getNorm(double &trans, double &strat) {
	trans = this->normTrans;
	strat = this->normStrat;
}
*/