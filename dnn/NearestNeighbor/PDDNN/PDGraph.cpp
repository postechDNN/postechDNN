#include <vector>
#include <iterator>
#include "PDgraph.h"
#include "../../DataStrcutures/DCEL/PolygonalDomain.h"
#include "../../DataStrcutures/DCEL/SimplePolygon.h"

PDgraph::PDgraph() {
}

PDgraph::PDgraph(PolygonalDomain* pd) {
	for (int i = 0; i < pd->getObstacles()->size(); i++) {
		SimplePolygon* sp = (*pd->getObstacles())[i];

	}
}