#include "PDGraph.h"

int main() {
	Point p1(1, 1);
	Point p2(2, 2);
	Point p3(3, 4);

	PDNode pn1(&p1);
	PDNode pn2(&p2);
	PDNode pn3(&p3);


	pn1.addNode(&pn2);
	pn1.addNode(&pn3);

	pn1.printAdjlist();
	pn2.printAdjlist();
	pn3.printAdjlist();

	//pn2.getAdj()->push_back(&pn1);

	//pn2.adjlist->push_back(&pn1);



	PolygonalDomain pd;
	PDgraph graph;
	//graph.nodes->push_back(&pn1);


	//graph.Dijkstra(&pn1);

	graph.getNodes()->push_back(&pn1);
	graph.getNodes()->push_back(&pn2);
	graph.getNodes()->push_back(&pn3);

	graph.Dijkstra(&pn3);



	return 0;
}
