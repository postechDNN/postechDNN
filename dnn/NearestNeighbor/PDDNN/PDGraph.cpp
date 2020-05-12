#include "PDGraph.h"


PDgraph::PDgraph() {
}

PDgraph::PDgraph(PolygonalDomain* pd) {
	this->nodes = new std::vector<PDNode*>();
	
	for (int i = 0; i < pd->getObstacles()->size(); i++) {
		SimplePolygon* sp = (*pd->getObstacles())[i];
		for (int j = 0; j < sp->getEdges()->size(); j++) {
			Edge* e = (*sp->getEdges())[j];
			PDNode* n = new PDNode();
			n->p = e->gets();
			n->issite = false;
			n->adjlist = new std::vector<PDNode*>();
		}
	}

	for (int i = 0; i < this->nodes->size(); i++) {
		for (int j = i + 1; j < this->nodes->size(); j++) {
			Edge* ne = new Edge((*this->nodes)[i]->p, (*this->nodes)[j]->p);
			bool flag = true;
			for (int k = 0; k < pd->getObstacles()->size(); k++) {
				SimplePolygon* sp = (*pd->getObstacles())[k];
				for (int l = 0; l < sp->getEdges()->size(); l++) {
					Edge* te = (*sp->getEdges())[l];
					if (!((*ne) == (*te)) && ne->crossing(te, false)) {
						flag = false;
						break;
					}
				}
				if (!flag)
					break;
			}
			if (flag) {
				(*this->nodes)[i]->adjlist->push_back((*this->nodes)[j]);
				(*this->nodes)[j]->adjlist->push_back((*this->nodes)[i]);
			}
			delete(ne);
		}
	}
}