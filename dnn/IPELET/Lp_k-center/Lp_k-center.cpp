// --------------------------------------------------------------------
// Ipelet for creating regular k-gons
// --------------------------------------------------------------------
#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipebase.h"
#include "ipereference.h"
#include<vector>
#include<algorithm>
#include<cstring>

using namespace ipe;
using namespace std;
// --------------------------------------------------------------------
class LpCenterIpelet : public Ipelet {
public:
	virtual int ipelibVersion() const { return IPELIB_VERSION; }
	virtual bool run(int, IpeletData* data, IpeletHelper* helper);
private:
	void swap(Vector&, Vector&);
	Angle ccw(Vector v1, Vector v2, Vector v3);
	double distance(Vector, Vector);
	double linear_time_dist(vector <Vector>, Vector);
	Vector circumcircle(Vector, Vector, Vector);
	Vector centerwithp(vector <Vector>, int);
	Vector centerwithpq(vector <Vector>, int, int);
	double threemax(double, double, double);
	double threemin(double, double, double);
	Vector One_center(vector <Vector> p);
	void Two_center(vector <Vector> p);
	double getoptrad();
	Shape draw_centercircle(int);
	int metric = 2;
	int K = 1;
	double r = 0;
	double b = 0;
	double scaler = 1000;
	vector <Vector> points;
	vector <Vector> centerlist;
};
// --------------------------------------------------------------------

Angle LpCenterIpelet::ccw(Vector v1, Vector v2, Vector v3) {
	Vector p31(v3.x - v1.x, v3.y - v1.y);
	Vector p21(v2.x - v1.x, v2.y - v1.y);
	if (p31.angle() * p21.angle() < 0)
		return p21.angle() - p31.angle();
	else return p31.angle() - p21.angle();
}

void LpCenterIpelet::swap(Vector& a, Vector& b) {
	Vector temp = a;
	a = b;
	b = temp;
}

double LpCenterIpelet::threemax(double a, double b, double c) {
	if (a > b) {
		if (a > c)
			return a;
		return c;
	}
	if (b > c)
		return b;
	return c;
}

double LpCenterIpelet::threemin(double a, double b, double c) {
	if (a < b) {
		if (a < c)
			return a;
		return c;
	}
	if (b < c)
		return b;
	return c;
}

Vector LpCenterIpelet::circumcircle(Vector p1, Vector p2, Vector p3) {
	double X;
	double Y;
	if (ccw(p1, p2, p3) == 0) {
		X = (threemax(p1.x, p2.x, p3.x) + threemin(p1.x, p2.x, p3.x)) / 2.0;
		Y = (threemax(p1.y, p2.y, p3.y) + threemin(p1.y, p2.y, p3.y)) / 2.0;
	}
	else if (p2.y == p1.y || p3.y == p2.y) {
		if (p2.y == p1.y) {
			double m23 = -(p3.x - p2.x) / (p3.y - p2.y);
			double x23 = (p2.x + p3.x) / 2;
			double y23 = (p2.y + p3.y) / 2;
			X = (p1.x + p2.x) / 2;
			Y = m23 * (X - x23) + x23;
		}
		else {
			double m12 = -(p2.x - p1.x) / (p2.y - p1.y);
			double x12 = (p1.x + p2.x) / 2;
			double y12 = (p1.y + p2.y) / 2;
			X = (p2.x + p3.x) / 2;
			Y = m12 * (X - x12) + x12;
		}
	}
	else {
		double m12 = -(p2.x - p1.x) / (p2.y - p1.y);
		double m23 = -(p3.x - p2.x) / (p3.y - p2.y);
		double x12 = (p1.x + p2.x) / 2;
		double y12 = (p1.y + p2.y) / 2;
		double x23 = (p2.x + p3.x) / 2;
		double y23 = (p2.y + p3.y) / 2;
		X = (y23 - y12 - m23 * x23 + m12 * x12) / (m12 - m23);
		Y = m12 * (X - x12) + y12;

	}
	return Vector(X, Y);
}

double LpCenterIpelet::distance(Vector a, Vector b) {
	double xdiff = fabs(a.x - b.x);
	double ydiff = fabs(a.y - b.y);
	if (metric == 0)
		return std::max(xdiff, ydiff);
	else if (metric == 1)
		return xdiff + ydiff;
	return pow(xdiff, metric) + pow(ydiff, metric);
	//return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double LpCenterIpelet::linear_time_dist(vector <Vector> p, Vector b) {
	double maxdist = 0;
	double temp;
	for (int i = 0; i < p.size(); i++) {
		temp = distance(p[i], b);
		if (maxdist < temp)
			maxdist = temp;
	}
	return maxdist;
}

