#include"Point.h"
#include"Edge.h"
#include"Segment.h"
#include <vector>
using namespace std;

typedef struct temp {
	bool reachable;
	double ul;
	double br;
}TMP;

class Rectangle {
public:
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
	bool isPoint;
	vector < Segment > shadow[8];
	/*
	shadow는 lwake의 shadow[0]부터 시계방향으로
	 6 7
	5   0
	4   1
	 3 2
	*/
	vector < TMP > lwake;
	vector < TMP > rwake;
	vector < TMP > twake;
	vector < TMP > bwake;
	
	Rectangle();
	Rectangle(double, double);
	Rectangle(double, double, double, double);
	double getl(void);
	double getr(void);
	double gett(void);
	double getb(void);
	void setid(int);
};