#include "pch.h"
#include "DNN_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"

using namespace std;
#define DIV 1024

DNN_DS::DNN_DS() {
	this->Graph = new EPS::Eps_Graph_3D();
	memory.dwLength = sizeof(memory);
	exe_time = 0;
}

DNN_DS::~DNN_DS() {

}

void DNN_DS::add_fr(double coor[3])
{
	EPS::Free_Point* temp = new EPS::Free_Point(coor[0], coor[1], coor[2]);
	this->Graph->add_freepts(temp);
}

void DNN_DS::add_fr(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	vector<EPS::Free_Point> temp = {};
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		EPS::Free_Point p(x,y,z);
		temp.push_back(p);
	}
	this->Graph->add_freepts(temp);
}

void DNN_DS::add_poly(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	EPS::Polytope temp;
	vector<EPS::Face*> t = {};
	for (int i = 0; i < fn; i++) {
		vector<EPS::Point*> face;
		for (int i = 0; i < 3; i++) {
			double x, y, z;
			file >> x >> y >> z;
			EPS::Point* p = new EPS::Point(x, y, z);
			face.push_back(p);
		}
		EPS::Face* f = new EPS::Face(face);
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
	EPS::Free_Point temp(coor[0], coor[1], coor[2]);
	Eps_Query=temp;
}

vector<OGL_Vertex> DNN_DS::get_fr()
{
	list<EPS::Free_Point> temp = {};
	vector <OGL_Vertex> convert = {};
	temp = Graph->get_free_points();

	for (auto v : temp) {
		OGL_Vertex tv;
		tv.setPos(v.x, v.y, v.z);
		convert.push_back(tv);
	}

	return convert;
}

vector<OGL_Edge> DNN_DS::do_knn()
{
	vector<EPS::Edge> temp = {};
	vector <OGL_Edge> convert = {};
	auto start = chrono::high_resolution_clock::now();
	temp = Graph->get_path(Eps_Query, Eps_knn);
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	exe_time = duration.count();

	for (auto e : temp) {
		OGL_Edge te;
		OGL_Point a1, a2;
		a1 = { e.p1->x, e.p1->y, e.p1->z };
		a2 = { e.p2->x, e.p2->y, e.p2->z };
		te.setStartP(a1);
		te.setEndP(a2);
		convert.push_back(te);
	}
	return convert;
}

vector<OGL_Face> DNN_DS::get_pol()
{
	vector<EPS::Polytope> temp = {};
	vector <OGL_Face> convert = {};
	temp = Graph->get_Polytope();

	for (auto pol : temp) {
		vector<EPS::Face*> tf = pol.getfaces();
		for (auto f : tf) {
			OGL_Face addf;
			vector<EPS::Point*> tp = f->getpoints();
			for (auto p : tp) {
				OGL_Point addp = { p->x, p->y, p->z };
				addf.addPoint(addp);
			}
			convert.push_back(addf);
		}
	}

	return convert;
}

void DNN_DS::read3Deps(CString path)
{
	std::ifstream file(path);
	int fn, pn;
	file >> fn >> pn;
	vector<EPS::Free_Point> fr_temp = {};
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		EPS::Free_Point p(x, y, z);
		fr_temp.push_back(p);
	}
	this->Graph->add_freepts(fr_temp);

	for (int j = 0; j < pn; j++) {
		EPS::Polytope temp;
		file >> fn;
		vector<EPS::Face*> t = {};
		for (int i = 0; i < fn; i++) {
			vector<EPS::Point*> face;
			for (int i = 0; i < 3; i++) {
				double x, y, z;
				file >> x >> y >> z;
				EPS::Point* p = new EPS::Point(x, y, z);
				face.push_back(p);
			}
			EPS::Face* f = new EPS::Face(face);
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

long long int DNN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int DNN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int DNN_DS::get_execution_time()
{
	return exe_time;
}