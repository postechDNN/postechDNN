#pragma once
#include "../../../DataStructures/DCEL/PolygonalDomain.h"

class PDNode {
private:
	Point* p;
	bool site;
	double dist;
	std::vector<PDNode*>* adjlist;

public:
	Point* getPoint();
	void setPoint(Point*);
	bool issite();
	void setSite(bool);
	double getDist();
	void setDist(double);
	std::vector<PDNode*>* getAdj();
	void setAdj(std::vector<PDNode*>*);
	bool operator==(PDNode);

	void printAdjlist();
	void addNode(PDNode*);

	PDNode();
	PDNode(Point*);
	PDNode(Point*, std::vector<PDNode*>*);
};