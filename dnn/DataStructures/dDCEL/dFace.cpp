#include "dPoint.h"
#include "dEdge.h"
#include "dFace.h"

#define ERR 1e-6

dFace::dFace(int d) {
	this->d = d;
	this->innerEdges = nullptr;
	this->innerFaces = nullptr;
}

dFace::~dFace() {
	delete(this->d);
	delete(this->innerEdges);
	delete(this->innerFaces);
}

//getters
std::vector<dEdge*>* dFace::getInnerEdges() {
	return this->innerEdges;
};

std::vector<dFace*>* dFace::getInnerFaces() {
	return this->innerFaces;
};

int dFace::getd(int d) {
	return this->d;
};

//setters
void Face::setInnerEdges(std::vector<dEdge*>* edges) {
	this->innerEdges = edges;
}

void Face::setInnerFaces(std::vector<dFace*>* faces) {
	this->innerFaces = faces;
}

//
void dFace::addInnerEdge(dEdge* e) {
	this->innerEdges.push_back(e);
};

void dFace::addInnerFace(dFace* f) {
	this->innerFaces.push_back(f);
};


