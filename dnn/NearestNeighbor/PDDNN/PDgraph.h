#pragma once
#include <vector>
#include "../../DataStrcutures/DCEL/PolygonalDomain.h"
#include "../../DataStrcutures/DCEL/Point.h"

class PDgraph{
public:
	struct PDNode {
		Point* p;
		bool issite;
		std::vector<PDNode*> adjlist;
	};
	PDgraph();
	PDgraph(PolygonalDomain*);
private:
	std::vector<PDNode*> nodes;
};
