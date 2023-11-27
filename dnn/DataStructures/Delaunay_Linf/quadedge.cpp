#include "quadedge.h"
//#include "fakeirisgl.h"

#include <GLUT/glut.h>
#include <vector>

#include "gnuplot-iostream.h"

using namespace std; 


Point2d drA, drB; // for drawing 

// Function to initialize the drivers
void myInit(void)
{
    // Clear all the screen color
    glClearColor(1.0, 0.5, 0.0, 1.0);
 
    // Sets background color to orang
    glMatrixMode(GL_PROJECTION);
 
    glLoadIdentity();
 
    // Specify the display area
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}

void myDisplay(void)
{
    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
 
    glPointSize(4.0);
 
    // Rectangular part of hut
    glColor3f(0.5f, 0.5f, 0.5f);
 
    // Begin the polygon
    glBegin(GL_POLYGON);
 
    // Create the polygon
    glVertex2i(40, 40);
    glVertex2i(320, 40);
 
    glVertex2i(40, 200);
    glVertex2i(320, 200);
 
    glVertex2i(40, 200);
    glVertex2i(40, 40);
 
    glVertex2i(320, 200);
    glVertex2i(320, 40);
 
    glEnd();
 
 
    // Sends all output to display
    glFlush();
}

void bgnline(const Point2d& a, const Point2d& b){
	printf("Draw_bgnline\n");



}
void endline(){

	printf("Draw\n");
}
void v2d(double*){

	printf("Draw\n");
}

bool LowerThan(Point2d a, Point2d b){
  return ((a.y<b.y) || (a.y==b.y) && (a.x<b.x));
}

bool LessThan(Point2d a, Point2d b){
  return ((a.x < b.x) || ((a.x==b.x) && (a.y < b.y)));
}

bool LeftThan(Point2d a, Point2d b){
  return (a.x < b.x);
}

/*********************** Basic Topological Operators ************************/

Edge* MakeEdge()
{
	QuadEdge *ql = new QuadEdge;
	return ql->e;
}

void Splice(Edge* a, Edge* b)
// This operator affects the two edge rings around the origins of a and b,
// and, independently, the two edge rings around the left faces of a and b.
// In each case, (i) if the two rings are distinct, Splice will combine
// them into one; (ii) if the two are the same ring, Splice will break it
// into two separate pieces.
// Thus, Splice can be used both to attach the two edges together, and
// to break them apart. See Guibas and Stolfi (1985) p.96 for more details
// and illustrations.
{
	Edge* alpha = a->Onext()->Rot();
	Edge* beta  = b->Onext()->Rot();

	Edge* t1 = b->Onext();
	Edge* t2 = a->Onext();
	Edge* t3 = beta->Onext();
	Edge* t4 = alpha->Onext();

	a->next = t1;
	b->next = t2;
	alpha->next = t3;
	beta->next = t4;
}

void DeleteEdge(Edge* e)
{
	Splice(e, e->Oprev());
	Splice(e->Sym(), e->Sym()->Oprev());
	delete e->Qedge();
}

/************* Topological Operations for Delaunay Diagrams *****************/

Subdivision:: Subdivision(const Point2d& a, const Point2d& b) // Mine
{ // Mine
	Point2d *da, *db;
	da = new Point2d(a), db = new Point2d(b);
	Edge* e = MakeEdge();

	// Let lower edge be a starting edge. 
	if(a.y < b.y){
		e->EndPoints(da, db);
	}
	else{
		e->EndPoints(db, da);
	}
	
	startingEdge = e;
}

Subdivision::Subdivision(const Point2d& a, const Point2d& b, const Point2d& c)
// Initialize a subdivision to the triangle defined by the points a, b, c.
{
	Point2d *da, *db, *dc;
	da = new Point2d(a), db = new Point2d(b), dc = new Point2d(c);

	vector<Point2d> pList; 
	pList.push_back(*da); 
	pList.push_back(*db);
	pList.push_back(*dc);
	sort(pList.begin(), pList.end(), LowerThan);

	Point2d *lowest, *left, *right;
	lowest = new Point2d(pList[0]);
	left = LeftThan(pList[1], pList[2]) ? new Point2d(pList[1]) : new Point2d(pList[2]);
	right = LeftThan(pList[1], pList[2]) ? new Point2d(pList[2]) : new Point2d(pList[1]);
	
	Edge* e1 = MakeEdge();
	e1->EndPoints(lowest, right);
	Edge* e2 = MakeEdge();
	Splice(e1->Sym(), e2); //CHECK
	e2->EndPoints(right, left);
	Edge* e3 = MakeEdge();
	Splice(e2->Sym(), e3); //CHECK
	e3->EndPoints(left, lowest);
	Splice(e3->Sym(), e1); //CHECK
	startingEdge = e1;
}

