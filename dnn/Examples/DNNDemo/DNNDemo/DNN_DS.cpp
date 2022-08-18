#include "pch.h"
#include "DNN_DS.h"
#include "fstream"

DNN_DS::DNN_DS() {

}

DNN_DS::~DNN_DS() {

}

void DNN_DS::add_fr(double coor[3])
{
	Free_Point* temp = new Free_Point(coor[0], coor[1], coor[2]);
	this->Graph->add_freepts(temp);
}

void DNN_DS::add_fr(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	vector<Free_Point> temp = {};
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		Free_Point p(x,y,z);
		temp.push_back(p);
	}
	this->Graph->add_freepts(temp);
}

void DNN_DS::add_poly(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	Polytope temp;
	vector<Face*> t = {};
	for (int i = 0; i < fn; i++) {
		vector<Point*> face;
		for (int i = 0; i < 3; i++) {
			double x, y, z;
			file >> x >> y >> z;
			Point* p = new Point(x, y, z);
			face.push_back(p);
		}
		Face* f = new Face(face);
		t.push_back(f);
	}
	temp.setpolytope(t);
	this->Graph->add_pol(temp);
}

void DNN_DS::del_fr(int key)
{
	Graph->delete_freept(key);
}

void DNN_DS::del_poly(int key)
{
	Graph->delete_pol(key);
}

void DNN_DS::set_knn(double coor[3], int knn)
{
	Free_Point temp(coor[0], coor[1], coor[2]);
	Eps_Query=temp;
}

vector<OGL_Vertex> DNN_DS::get_fr()
{
	list<Free_Point> temp = {};
	vector <OGL_Vertex> convert = {};
	temp = Graph->get_free_points();

	for (auto v : temp) {

	}

	return convert;
}

vector<OGL_Edge> DNN_DS::do_knn()
{
	vector<Edge> temp = {};
	vector <OGL_Edge> convert = {};
	temp = Graph->get_path(Eps_Query, Eps_knn);

	for (auto e : temp) {

	}

	return convert;
}

vector<OGL_Face> DNN_DS::get_pol()
{
	vector<Polytope> temp = {};
	vector <OGL_Face> convert = {};
	temp = Graph->get_Polytope();

	for (auto p : temp) {
		
	}

	return convert;
}

void DNN_DS::read3Deps(CString path)
{
	std::ifstream file(path);
	int fn, pn;
	file >> fn >> pn;
	vector<Free_Point> fr_temp = {};
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		Free_Point p(x, y, z);
		fr_temp.push_back(p);
	}
	this->Graph->add_freepts(fr_temp);

	for (int j = 0; j < pn; j++) {
		Polytope temp;
		file >> fn;
		vector<Face*> t = {};
		for (int i = 0; i < fn; i++) {
			vector<Point*> face;
			for (int i = 0; i < 3; i++) {
				double x, y, z;
				file >> x >> y >> z;
				Point* p = new Point(x, y, z);
				face.push_back(p);
			}
			Face* f = new Face(face);
			t.push_back(f);
		}
		temp.setpolytope(t);
		this->Graph->add_pol(temp);
	}

	vector<OGL_Vertex> v_temp = get_fr();
	vector<OGL_Edge> e_temp = do_knn();
	vector<OGL_Face> f_temp = get_pol();

	for (auto v : v_temp) {
		object3D.addVertex(v);
	}
	for (auto e : e_temp) {
		object3D.addEdge(e);
	}
	for (auto f : f_temp) {
		object3D.addFace(f);
	}

}
