#ifndef DDCEL_EDGE
#define DDCEL_EDGE
//#include "dPoint.h"
class dPoint;
class dEdge{
protected:
	dPoint *s, *t;
public:
	dEdge(); //default constructor
	dEdge(dPoint*,dPoint*); //simple implicit constructor
	dEdge(dEdge*); //copy constructor
	~dEdge(); //destructor

	bool operator==(const dEdge);
	bool on(dPoint*) const;

	//if not crossing, return NULL
	//otherwise, return non-NULL, 
	//newly allocated element
	dPoint* crossing(const dEdge*,bool c=true) const;
	bool is_crossing(const dEdge*,bool c=true) const;
	dPoint* get_s() const;
	dPoint* get_t() const;
	void set_s(dPoint*);
	void set_t(dPoint*);
	double length(const int p=2) const; //Consider L_p distance, regard p=0 as L_inf
};

#endif