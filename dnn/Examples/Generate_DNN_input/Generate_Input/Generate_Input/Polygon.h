#pragma once
#include "Point.h"
#include "Edge.h"
#include <iostream>
#include <vector>
#include <algorithm>

//Simple Polygon without holes =>Ring
class SimplePolygon {
private:
	int n_vertices;
	std::vector<Point> vertices; //clockwise order
	std::vector<Edge> edges;
public:

	SimplePolygon(std::vector<Point>& v,bool parallel = true);
	SimplePolygon(std::vector<Edge>& v);
	~SimplePolygon();
	int inPolygon(Point p);
	std::vector<Point> getVertices();
	std::vector<Edge> getEdges();
	//void draw_line(cv::Mat& img, cv::Scalar line_color = cv::Scalar(255, 0, 0));
	//void draw_fill(cv::Mat& img, cv::Scalar line_color = cv::Scalar(255, 255, 255));
	};