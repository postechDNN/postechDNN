#include"Point.h"
#include"Edge.h"
#include"Segment.h"
#include <vector>
using namespace std;

class Rect {
	double l;
	double r;
	double t;
	double b;
	int id;
	Point *lt;
	Point *lb;
	Point *rt;
	Point *rb;
	Point *p;
	bool pcheck;

public:
	Rect();
	Rect(Point*);
	Rect(double, double);
	Rect(double, double, double, double);
	double getl(void);
	double getr(void);
	double gett(void);
	double getb(void);
	int getid(void);
	bool isPoint(void);
	Point* getlt(void);
	Point* getlb(void);
	Point* getrt(void);
	Point* getrb(void);
	Point* getp(void);

	void setid(int);
};