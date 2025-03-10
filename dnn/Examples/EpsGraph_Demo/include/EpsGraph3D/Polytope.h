#pragma once
#include "framework.h"
#include "Point.h"


namespace EPS {
	class Edge {
	public:
		Point* p1;
		Point* p2;
		Point p3;
		Point p4;
		double length;
	public:
		Edge();
		Edge(std::vector<Point*>);
		Edge(Point*, Point*);
		Edge(Grid_Point*, Grid_Point*);
		Edge(Free_Point*, Grid_Point*);
		Edge(Point, Point);
		Edge(Grid_Point, Grid_Point);
		Edge(Free_Point, Grid_Point);
		~Edge();
		bool operator==(Edge);
		bool on(Point* p);
		bool cross(Point* p, Point* ray);
	};

	class Face {
	protected:
		char* face_key;
		std::vector<Point*> points;

	public:
		Face();
		Face(std::vector<Point*>);
		~Face();
		bool on(Point* p, int mode);
		bool cross(Point* p, Point* ray);
		bool pass(Point* p1, Point* p2, int dir);
		std::vector<Point*> getpoints();
	};

	class Polytope {
	protected:
		std::vector<Face*> faces;
		std::vector <Edge> edges;
		std::vector<Point*> vertices;
		int num_faces;
		int num_points;
		int num_edges;
		double min_length;
	public:
		double x_min, x_max, y_min, y_max, z_min, z_max;
		std::vector<Point*> encl_pts;
		int ord;

	public:
		Polytope();
		Polytope(FILE*);
		~Polytope();
		void setpolytope(std::vector<Face*> input);
		bool isIn(Point* p);
		bool intersect(Point, Point, int);
		bool operator==(Polytope);
		std::vector<Face*> getfaces();
	};
}