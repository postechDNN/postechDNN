#include "pch.h"
#include "ENN_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"

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
		Point p(x,y);
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
	query=temp;
	knn = knn;
}

vector<OGL_Vertex> ENN_DS::get_fr(CString path)
{
	vector <OGL_Vertex> convert = {};
	std::ifstream file(path);
	int n_points, n_polygons;
	file >> n_points >> n_polygons;
	vector<Point> points_temp = {};
	for (int i = 0; i < n_points; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		OGL_Vertex tv;
		tv.setPos(x, y);
		convert.push_back(tv);
		points_temp.push_back(p);
	}

	return convert;
}

vector<OGL_Edge> ENN_DS::do_knn(CString path)
{
	pair<Point, Point> temp = {};
	vector <OGL_Edge> convert = {};
	auto start = chrono::high_resolution_clock::now();
	
	std::ifstream file(path);
	int n_points, n_polygons;
	file >> n_points >> n_polygons;
	vector<Point> points_temp = {};
	for (int i = 0; i < n_points; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		points_temp.push_back(p);
	}
	vector<SimplePolygon> poly_temp = {};
	for (int j = 0; j < n_polygons; j++) {
		// EPS::Polytope temp;
		file >> n_points;
		for (int i = 0; i < n_points; i++) {
			vector<Point> vertices;
			for (int k = 0; k < 2; k++) {
				double x, y;
				file >> x >> y;
				Point* p = new Point(x, y);
				vertices.push_back(*p);
			}
			SimplePolygon* f = new SimplePolygon(vertices);
			poly_temp.push_back(*f);
		}
	}

	for (auto poly : poly_temp) {
		SimplePolygon tf = poly;
		OGL_Face addf;
		for (auto p : tf.getVertices()) {
			OGL_Point addp = { p.getx(), p.gety() };
			addf.addPoint(addp);
		}
	}

	Space s(points_temp, poly_temp);
	pair<Point, double>answer = s.query(this->query);
	temp = { answer.first, this->query };
		
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	exe_time = duration.count();

	//for (auto e : temp) {
	OGL_Edge te;
	OGL_Point a1, a2;
	a1 = { temp.first.getx(), temp.first.gety()};
	a2 = { temp.second.getx(), temp.second.gety()};
	te.setStartP(a1);
	te.setEndP(a2);
	convert.push_back(te);
	//}
	return convert;
}

vector<OGL_Face> ENN_DS::get_pol(CString path)
{
	vector <OGL_Face> convert = {};

	object2D.total_clear();
	std::ifstream file(path);
	int n_points, n_polygons;
	file >> n_points >> n_polygons;
	vector<Point> points_temp = {};
	for (int i = 0; i < n_points; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		points_temp.push_back(p);
	}
	vector<SimplePolygon> poly_temp = {};
	for (int j = 0; j < n_polygons; j++) {
		// EPS::Polytope temp;
		file >> n_points;
		for (int i = 0; i < n_points; i++) {
			vector<Point> vertices;
			for (int k = 0; k < 2; k++) {
				double x, y;
				file >> x >> y;
				Point* p = new Point(x, y);
				vertices.push_back(*p);
			}
			SimplePolygon* f = new SimplePolygon(vertices);
			poly_temp.push_back(*f);
		}
	}

	for (auto poly : poly_temp) {
		SimplePolygon tf = poly;
		OGL_Face addf;
		for (auto p : tf.getVertices()) {
			OGL_Point addp = { p.getx(), p.gety() };
			addf.addPoint(addp);
		}
		convert.push_back(addf);
	}

	return convert;
}

void ENN_DS::readENN(CString path)
{
	object2D.total_clear();
	std::ifstream file(path);
	int n_points, n_polygons;
	file >> n_points >> n_polygons;
	vector<Point> points_temp = {};
	for (int i = 0; i < n_points; i++) {
		double x, y;
		file >> x >> y;
		Point p(x, y);
		points_temp.push_back(p);
	}

	vector<SimplePolygon> poly_temp = {};
	for (int j = 0; j < n_polygons; j++) {
		// EPS::Polytope temp;
		file >> n_points;
		for (int i = 0; i < n_points; i++) {
			vector<Point> vertices;
			for (int k = 0; k < 2; k++) {
				double x, y;
				file >> x >> y;
				Point* p = new Point(x, y);
				vertices.push_back(*p);
			}
			SimplePolygon* f = new SimplePolygon(vertices);
			poly_temp.push_back(*f);
		}
	}
	//EPS::Eps_Graph_3D grid(fr_temp, pol_temp, eps);
	//this->Graph = &grid;
	//for (auto p : store_add_pol) {
	//	Graph->add_pol(p);
	//}
	////store_add_pol.clear();
	//for (auto p : store_add_fr) {
	//	Graph->add_freepts(&p);
	//}
	////Graph->add_freepts(store_add_fr);
	////store_add_fr.clear();
	//for (auto del : del_pol_key) {
	//	Graph->delete_pol(del);
	//}
	////del_pol_key.clear();
	//for (auto del : del_fr_key) {
	//	Graph->delete_freept(del);
	//}
	////del_fr_key.clear();

	vector<OGL_Vertex> v_temp = get_fr(path);
	vector<OGL_Edge> e_temp = do_knn(path);
	vector<OGL_Face> f_temp = get_pol(path);

	for (auto v : v_temp) {
		object2D.addVertex(v);
	}
	for (auto e : e_temp) {
		object2D.addPath(e);
	}
	for (auto f : f_temp) {
		for (int i = 0; i < 2; i++) {
			OGL_Edge temp;
			temp.isCustom = true;
			temp.color[0] = 0.5f;
			temp.color[1] = 0.5f;
			temp.color[2] = 0.5f;
			temp.width = 2.0f;
			temp.setStartP(f.getPoint(i % 3));
			temp.setEndP(f.getPoint((i+1) % 3));
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