#include "SimplePolygon.h"

SimplePolygon::SimplePolygon() {
}

SimplePolygon::SimplePolygon(std::vector<Edge*>*) {
}

std::vector<Edge*>* SimplePolygon::getEdges() {
	return this->edges;
}

