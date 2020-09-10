
#include <iostream>
#include "Point.h"
#include <vector>

class Edge {
protected:
	Point s, t;
public:
	Edge();
	Edge(Point s, Point t);
	~Edge();
	Point getOrigin() const;
	Point getDest() const;
	Point getDir();

	double turn_direction(Point op);
	int intersect(Edge& op, double* i_x = NULL, double* i_y = NULL);
	bool contain(Point p);
	//void draw(cv::Mat& img, cv::Scalar line_color = cv::Scalar(255, 0, 0));
	bool operator <(Edge& operand);
	bool operator ==(Edge& operand);
	friend std::ostream& operator<< (std::ostream& os, const Edge& op);
};