#include "PDGraph.h"
#include <queue>
#include <map>

PDgraph::PDgraph() {
	this->nodes = new std::vector<PDNode*>();
}

PDgraph::PDgraph(PolygonalDomain* _pd, std::vector<Point*>* pv) {
	this->pd = _pd;
	this->nodes = new std::vector<PDNode*>();
	for (int i = 0; i < pd->getObstacles()->size(); i++) {
		SimplePolygon* sp = (*pd->getObstacles())[i];
		for (int j = 0; j < sp->getEdges()->size(); j++) {
			Edge* e = (*sp->getEdges())[j];
			PDNode* n = new PDNode();
			n->setPoint(e->gets());
			n->setSite(false);
			n->setAdj(new std::vector<PDNode*>());
		}
	}

	for (int i = 0; i < pv->size(); i++) {
		PDNode* n = new PDNode();
		n->setPoint((*pv)[i]);
		n->setSite(true);
		n->setAdj(new std::vector<PDNode*>());
	}

	for (int i = 0; i < this->nodes->size(); i++) {
		for (int j = i + 1; j < this->nodes->size(); j++) {
			Edge* ne = new Edge((*this->nodes)[i]->getPoint(), (*this->nodes)[j]->getPoint());
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
				(*this->nodes)[i]->getAdj()->push_back((*this->nodes)[j]);
				(*this->nodes)[j]->getAdj()->push_back((*this->nodes)[i]);
			}
			delete(ne);
		}
	}
}


std::vector<PDNode*>* PDgraph::getNodes() {
	return this->nodes;
}

std::vector<std::pair<Point*, double>>* PDgraph::knn(Point* start, int k) {

	std::priority_queue <std::pair<PDNode*, double>, std::vector <std::pair<PDNode*, double>>, struct cmp> PQ;
	for (int i = 0; i < this->nodes->size(); i++) {
		Edge* ne = new Edge((*this->nodes)[i]->getPoint(), start);
		bool flag = true;
		for (int j = 0; j < pd->getObstacles()->size(); j++) {
			SimplePolygon* sp = (*pd->getObstacles())[j];
			for (int k = 0; k < sp->getEdges()->size(); k++) {
				Edge* te = (*sp->getEdges())[k];
				if (!((*ne) == (*te)) && ne->crossing(te, false)) {
					flag = false;
					break;
				}
			}
			if (!flag)
				break;
		}
		if (flag) {
			PQ.push(std::pair<PDNode*, double>((*this->nodes)[i], start->distance((*this->nodes)[i]->getPoint())));
		}
		delete(ne);
	}
	std::map <PDNode*, bool> visited;
	std::vector<std::pair<Point*, double>>* nns = new std::vector<std::pair<Point*, double>>;

	while (!PQ.empty() && nns->size() < k) {
		PDNode* here = PQ.top().first;
		double cost = PQ.top().second;

		PQ.pop();

		if (visited[here] == true) continue;

		visited[here] = true;
		if (here->issite()) {
			nns->push_back(std::pair<Point*, double>(here->getPoint(), cost));
		}

		for (int i = 0; i < here->getAdj()->size(); i++) {
			if (visited[(*here->getAdj())[i]] == true) continue;
			PDNode* next = (*here->getAdj())[i];
			double acost = (*here->getAdj())[i]->getPoint()->distance(here->getPoint()) + cost;
			PQ.push({ next, acost });
		}
	}
}

