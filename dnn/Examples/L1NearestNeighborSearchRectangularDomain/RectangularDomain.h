#pragma once
#include"CarrierGraph.h"

class RectangularDomain {
protected:
	typedef struct waketemp {
		bool reachable;
		double ld, lu, rd, ru;
	}Wake;
	enum constant{BOUNDINGBOX=-1,FAIL,SUCCESS,INIT=0,DYNAMIC};
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

	void domainconstruct(int);
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
	bool intersect(Rect*, Rect*);
	bool intersect(Rect*, Point*);
public:
	RectangularDomain();
	RectangularDomain(vector<Rect*>, vector<Point*>);
	~RectangularDomain();
	Point* NNS(Point*);
	Point* FNS(Point*);
	vector<Point*> kNNS(Point*,int);
	int insertion(vector<Rect*>, vector<Point*>);
	int insertion(vector<Rect*>);
	int insertion(vector<Point*>);
	int deletion(vector<Rect*>, vector<Point*>);
	int deletion(vector<Rect*>);
	int deletion(vector<Point*>);
	int getdatacnt();
	int getobscnt();
	vector<Rect*> getobstacles();
	vector<Point*> getdata();
	Rect* getobstacle(int);
	Point* getdatum(int);
	Rect* getbbox();
};