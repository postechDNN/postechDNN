#pragma once
#include "Edge.h"


class Site {
protected:
	Point p;
	Edge onEdge;
	Edge negSlope;
	Edge posSlope;
public:
	Site(Point&, Edge&);
	~Site();
	Point getPoint();
	Edge getOnEdge();
	Edge getNegSlope();
	Edge getPosSlope();
	void setOnEdge(Edge&);
	void setNegSlope(Edge&);
	void setPosSlope(Edge&);
};