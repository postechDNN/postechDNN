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
}
OGL_Edge::OGL_Edge(OGL_Point s, OGL_Point e) {
	endpoints.push_back(s);
	endpoints.push_back(e);
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
OGL_Vertex& Object::getVertex(int idx) {
	return this->vertices[idx];
}
OGL_Edge& Object::getEdge(int idx) {
	return this->edges[idx];
}
OGL_Face& Object::getFace(int idx) {
	return this->faces[idx];
}
void Object::addVertex() {
	OGL_Vertex v;
	this->vertices.push_back(v);
}
void Object::addEdge() {
	OGL_Edge e;
	this->edges.push_back(e);
}
void Object::addFace() {
	OGL_Face f;
	this->faces.push_back(f);
}
void Object::addFace(OGL_Face f) {
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

	std::vector<OGL_Face> faceList;
	std::map<std::string, int> fMap;
	for (int i = 0; i < fn; i++) {
		file >> key;
		fMap[key] = i;
		OGL_Face f;
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
		OGL_Point p = this->vertices[vMap[vkey]].getPos();
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

void Object::read3Deps(CString path) 
{
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();

	std::ifstream file(path);
	int frptn, polyn;
	file >> frptn >> polyn;

	std::string key;
	std::map<std::string, int> frptMap;
	double x, y, z;
	for (int i = 0; i < frptn; i++) {
		file >> key >> x >> y >> z;
		this->addVertex();
		this->vertices[i].setName(key);
		this->vertices[i].setPos(x, y, z);
	}

	Object* polytopes = new Object[polyn];

	for (int i = 0; i < polyn; i++) {
		int vn, en, fn;
		file >> vn >> en >> fn;

		std::map<std::string, int> vMap;
		for (int j = 0; j < vn; j++) {
			file >> key >> x >> y >> z;
			vMap[key] = j;
			this->addVertex();
			polytopes[i].vertices[j].setName(key);
			polytopes[i].vertices[j].setPos(x, y, z);
		}

		std::map<std::string, int> eMap;
		std::string v1, v2, v3;
		for (int i = 0; i < en; i++) {
			file >> key >> v1 >> v2;
			eMap[key] = i;
			OGL_Point p = this->vertices[vMap[v1]].getPos();
			OGL_Point q = this->vertices[vMap[v2]].getPos();
			polytopes[i].addEdge();
			polytopes[i].edges[i].setName(key);
			polytopes[i].edges[i].setStartP(p);
			polytopes[i].edges[i].setEndP(q);
		}

		std::vector<OGL_Face> faceList;
		std::map<std::string, int> fMap;
		for (int i = 0; i < fn; i++) {
			file >> key >> v1 >> v2 >> v3;
			fMap[key] = i;
			OGL_Point p = this->vertices[vMap[v1]].getPos();
			OGL_Point q = this->vertices[vMap[v2]].getPos();
			OGL_Point r = this->vertices[vMap[v3]].getPos();
			OGL_Face f;
			f.addPoint(p); f.addPoint(q); f.addPoint(r);
			f.setName(key);
			faceList.push_back(f);
			faceList[i].setName(key);
		}
	}

	this->updateNorm(3);

	frptMap.clear();
}