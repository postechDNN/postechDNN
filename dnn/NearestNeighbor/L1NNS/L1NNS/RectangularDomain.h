#pragma once
#include"CGraph.h"
#include<vector>
#include<algorithm>
using namespace std;

typedef struct temp {
	bool reachable;
	double ul;
	double br;
}Wake;

typedef struct temp2 {
	Point* p;
	double distance;
}Point_Distance;
/*
shadow는 lwake의 shadow[0]부터 시계방향으로
 6 7
5   0
4   1
 3 2
*/

class RectangularDomain {
private:
	vector < Rectangle > rectset; // input rectangles and points
	Rectangle bbox;
	vector<vector<Wake>> lcheck, rcheck, tcheck, bcheck;
	vector < vector <Segment> > rup, urp, ulp, lup, ldp, dlp, drp, rdp;
	int n;
	int np;
	double INF;
	Cgraph xpos,xneg,ypos,yneg;
	void makeshadow();
	void makecheck();
public:
	RectangularDomain();
	RectangularDomain(vector<Rectangle>);
	Rectangle getboundary();
	Rectangle getrect(int);
	int rectcnt();
	Cgraph getcarrier(int);
	bool incidentcheck(int,int,int);
	Point_Distance NNS(Point);
	vector<Point_Distance> kNNS(Point,int);
};