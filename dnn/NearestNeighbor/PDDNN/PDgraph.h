#pragma once
#include "../../DataStrcutures/DCEL/PolygonalDomain.h"
#include "../../DataStrcutures/DCEL/Point.h"
#include "../../DataStrcutures/DCEL/SimplePolygon.h"
#include "PDNode.h"

class PDgraph{
public:
	// implementing a min-heap using prioirty queue
	struct cmp {
		bool operator() (const std::pair<PDNode*, double>& a, const std::pair<PDNode*, double>& b) {
			if (a.second < b.second) {
				return false;
			}
			else {
				return true;
			}
		}
	};

	PDgraph();
	PDgraph(PolygonalDomain*);
	std::vector<PDNode*>* getNodes();
	void Dijkstra(PDNode*);
	void minimumDistance(std::vector <std::pair<PDNode*, double>>);

private:
	std::vector<PDNode*>* nodes;
	PolygonalDomain* space;
};
