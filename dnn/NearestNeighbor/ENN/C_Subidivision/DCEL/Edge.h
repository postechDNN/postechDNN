#pragma once
#include "Point.h"

class Edge {
protected:
	Point s,  t;
public:
	Edge();
	Edge(const Point& _s, const Point& _t);
	Edge(const Edge&);
	~Edge();
	bool operator==(const Edge&);
	bool on(Point& p);
	Edge* crossing(Edge&, bool);
	Point gets() const;
	Point gett() const;
	void sets(Point&);
	void sett(Point&);
	double length();

	friend std::ostream& operator<<(std::ostream& os, const Edge& p); //For testing

	Edge& operator=(const Edge&);
	//void operator=(Edge);
};
