#include "pch.h"
#include "Object.h"


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