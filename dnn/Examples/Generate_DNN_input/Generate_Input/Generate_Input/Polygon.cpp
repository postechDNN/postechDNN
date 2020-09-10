#include "Polygon.h"

//Construct by using vertices
SimplePolygon::SimplePolygon(std::vector<Point>& v, bool parallel) {
	n_vertices = 0;
	int v_len = v.size();

	if (!parallel) {	//Is it necessary?
		for (int i = 0; i < v_len;i++) {
			Point prev, next;
			if (i == 0) prev = v[v_len - 1]; else prev = v[i - 1];
			if (i == v_len - 1) next = v[0]; else next = v[i + 1];

			Point vec_a = v[i] - prev, vec_b = next - v[i];
			double cosine = (vec_a * vec_b) / vec_a.norm() / vec_b.norm();
			if (1 - abs(cosine) > 1e-6)
				vertices.push_back(v[i]);
		}
		n_vertices = vertices.size();
	}
	else {
		for (auto p : v) {
			vertices.push_back(p);
			n_vertices++;
		}
	}

	for (auto i = vertices.begin();i != vertices.end();i++) {
		Point p = *i;
		Point p_next;
		if (i + 1 == vertices.end())
			p_next = *(vertices.begin());
		else
			p_next = *(i + 1);
		edges.push_back(Edge(p, p_next));
	}
}

//Construct by using edges
SimplePolygon::SimplePolygon(std::vector<Edge>& v) {}

SimplePolygon::~SimplePolygon() {}

//The point p is inside polygon return 1
//The point p is on boundary return 0
//The point p is outside polygon return -1
int SimplePolygon::inPolygon(Point p) {
	//Suppose that every vertex and point are far from each other as much as epsilon
	double epsilon = 1e-6;
	double x = p.getx(), y = p.gety();
	int count = 0;
	for (int i = 0; i < edges.size(); i++) {
		Edge he = edges[i];
		Point origin = he.getOrigin(), dest = he.getDest();

		if (origin.gety() > dest.gety())	//y1 <= y2
			std::swap(origin, dest);
		double x1 = origin.getx(), y1 = origin.gety();
		double x2 = dest.getx(), y2 = dest.gety();

		//Point is on boundary line
		if (abs((y1 - y2) * (x - x1) - (y - y1) * (x1 - x2)) < 1e-6) {
			if (std::min(y1, y2) - 1e-6 < y && y < std::max(y1, y2) + 1e-6 && std::min(x1, x2) - 1e-6 < x && x < std::max(x1, x2) + 1e-6) {
				
				return 0;
			}
		}
		//y1 != y2
		if (abs(y1 - y2) > 1e-6 && y1 < y - 1e-6 && y - 1e-6 < y2) {
			double tmp = (y - y1) * (x1 - x2) / (y1 - y2) + x1;
			if (tmp > x) count++;
		}
	}
	if (count % 2) return 1;
	else return -1;
}

std::vector<Point> SimplePolygon::getVertices() {
	return this->vertices;
}

std::vector<Edge> SimplePolygon::getEdges() {
	return this->edges;
}

/*
void SimplePolygon::draw_line(cv::Mat& img, cv::Scalar line_color) {
	for (auto it = this->edges.begin();it != this->edges.end();it++) {
		Edge he = *it;
		he.draw(img, line_color);
	}
}

void SimplePolygon::draw_fill(cv::Mat& img, cv::Scalar fill_color) {
	cv::Point* pts[1];
	pts[0] = new cv::Point[n_vertices];
	int i = 0;
	for (auto p : vertices) {
		pts[0][i] = cv::Point(p.getx(), p.gety());
		i++;
	}
	const cv::Point* ppt[1] = { pts[0] };
	int npts[] = { n_vertices };

	cv::fillPoly(img, ppt, npts,1, fill_color);
}*/