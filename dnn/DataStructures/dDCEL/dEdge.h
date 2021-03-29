#pragma once
#include "dPoint.h"

class dEdge{
protected:
	dPoint *s, *t;
public:
	dEdge();
	dEdge(dPoint*,dPoint*);
	dEdge(dEdge*);
	~dEdge();

	bool operator==(dEdge);
	bool on(dPoint*);
	dPoint* crossing(dEdge*,bool);
	dPoint* get_s();
	dPoint* get_t();
	void set_s(dPoint*);
	void set_t(dPoint*);
	double length();
}