Subdivision::Subdivision(const Point2d& a, const Point2d& b, const Point2d& c, int t)
{//mine
	//Make edges ab(or ba) and bc(or cb) (not ac)
	Point2d *da, *db, *dc;
	da = new Point2d(a), db = new Point2d(b), dc = new Point2d(c);

	Edge* e1 = MakeEdge();
	Edge* e2 = MakeEdge();

	if (LowerThan(b, a) && LowerThan(b, c)){ //Does this happen? 
		
		e1->EndPoints(db, dc);

		Splice(e1, e2->Sym()); // CHECK
		e2->EndPoints(da, db);

		startingEdge = e1;
	}

	else{
		Point2d *lower, *upper; 
		lower = LowerThan(a, c) ? da : dc;
		upper = LowerThan(a, c) ? dc : da; 

		e1->EndPoints(lower, db);

		Splice(e1->Sym(), e2); // CHECK
		e2->EndPoints(db, upper);

		startingEdge = e1;
	}

}


Edge* Connect(Edge* a, Edge* b)
// Add a new edge e connecting the destination of a to the
// origin of b, in such a way that all three have the same
// left face after the connection is complete.
// Additionally, the data pointers of the new edge are set.
{
	Edge* e = MakeEdge();
	Splice(e, a->Lnext());
	Splice(e->Sym(), b);
	e->EndPoints(a->Dest(), b->Org());
	return e;
}

void Swap(Edge* e)
// Essentially turns edge e counterclockwise inside its enclosing
// quadrilateral. The data pointers are modified accordingly.
{
	Edge* a = e->Oprev();
	Edge* b = e->Sym()->Oprev();
	Splice(e, a);
	Splice(e->Sym(), b);
	Splice(e, a->Lnext());
	Splice(e->Sym(), b->Lnext());
	e->EndPoints(a->Dest(), b->Dest());
}

/*************** Geometric Predicates for Delaunay Diagrams *****************/

