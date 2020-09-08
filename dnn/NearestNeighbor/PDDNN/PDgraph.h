#pragma once
#include "../../DataStructures/DCEL/PolygonalDomain.h"
#include "../../DataStructures/DCEL/Point.h"
#include "../../DataStructures/DCEL/SimplePolygon.h"
#include "PDNode.h"
#include <vector>

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
	PDgraph(PolygonalDomain*, std::vector<Point*>*);
	std::vector<PDNode*>* getNodes();
	void Dijkstra(PDNode*);
	void minimumDistance(std::vector <std::pair<PDNode*, double>>);

private:
	std::vector<PDNode*>* nodes;
};
