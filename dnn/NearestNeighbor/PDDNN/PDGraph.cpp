#include "PDGraph.h"
#include "PDNode.h"
#include <map>

PDgraph::PDgraph() {
	this->nodes = new std::vector<PDNode*>();
}

PDgraph::PDgraph(PolygonalDomain* pd) {

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

	for (int i = 0; i < this->nodes->size(); i++) {
		for (int j = i + 1; j < this->nodes->size(); j++) {

			if (((*this->nodes)[i]->issite() == true) && ((*this->nodes)[j]->issite() == true)) {
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
}


std::vector<PDNode*>* PDgraph::getNodes() {
	return this->nodes;
}

void PDgraph::Dijkstra(PDNode* startingPDNode) {

	std::priority_queue <std::pair<PDNode*, double>, std::vector <std::pair<PDNode*, double>>, cmp> PQ;
	PQ.push({ startingPDNode, 0 });
	std::vector <std::pair<PDNode*, double> > dist;
	dist.resize(this->nodes->size());
	std::map <PDNode*, bool> visited;

	std::cout << "[dist vector initializes as -1]" << std::endl;
	for (int i = 0; i < dist.size(); i++) {
		dist[i] = std::make_pair((*this->nodes)[i], -1);
	}

	std::cout << dist.size() << std::endl << std::endl;


	for (int i = 0; i < dist.size(); i++) {
		std::cout << dist[i].first->getPoint()->getx() << "," << dist[i].first->getPoint()->gety() << std::endl;
		std::cout << dist[i].second << std::endl << std::endl;

	}


	//int location = 0;
	while (!PQ.empty()) {
		PDNode* here = PQ.top().first;
		double cost = PQ.top().second;
		std::cout << "here: [" << here->getPoint()->getx() << "," << here->getPoint()->gety() << "]: " << cost << std::endl << std::endl;

		PQ.pop();

		if (visited[here] == true) continue;

		int location = 0;
		for (int i = 0; i < dist.size(); i++) {
			if (dist[i].first == here) {
				location = i;
				break;
			}
		}

		dist[location].second = cost;
		visited[here] = true;



		std::cout << here->getAdj()->size() << std::endl;
		here->printAdjlist();

		location++;

		here->printAdjlist();

		for (int i = 0; i < here->getAdj()->size(); i++) {
			//std::cout << "dddd" << std::endl;



			//printf("\nnext index %d\n", i);
			PDNode* next = (*here->getAdj())[i];
			double acost = (*here->getAdj())[i]->getPoint()->distance(here->getPoint()) + cost;

			std::cout << "next: [" << next->getPoint()->getx() << "," << next->getPoint()->gety() << "]: " << acost << std::endl << std::endl;


			if (visited[(*here->getAdj())[i]] == true) continue;

			PQ.push({ next, acost });
		}
	}

	minimumDistance(dist);
}

void PDgraph::minimumDistance(std::vector <std::pair<PDNode*, double> > dist) {
	std::cout << "dist print" << std::endl;


	for (int i = 0; i < dist.size(); i++) {
		std::cout << dist[i].first->getPoint()->getx() << "," << dist[i].first->getPoint()->gety() << std::endl;
		//std::cout << it->first->adjlist << std::endl;

		if (dist[i].second == -1) {
			std::cout << "INF" << std::endl;
		}

		else {
			std::cout << dist[i].second << std::endl;
		}
		std::cout << std::endl;
	}
}