inline Real TriArea(const Point2d& a, const Point2d& b, const Point2d& c)
// Returns twice the area of the oriented triangle (a, b, c), i.e., the
// area is positive if the triangle is oriented counterclockwise.
{
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

int InCircle(const Point2d& a, const Point2d& b,
			 const Point2d& c, const Point2d& d)
// Returns TRUE if the point d is inside the circle defined by the
// points a, b, c. See Guibas and Stolfi (1985) p.107.
{
	return (a.x*a.x + a.y*a.y) * TriArea(b, c, d) -
	       (b.x*b.x + b.y*b.y) * TriArea(a, c, d) +
	       (c.x*c.x + c.y*c.y) * TriArea(a, b, d) -
	       (d.x*d.x + d.y*d.y) * TriArea(a, b, c) > 0;
}

int ccw(const Point2d& a, const Point2d& b, const Point2d& c)
// Returns TRUE if the points a, b, c are in a counterclockwise order
{
	return (TriArea(a, b, c) > 0);
}

int RightOf(const Point2d& x, Edge* e)
{
	return ccw(x, e->Dest2d(), e->Org2d());
}

int LeftOf(const Point2d& x, Edge* e)
{
	return ccw(x, e->Org2d(), e->Dest2d());
}

int OnEdge(const Point2d& x, Edge* e)
// A predicate that determines if the point x is on the edge e.
// The point is considered on if it is in the EPS-neighborhood
// of the edge.
{
	Real t1, t2, t3;
	t1 = (x - e->Org2d()).norm();
	t2 = (x - e->Dest2d()).norm();
	if (t1 < EPS || t2 < EPS)
	    return TRUE;
	t3 = (e->Org2d() - e->Dest2d()).norm();
	if (t1 > t3 || t2 > t3)
	    return FALSE;
	Line line(e->Org2d(), e->Dest2d());
	return (fabs(line.eval(x)) < EPS);
}

/************* An Incremental Algorithm for the Construction of *************/
/************************ Delaunay Diagrams *********************************/

Edge* Subdivision::Locate(const Point2d& x)
// Returns an edge e, s.t. either x is on e, or e is an edge of
// a triangle containing x. The search starts from startingEdge
// and proceeds in the general direction of x. Based on the
// pseudocode in Guibas and Stolfi (1985) p.121.
{
	Edge* e = startingEdge;

	while (TRUE) {
		if (x == e->Org2d() || x == e->Dest2d())
		    return e;
		else if (RightOf(x, e))
			 e = e->Sym();
		else if (!RightOf(x, e->Onext()))
			 e = e->Onext();
		else if (!RightOf(x, e->Dprev()))
			 e = e->Dprev();
		else
		    return e;
	}
}

void Subdivision::InsertSite(const Point2d& x)
// Inserts a new point into a subdivision representing a Delaunay
// triangulation, and fixes the affected edges so that the result
// is still a Delaunay triangulation. This is based on the
// pseudocode from Guibas and Stolfi (1985) p.120, with slight
// modifications and a bug fix.
{
	Edge* e = Locate(x);
	if ((x == e->Org2d()) || (x == e->Dest2d()))  // point is already in
	    return;
	else if (OnEdge(x, e)) {
		e = e->Oprev();
		DeleteEdge(e->Onext());
	}

	// Connect the new point to the vertices of the containing
	// triangle (or quadrilateral, if the new point fell on an
	// existing edge.)
	Edge* base = MakeEdge();
	base->EndPoints(e->Org(), new Point2d(x));
	Splice(base, e);
	startingEdge = base;
	do {
		base = Connect(e, base->Sym());
		e = base->Oprev();
	} while (e->Lnext() != startingEdge);

	// Examine suspect edges to ensure that the Delaunay condition
	// is satisfied.
	do {
		Edge* t = e->Oprev();
		if (RightOf(t->Dest2d(), e) &&
			InCircle(e->Org2d(), t->Dest2d(), e->Dest2d(), x)) {
				Swap(e);
				e = e->Oprev();
		}
		else if (e->Onext() == startingEdge)  // no more suspect edges
			return;
		else  // pop a suspect edge
		    e = e->Onext()->Lprev();
	} while (TRUE);
}

/*****************************************************************************/

//#include <gl.h>
static unsigned int timestamp = 0;

void Subdivision::Draw()
{
	if (++timestamp == 0)
		timestamp = 1;
	//startingEdge->Draw(timestamp); //CHECK
}

void DrawFun(void){
	// Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
 
    glPointSize(4.0);
 
    // Rectangular part of hut
    glColor3f(0.5f, 0.5f, 0.5f);
 
    // Begin the polygon
    glBegin(GL_LINES);
    glVertex2d(drA.x, drA.y);
    glVertex2d(drB.x, drB.y);
    glEnd();
 
 

}



void Edge::Draw(unsigned int stamp, std::vector<std::vector<std::pair<double, double>>> & segments)
// This is a recursive drawing routine that uses time stamps to
// determine if the edge has already been drawn. This is given
// here for testing purposes only: it is not efficient, and for
// large triangulations the stack might overflow. A better way
// of doing this (and other traversals of the edges) is to maintain
// a list of edges in the corresponding Subdivision object. This
// list should be updated every time an edge is created or destroyed.
{
	if (Qedge()->TimeStamp(stamp)) {
		
		// Draw the edge
		Point2d a = Org2d();
		Point2d b = Dest2d();
		//bgnline(a, b);
		// v2d((double*)&a);
		// v2d((double*)&b);
		// endline();

		drA = a; 
		drB = b; 

		printf("Edge: (%.1f, %.1f) to (%.1f, %.1f)\n", a.x, a.y, b.x, b.y);
		
  		//glutDisplayFunc(DrawFun);
		//glutDisplayFunc(myDisplay);

		
		//std::vector<std::vector<std::pair<double, double>>> all_segments, all1, all2, all3, all4;
		std::vector<std::pair<double, double>> segment;
		segment.emplace_back(a.x, a.y);
		segment.emplace_back(b.x, b.y);
		segments.push_back(segment);

		// all_segments.push_back(segment);


		//gp << "plot '-' with linespoints\n";
		// NOTE: send2d is used here, rather than send1d.  This puts a blank line between segments.
		//gp.send2d(all_segments);

		

		
		// visit neighbors
		//printf("Draw Onext \n");
		Onext()->Draw(stamp, segments);
		//printf("Draw Oprev \n");
		Oprev()->Draw(stamp, segments);
		//printf("Draw Dnext \n");
		Dnext()->Draw(stamp, segments);
		//printf("Draw Dprev \n");
		Dprev()->Draw(stamp, segments);

		// if (!all1.empty()) all_segments.insert(all_segments.end(), all1.begin(), all1.end());
		// if (!all2.empty()) all_segments.insert(all_segments.end(), all2.begin(), all2.end());
		// if (!all3.empty()) all_segments.insert(all_segments.end(), all3.begin(), all3.end());
		// if (!all4.empty()) all_segments.insert(all_segments.end(), all4.begin(), all4.end());

		//return all_segments;
		return;
		
		
		//glFlush();// Sends all output to display
	}
}

void Edge::EdgeDraw(void){
// Draw the edge
	Point2d a = Org2d();
	Point2d b = Dest2d();
	//bgnline(a, b);
	// v2d((double*)&a);
	// v2d((double*)&b);
	// endline();

	drA = a; 
	drB = b; 


	printf("Edge: (%.1f, %.1f) to (%.1f, %.1f)\n", a.x, a.y, b.x, b.y);
	return;
}
