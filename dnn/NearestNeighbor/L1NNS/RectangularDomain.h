#pragma once
#include"CarrierGraph.h"

class RectangularDomain {
protected:
	typedef struct waketemp {
		bool reachable; // From i-th rectangle, the boolean value if j-th data point is reachable.
		double ld, lu, rd, ru; // From i-th rectangle and j-th data point is reachable, distance from corners according to the position to j-th point.
	}Wake; // The type for wake used inside of rectangular domain
	enum constant{BOUNDINGBOX=-1,FAIL,SUCCESS,INIT=0,DYNAMIC}; // constant number used {index of bounding box, failure return, success return, initial, state of insertion/deletion}
	const double INF = 2147483647.0; // sufficiently large value

	vector < Rect* > obstacles; // set of obstacles
	vector < Rect* > lfromld, dfromld; // stores rectangle hit first by two axis-aligned rays from the left-bottom corner of i-th rectangle 
	vector < Rect* > lfromlu, ufromlu; // stores rectangle hit first by two axis-aligned rays from the left-top corner of i-th rectangle 
	vector < Rect* > rfromrd, dfromrd; // stores rectangle hit first by two axis-aligned rays from the right-bottom corner of i-th rectangle 
	vector < Rect* > rfromru, ufromru; // stores rectangle hit first by two axis-aligned rays from the right-top corner of i-th rectangle 

	vector < Point* > data; // set of data
	vector < double > dist; // set of distances from query to data. Used for nearest neighbor search.
	vector < Rect* > lfromp, rfromp, ufromp, dfromp; //  stores rectangle hit first by four axis-aligned rays from i-th data point 

	vector < Point* > querylog; // Set of queries. Used for deallocate query.
	Rect* bbox; // bounding box

	vector < vector < Wake* > > lwake, rwake, uwake, dwake; // from each sides of i-th rectangle to j-th data points, {refer Wake structure}.

	CarrierGraph *xpos, *xneg, *ypos, *yneg; // Four carrier graphs with different directions.

	int obscnt; // # of obstacles
	int datacnt; // # of data

	void domainconstruct(int); // Function of construction of rectangular domain
	void setray(); // Function to compute rectangles hit first by rays from objects 
	void setlwake(); // Set wake of left side of rectangle
	void setrwake(); // Set wake of right side of rectangle
	void setuwake(); // Set wake of upper side of rectangle
	void setdwake(); // Set wake of lower side of rectangle
	void lwakeNNS(Point*); // Compute NNS from wakes of left side of rectangles
	void rwakeNNS(Point*); // Compute NNS from wakes of right side of rectangles
	void uwakeNNS(Point*); // Compute NNS from wakes of upper side of rectangles
	void dwakeNNS(Point*); // Compute NNS from wakes of lower side of rectangles
	Rect* getleftray(double, double); // Compute rectangle hit first by left ray from query with two double coordinates.
	Rect* getrightray(double, double); // Compute rectangle hit first by right ray from query with two double coordinates.
	Rect* getupray(double, double); // Compute rectangle hit first by up ray from query with two double coordinates.
	Rect* getdownray(double, double); // Compute rectangle hit first by down ray from query with two double coordinates.
	bool ishit(double, double, double); // simple method for detect hitting
	bool isclosest(double, double, double);  // simple method for computing closest edge
	double threemin(double, double, double); // return minimum value among three parameters
	bool intersect(Rect*, Rect*); // return true when two rectangles intersect
	bool intersect(Rect*, Point*); // return true when a point is contained in rectangle
public:
	RectangularDomain(); // Constructs a rectangular domain with a large bounding box.
	RectangularDomain(vector<Rect*>, vector<Point*>); // Constructs a rectangular domain with a large bounding box with rectangles and points
	~RectangularDomain(); // Destructor
	Point* NNS(Point*); // Finds the nearest neighbor pointer among data points for given query.
	Point* FNS(Point*); // Finds the farthest neighbor pointer among data points for given query.
	vector<Point*> kNNS(Point*,int); // Finds the k-nearest neighbor pointer among data points for given query.
	int insertion(vector<Rect*>, vector<Point*>); // Inserts obstacles and data to the rectangular domain
	int insertion(vector<Rect*>); // Inserts obstacles to the rectangular domain
	int insertion(vector<Point*>); // Inserts data to the rectangular domain
	int deletion(vector<Rect*>, vector<Point*>); // Deletes obstacles and data from the rectangular domain
	int deletion(vector<Rect*>); // Deletes obstacles rom the rectangular domain
	int deletion(vector<Point*>); // Deletes data from the rectangular domain
	int getdatacnt(); // Returns the number of obstacles.
	int getobscnt(); // Returns the number of data.
	vector<Rect*> getobstacles(); // Returns vector of pointers of all obstacles.
	vector<Point*> getdata(); // Returns vector of pointers of all data.
	Rect* getobstacle(int); // Returns i-th pointer of obstacles.
	Point* getdatum(int); // Returns i-th pointer of data.
	Rect* getbbox(); // Returns the pointer of bounding box.
};