double LpCenterIpelet::getoptrad() {
	double covdist = 0;
	double maxdist = 0;
	double temp;
	for (int i = 0; i < points.size(); i++) {
		covdist = distance(points[i], centerlist[0]);
		for (int k = 1; k < centerlist.size(); k++) {
			temp = distance(points[i], centerlist[k]);
			if (covdist > temp)
				covdist = temp;
		}
		if (maxdist < covdist)
			maxdist = covdist;
	}
	if (metric == 0 || metric == 1)
		return maxdist;
	else {
		double guess = pow(maxdist, 1.0 / (float)metric);
		int c1 = metric - 1;
		double c2 = (float)c1 / metric;
		double c3 = maxdist / metric;
		for (int i = 0; i < metric*10; i++)
			guess = c2*guess + c3/pow(guess,c1);
		return guess;
	}
}


Vector LpCenterIpelet::centerwithpq(vector <Vector> p, int p1_index, int p2_index) {
	Vector p1 = p[p1_index];
	Vector p2 = p[p2_index];
	int n = p2_index;
	// Randomized incremental algorithm
	Vector center((p1 + p2) * 0.5);
	double radius = distance(p1, center);
	for (int i = 0; i < n; i++) {
		if (distance(p[i], center) > radius) {
			center = circumcircle(p1, p2, p[i]);
			radius = distance(p[i], center);
		}
	}
	return center;
}

Vector LpCenterIpelet::centerwithp(vector <Vector> p, int p1_index) {
	Vector p1 = p[p1_index];
	int n = p1_index;
	//Fisher-Yates shuffle (for random permutation)
	for (int i = n - 1; i >= 0; i--) {
		int target = rand() % (i + 1);
		swap(p[i], p[target]);
	}
	// Randomized incremental algorithm
	Vector center((p[0] + p1) * 0.5);
	double radius = distance(p1, center);
	for (int i = 1; i < n; i++) {
		if (distance(p[i], center) > radius) {
			center = centerwithpq(p, p1_index, i);
			radius = distance(p[i], center);
		}
	}
	return center;
}

Vector LpCenterIpelet::One_center(vector <Vector> p) {
	int n = p.size();
	if (n == 1)
		return p[0];
	//Fisher-Yates shuffle (for random permutation)
	for (int i = n - 1; i >= 0; i--) {
		int target = rand() % (i + 1);
		swap(p[i], p[target]);
	}
	// Randomized incremental algorithm
	Vector center((p[0] + p[1]) * 0.5);
	double radius = distance(p[0], center);
	for (int i = 2; i < n; i++) {
		if (distance(p[i], center) > radius) {
			center = centerwithp(p, i);
			radius = distance(p[i], center);
		}
	}

	return center;
}

void LpCenterIpelet::Two_center(vector <Vector> p) {
	int n = p.size();
	double optimum_rad = -1;
	Vector center1, center2;
	double radius1, radius2;
	Vector optimum_center1;
	Vector optimum_center2;
	vector <Vector> p1;
	vector <Vector> p2;
	String str1;
	Reference* refcenter;
	//Divide into two groups
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j)
				continue;
			p1.clear();
			p2.clear();
			for (int k = 0; k < n; k++) {
				if (k == i || k == j)
					continue;
				if (ccw(p[i], p[j], p[k]) > 0)
					p1.push_back(p[k]);
				else
					p2.push_back(p[k]);
			}
			p1.push_back(p[i]);
			p2.push_back(p[j]);
			center1 = One_center(p1);
			radius1 = linear_time_dist(p1, center1);
			center2 = One_center(p2);
			radius2 = linear_time_dist(p2, center2);
			if (optimum_rad == -1 || optimum_rad > std::max(radius1, radius2)) {
				optimum_rad = std::max(radius1, radius2);
				optimum_center1 = center1;
				optimum_center2 = center2;
			}
			p1.pop_back();
			p2.pop_back();
			p1.push_back(p[j]);
			p2.push_back(p[i]);
			center1 = One_center(p1);
			radius1 = linear_time_dist(p1, center1);
			center2 = One_center(p2);
			radius2 = linear_time_dist(p2, center2);
			if (optimum_rad == -1 || optimum_rad > std::max(radius1, radius2)) {
				optimum_rad = std::max(radius1, radius2);
				optimum_center1 = center1;
				optimum_center2 = center2;
			}
		}
	}
	centerlist.push_back(optimum_center1);
	centerlist.push_back(optimum_center2);
}

