#ifndef QUADEDGE_H
#define QUADEDGE_H

#include "geom2d.h"

#include "./../../Algorithms/ESP_plane/Graph.h"

#include "./../../Algorithms/ESP_plane/Point.h"

class QuadEdge;

class Edge2d {
	friend QuadEdge;
	friend void Splice(Edge2d*, Edge2d*);
  private:
	int num;
	Edge2d *next;
	Point2d *data;
  public:
	Edge2d()			{ data = 0; }
	Edge2d* Rot();
	Edge2d* invRot();
	Edge2d* Sym();
	Edge2d* Onext();
	Edge2d* Oprev();
	Edge2d* Dnext();
	Edge2d* Dprev();
	Edge2d* Lnext();
	Edge2d* Lprev();
	Edge2d* Rnext();
	Edge2d* Rprev();
	Point2d* Org();
	Point2d* Dest();
	const Point2d& Org2d() const;
	const Point2d& Dest2d() const;
	void  EndPoints(Point2d*, Point2d*);
	QuadEdge* Qedge()	{ return (QuadEdge *)(this - num); }
	void Draw(unsigned int, std::vector<std::vector<std::pair<double, double>>> &,  Graph *);
	void EdgeDraw();
};

class QuadEdge {
	friend Edge2d *MakeEdge();
  private:
	Edge2d e[4];
	unsigned int ts;
  public:
	QuadEdge();
	int TimeStamp(unsigned int);
};

class Subdivision {
  private:
	Edge2d *Locate(const Point2d&);
  public:
	Edge2d *startingEdge;
	Subdivision(const Point2d&, const Point2d&);
	Subdivision(const Point2d&, const Point2d&, const Point2d&);
	Subdivision(const Point2d&, const Point2d&, const Point2d&, int);
	void InsertSite(const Point2d&);
	void Draw();
};

inline QuadEdge::QuadEdge()
{
	e[0].num = 0, e[1].num = 1, e[2].num = 2, e[3].num = 3;
	e[0].next = &(e[0]); e[1].next = &(e[3]);
	e[2].next = &(e[2]); e[3].next = &(e[1]);
	ts = 0;
}

inline int QuadEdge::TimeStamp(unsigned int stamp)
{
	if (ts != stamp) {
		ts = stamp;
		return TRUE;
	} else
		return FALSE;
}

/************************* Edge Algebra *************************************/

inline Edge2d* Edge2d::Rot()
// Return the dual of the current edge, directed from its right to its left.
{
	return (num < 3) ? this + 1 : this - 3;
}

inline Edge2d* Edge2d::invRot()
// Return the dual of the current edge, directed from its left to its right.
{
	return (num > 0) ? this - 1 : this + 3;
}

inline Edge2d* Edge2d::Sym()
// Return the edge from the destination to the origin of the current edge.
{
	return (num < 2) ? this + 2 : this - 2;
}

inline Edge2d* Edge2d::Onext()
// Return the next ccw edge around (from) the origin of the current edge.
{
	return next;
}

inline Edge2d* Edge2d::Oprev()
// Return the next cw edge around (from) the origin of the current edge.
{
	return Rot()->Onext()->Rot();
}

inline Edge2d* Edge2d::Dnext()
// Return the next ccw edge around (into) the destination of the current edge.
{
	return Sym()->Onext()->Sym();
}

inline Edge2d* Edge2d::Dprev()
// Return the next cw edge around (into) the destination of the current edge.
{
	return invRot()->Onext()->invRot();
}

inline Edge2d* Edge2d::Lnext()
// Return the ccw edge around the left face following the current edge.
{
	return invRot()->Onext()->Rot();
}

inline Edge2d* Edge2d::Lprev()
// Return the ccw edge around the left face before the current edge.
{
	return Onext()->Sym();
}

inline Edge2d* Edge2d::Rnext()
// Return the edge around the right face ccw following the current edge.
{
	return Rot()->Onext()->invRot();
}

inline Edge2d* Edge2d::Rprev()
// Return the edge around the right face ccw before the current edge.
{
	return Sym()->Onext();
}

/************** Access to data pointers *************************************/

inline Point2d* Edge2d::Org()
{
	return data;
}

inline Point2d* Edge2d::Dest()
{
	return Sym()->data;
}

inline const Point2d& Edge2d::Org2d() const
{
	return *data;
}

inline const Point2d& Edge2d::Dest2d() const
{
	return (num < 2) ? *((this + 2)->data) : *((this - 2)->data);
}

inline void Edge2d::EndPoints(Point2d* origin, Point2d* de)
{
	data = origin;
	Sym()->data = de;
}



/******************** I added ****************/
Edge2d* MakeEdge();
void Splice(Edge2d* a, Edge2d* b);
void DeleteEdge(Edge2d* e);
Edge2d* Connect(Edge2d* a, Edge2d* b);
void Swap(Edge2d* e);
inline Real TriArea(const Point2d& a, const Point2d& b, const Point2d& c);
int InCircle(const Point2d& a, const Point2d& b,
			 const Point2d& c, const Point2d& d);
int ccw(const Point2d& a, const Point2d& b, const Point2d& c);
int RightOf(const Point2d& x, Edge2d* e);
int LeftOf(const Point2d& x, Edge2d* e);
int OnEdge(const Point2d& x, Edge2d* e);

void myInit(void);
void myDisplay(void);
bool LowerThan(Point2d a, Point2d b);
bool LessThan(Point2d a, Point2d b);
/********************************************/

#endif /* QUADEDGE_H */