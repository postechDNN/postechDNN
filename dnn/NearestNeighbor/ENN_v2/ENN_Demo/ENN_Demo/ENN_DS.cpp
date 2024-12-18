#include "pch.h"
#include "ENN_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"
#include "Polygon.h"

using namespace std;
#define DIV 1024

ENN_DS::ENN_DS() {
	Space* space;
	memory.dwLength = sizeof(memory);
	exe_time = 0;
	store_add_pol = {};
}

ENN_DS::~ENN_DS() {

}

void ENN_DS::add_fr(double coor[2])
{
	Point* temp = new Point(coor[0], coor[1]);
	store_add_fr.push_back(*temp);

	this->object2D.addVertex(OGL_Vertex(coor[0], coor[1]));
	this->object2D.updateNorm(2);
}

void ENN_DS::add_fr(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	for (int i = 0; i < fn; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		store_add_fr.push_back(p);

		this->object2D.addVertex(OGL_Vertex(x, y));
	}
	this->object2D.updateNorm(2);
}

void ENN_DS::add_poly(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	vector<SimplePolygon> t = {};
	for (int i = 0; i < fn; i++) {
		vector<Point> vertices;
		OGL_Face ogl_face;
		for (int i = 0; i < 3; i++) {
			double x, y;
			file >> x >> y;
			Point* p = new Point(x, y);
			vertices.push_back(*p);
			ogl_face.addPoint(OGL_Point(x, y));
		}
		SimplePolygon* Poly = new SimplePolygon(vertices);
		store_add_pol.push_back(*Poly);
		this->object2D.addFace(ogl_face);
	}

	this->object2D.updateNorm(2);
}

void ENN_DS::del_fr(int key)
{
	del_fr_key.push_back(key);
}

void ENN_DS::del_poly(int key)
{
	del_pol_key.push_back(key);
}

void ENN_DS::set_knn(double coor[2], int knn)
{
	Point temp(coor[0], coor[1]);
	query = temp;
	knn = knn;
}


void ENN_DS::readENN(CString path)
{
	object2D.total_clear();

	vector<OGL_Vertex> v_temp = {};
	vector<OGL_Edge> e_temp = {};
	vector<OGL_Face> f_temp = {};

	std::ifstream file(path);
	int n_points, n_polygons;
	file >> n_points >> n_polygons;
	vector<Point> points_temp = {};
	for (int i = 0; i < n_points; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		OGL_Vertex pts;
		pts.setPos(x, y);
		v_temp.push_back(pts);
		points_temp.push_back(p);
	}

	int n_vertices;
	vector<SimplePolygon> poly_temp = {};
	for (int j = 0; j < n_polygons; j++) {
		// EPS::Polytope temp;
		file >> n_vertices;
		vector<Point> vertices;
		vertices.clear();
		for (int k = 0; k < n_vertices; k++) {
			double x, y;
			file >> x >> y;
			Point p(x, y);
			vertices.push_back(p);
		}
		SimplePolygon f(vertices);
		poly_temp.push_back(f);
	}

	for (auto poly : poly_temp) {
		SimplePolygon tf = poly;
		OGL_Face addf;
		for (auto p : tf.getVertices()) {
			OGL_Point addp = { p.getx(), p.gety() };
			addf.addPoint(addp);
		}
		f_temp.push_back(addf);
	}

	double q_x, q_y;
	file >> q_x >> q_y;
	Point q(q_x, q_y);
	OGL_Vertex query;
	query.setPos(q_x, q_y);
	v_temp.push_back(query);


	Space s(points_temp, poly_temp);
	pair<Point, double>answer = s.query(q);
	pair<Point, Point> temp = {};
	temp = { answer.first, q };

	OGL_Edge te;
	OGL_Point a1, a2;
	a1.setX(temp.first.getx());
	a1.setY(temp.first.gety());
	a2.setX(temp.second.getx());
	a2.setY(temp.second.gety());
	te.setStartP(a1);
	te.setEndP(a2);
	e_temp.push_back(te);
	object2D.addEdge(te);





	for (auto v : v_temp) {
		object2D.addVertex(v);
	}
	for (auto e : e_temp) {
		object2D.addPath(e);
	}
	for (auto f : f_temp) {
		for (int i = 0; i < f.getSize(); i++) {
			OGL_Edge temp;
			temp.isCustom = true;
			temp.color[0] = 0.5f;
			temp.color[1] = 0.5f;
			temp.color[2] = 0.5f;
			temp.width = 2.0f;
			temp.setStartP(f.getPoint(i% f.getSize()));
			temp.setEndP(f.getPoint((i + 1)% f.getSize()));
			object2D.addEdge(temp);
		}
		object2D.addFace(f);
	}

}

long long int ENN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int ENN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int ENN_DS::get_execution_time()
{
	return exe_time;
}