Shape LpCenterIpelet::draw_centercircle(int i) {
	double x = centerlist[i].x;
	double y = centerlist[i].y;
	Curve* sp = new Curve;
	if (metric != 0) {
		vector<vector<Vector> > centercircle(4);

		centercircle[0].push_back(Vector(x + 0, y + r));
		centercircle[0].push_back(Vector(x + b, y + r));
		centercircle[0].push_back(Vector(x + r, y + b));
		centercircle[0].push_back(Vector(x + r, y + 0));

		centercircle[1].push_back(Vector(x + r, y + 0));
		centercircle[1].push_back(Vector(x + r, y - b));
		centercircle[1].push_back(Vector(x + b, y - r));
		centercircle[1].push_back(Vector(x + 0, y - r));

		centercircle[2].push_back(Vector(x + 0, y - r));
		centercircle[2].push_back(Vector(x - b, y - r));
		centercircle[2].push_back(Vector(x - r, y - b));
		centercircle[2].push_back(Vector(x - r, y + 0));

		centercircle[3].push_back(Vector(x - r, y + 0));
		centercircle[3].push_back(Vector(x - r, y + b));
		centercircle[3].push_back(Vector(x - b, y + r));
		centercircle[3].push_back(Vector(x + 0, y + r));

		sp->appendSpline(centercircle[0]);
		sp->appendSpline(centercircle[1]);
		sp->appendSpline(centercircle[2]);
		sp->appendSpline(centercircle[3]);
		sp->setClosed(true);
	}
	else if (metric == 0) {
		vector<Vector> centercircle;
		
		centercircle.push_back(Vector(x + r, y + r));
		centercircle.push_back(Vector(x + r, y - r));
		centercircle.push_back(Vector(x - r, y - r));
		centercircle.push_back(Vector(x - r, y + r));
		centercircle.push_back(Vector(x + r, y + r));

		for (int j = 0; j < centercircle.size() - 1; j++)
			sp->appendSegment(centercircle[j], centercircle[j + 1]);
		sp->setClosed(true);
	}
	Shape shape;
	shape.appendSubPath(sp);
	
	return shape;
}

bool LpCenterIpelet::run(int, IpeletData* data, IpeletHelper* helper)
{
	Page* page = data->iPage;
	points.clear();
	centerlist.clear();
	//int sel = page->primarySelection();
	//if (sel == -1) {
	//	helper->message("No selection");
	//	return false;
	//}
	int objectnumber = page->count();
	for (int i = 0; i < objectnumber; i++) {
		if (page->select(i) != ENotSelected) {
			if (page->object(i)->type() != Object::EReference) {
				helper->message("Inputs are only points");
				return false;
			}
			Reference* ref = page->object(i)->asReference();
			points.push_back(ref->position());
			page->setSelect(i, ENotSelected);
		}
	}
	if (points.size() < 2) {
		helper->message("Select at least two points");
		return false;
	}
	String str1;
	if (!helper->getString("Enter k (the number of centers) ", str1)) {
		helper->message("No input");
		return false;
	}
	K = Lex(str1).getInt();
	if (K <= 0 || K >= 3) {
		helper->message("Invalid k value");
		return false;
	}
	String str;
	if (!helper->getString("Enter p (Underlying metric L_p) ", str)) {
		helper->message("No input");
		return false;
	}
	metric = Lex(str).getInt();
	if (metric < 0 || metric > 100) {
		helper->message("Invalid p value");
		return false;
	}

	switch (K) {
	case 1: centerlist.push_back(One_center(points)); break;
	case 2: Two_center(points); break;
	}

	r = getoptrad();
	b = 4 * (pow(2, (metric - 1.0) / metric) - 1) / 3.0 * r;

	//Plotting
	Attribute centertype = Attribute(true, "mark/circle(sx)");
	Attribute centersize = Attribute(true, "large");
	for (int i = 0; i < K; i++) {
		Reference* refcenter = new Reference(data->iAttributes, centertype, centerlist[i]);
		data->iPage->append(EPrimarySelected, data->iLayer, refcenter);
		Shape circleshape = draw_centercircle(i);
		Path* circleobj = new Path(data->iAttributes, circleshape);
		data->iPage->append(EPrimarySelected, data->iLayer, circleobj);
	}

	helper->message("center is successfully created.");
	return true;
}
// --------------------------------------------------------------------
IPELET_DECLARE Ipelet* newIpelet()
{
	return new LpCenterIpelet;
}
// --------------------------------------------------------------------