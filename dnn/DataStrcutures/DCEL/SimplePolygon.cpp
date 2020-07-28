#include "SimplePolygon.h"

SimplePolygon::SimplePolygon() {
	this->edges = new std::vector<Edge*>();
}

SimplePolygon::SimplePolygon(std::vector<Edge*>* sp) {
	this->edges = sp;
}

std::vector<Edge*>* SimplePolygon::getEdges() {
	return this->edges;
}