#include "pch.h"
#include "Object.h"

#include <map>
#include <set>
#include <fstream>
#include <queue>

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
std::string Vertex::getName() {
	return this->name;
}
Point& Vertex::getPos() {
	return this->pos;
}
void Vertex::setName(std::string n) {
	this->name = n;
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
std::string Edge::getName() {
	return this->name;
}
Point& Edge::getStartP() {
	return this->endpoints[0];
}
Point& Edge::getEndP() {
	return this->endpoints[1];
}
std::string Edge::getFace() {
	return this->face;
}
std::string Edge::getTwin() {
	return this->twin;
}
std::string Edge::getNext() {
	return this->next;
}
void Edge::setName(std::string n) {
	this->name = n;
}
void Edge::setStartP(Point P) {
	this->endpoints[0] = P;
}
void Edge::setEndP(Point P) {
	this->endpoints[1] = P;
}
void Edge::setFace(std::string f) {
	this->face = f;
}
void Edge::setTwin(std::string t) {
	this->twin = t;
}
void Edge::setNext(std::string n) {
	this->next = n;
}

// Face
Face::Face() {

}
Face::~Face() {
	this->vertices.clear();
}
std::string Face::getName() {
	return this->name;
}
int Face::getSize() {
	return this->vertices.size();
}
int Face::getInnerEdgeSize() {
	return this->innerEdge.size();
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
void Face::addInnerEdge(std::string edge) {
	this->innerEdge.push_back(edge);
}
void Face::setName(std::string n) {
	this->name = n;
}
void Face::setInner(bool i) {
	this->inner = i;
}
std::string Face::getInnerEdge(int idx) {
	return this->innerEdge[idx];
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
void Object::addFace(Face f) {
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

void Object::getNorm(double trans[], double strat[], int dimension) {
	for (int i = 0; i < dimension; i++) {
		trans[i] = this->normTrans[i];
		strat[i] = this->normStrat[i];
	}
}

void Object::readDCEL(CString path, int dimension) {
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();

	std::ifstream file(path);
	int vn, fn, en;
	file >> vn >> fn >> en;

	std::string key;
	std::map<std::string, int> vMap;
	for (int i = 0; i < vn; i++) {
		file >> key;
		vMap[key] = i;
		this->addVertex();
		this->vertices[i].setName(key);
	}

	std::vector<Face> faceList;
	std::map<std::string, int> fMap;
	for (int i = 0; i < fn; i++) {
		file >> key;
		fMap[key] = i;
		Face f;
		faceList.push_back(f);
		faceList[i].setName(key);
		//this->addFace();
	}

	
	std::map<std::string, int> eMap;
	for (int i = 0; i < en; i++) {
		file >> key;
		eMap[key] = i;
		this->addEdge();
		this->edges[i].setName(key);
	}

	double x, y;
	std::string temp;
	for (int i = 0; i < vn; i++) {
		file >> key >> x >> y >> temp;
		this->vertices[vMap[key]].setPos(x, y);
		//this->object2D.getVertex(vMap[key]).setPos(x, y);
	}

	

	std::string se;
	int in, isOuter;
	for (int i = 0; i < fn; i++) {
		file >> key >> isOuter >> se >> in;
		if (se == "NULL") {
			faceList[fMap[key]].setInner(false);
			//this->faces[fMap[key]].setInner(false);
			//this->object2D.getFace(fMap[key]).setInner(false);
		}
		else {
			faceList[fMap[key]].setInner(true);
			//this->faces[fMap[key]].setInner(true);
			//this->object2D.getFace(fMap[key]).setInner(true);
		}
		if (in != 0) {
			for (int j = 0; j < in; j++) {
				file >> se;
				faceList[fMap[key]].addInnerEdge(se);
			}
		}
	}



	std::string vkey, fkey, next, prev, twin;
	for (int i = 0; i < en; i++) {
		file >> key >> vkey >> twin >> fkey >> next >> prev;
		Point p = this->vertices[vMap[vkey]].getPos();
		faceList[fMap[fkey]].addPoint(p);
		//this->faces[fMap[fkey]].addPoint(p);
		this->edges[eMap[key]].setEndP(p);
		this->edges[eMap[next]].setStartP(p);
		this->edges[eMap[key]].setTwin(twin);
		this->edges[eMap[key]].setFace(fkey);
		this->edges[eMap[key]].setNext(next);
	}

	// Find inclusive relationship
	std::vector<std::vector<int>> InnerFace;
	std::vector<int> inDeg(faceList.size(), 0);
	for (int i = 0; i < faceList.size(); i++) {
		std::string nowFace = faceList[i].getName();
		std::vector<int> innerList;
		std::set<std::string> innerFaceList;
		for (int j = 0; j < faceList[i].getInnerEdgeSize(); j++) {
			std::string innerEdge = faceList[i].getInnerEdge(j);
			std::string nowEdge = innerEdge;
			do {
				std::string nowTwin = this->edges[eMap[nowEdge]].getTwin();
				std::string nowTwinFace = this->edges[eMap[nowTwin]].getFace();
				if (nowFace != nowTwinFace) {
					innerFaceList.insert(nowTwinFace);
				}
				nowEdge = this->edges[eMap[nowEdge]].getNext();
			} while (innerEdge != nowEdge);
		}
		for (auto iter = innerFaceList.begin(); iter != innerFaceList.end(); iter++) {
			innerList.push_back(fMap[*iter]);
			inDeg[fMap[*iter]]++;
		}
		InnerFace.push_back(innerList);
	}

	// Topology sorting
	std::vector<int> faceOrder;
	std::queue<int> faceQ;
	for (int i = 0; i < inDeg.size(); i++) {
		if (inDeg[i] == 0) faceQ.push(i);
	}

	while (!faceQ.empty()) {
		int nowIdx = faceQ.front();
		faceQ.pop();
		faceOrder.push_back(nowIdx);
		for (int i = 0; i < InnerFace[nowIdx].size(); i++) {
			inDeg[InnerFace[nowIdx][i]]--;
			if (inDeg[InnerFace[nowIdx][i]] == 0) {
				faceQ.push(InnerFace[nowIdx][i]);
			}
		}
	}

	for (int i = 0; i < faceOrder.size(); i++) {
		this->addFace(faceList[faceOrder[i]]);
	}

	this->updateNorm(2);

	// Clear
	for (int i = 0; i < InnerFace.size(); i++) {
		InnerFace[i].clear();
	}
	InnerFace.clear();
	faceList.clear();
	inDeg.clear();
	fMap.clear();
	eMap.clear();
	vMap.clear();
}