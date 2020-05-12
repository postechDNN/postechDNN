#pragma once
#include "../../DataStrcutures/DCEL/PolygonalDomain.h"
#include "../../DataStrcutures/DCEL/Point.h"
#include "../../DataStrcutures/DCEL/SimplePolygon.h"


class PDgraph{
public:
	struct PDNode {
		Point* p;
		bool issite;
		std::vector<PDNode*>* adjlist;
	};

	PDgraph();
	PDgraph(PolygonalDomain*);
private:
	std::vector<PDNode*>* nodes;
};
