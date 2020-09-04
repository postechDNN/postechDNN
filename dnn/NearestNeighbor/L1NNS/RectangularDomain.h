#pragma once
#include"CarrierGraph.h"
#include<algorithm>

class RectangularDomain {
protected:
	typedef struct waketemp {
		bool reachable;
		double ld, lu, rd, ru;
	}Wake;
	enum constant{BOUNDINGBOX=-1};
	const double INF = 2147483647.0;

	vector < Rect* > obstacles;
	vector < Rect* > lfromld, dfromld;
	vector < Rect* > lfromlu, ufromlu;
	vector < Rect* > rfromrd, dfromrd;
	vector < Rect* > rfromru, ufromru;

	vector < Point* > data;
	vector < double > dist;
	vector < Rect* > lfromp, rfromp, ufromp, dfromp;

	vector < Point* > querylog;
	Rect* bbox;

	vector < vector < Wake* > > lwake, rwake, uwake, dwake;

	CarrierGraph *xpos, *xneg, *ypos, *yneg;

	int obscnt;
	int datacnt;

	void domainconstruct();
	void setray();
	void setlwake();
	void setrwake();
	void setuwake();
	void setdwake();
	void lwakeNNS(Point*);
	void rwakeNNS(Point*);
	void uwakeNNS(Point*);
	void dwakeNNS(Point*);
	Rect* getleftray(double, double);
	Rect* getrightray(double, double);
	Rect* getupray(double, double);
	Rect* getdownray(double, double);
	bool ishit(double, double, double);
	bool isclosest(double, double, double);
	double threemin(double, double, double);
public:
	RectangularDomain();
	RectangularDomain(vector<Rect*>, vector<Point*>);
	~RectangularDomain();
	Point* NNS(Point*);
	Point* FNS(Point*);
	vector<Point*> kNNS(Point*,int);
};