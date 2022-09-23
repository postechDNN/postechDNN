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
	store_add_pol = {};
	store_add_fr = {};
	del_pol_key = {};
	del_fr_key = {};
}

DNN_DS::~DNN_DS() {

}

void DNN_DS::add_fr(double coor[3])
{
	EPS::Free_Point* temp = new EPS::Free_Point(coor[0], coor[1], coor[2]);
	store_add_fr.push_back(*temp);

	this->object3D.addVertex(OGL_Vertex(coor[0], coor[1], coor[2]));
	this->object3D.updateNorm(3);
}

void DNN_DS::add_fr(CString path)
{
	std::ifstream file(path);
	int fn;
	file >> fn;
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		EPS::Free_Point p(x,y,z);
		store_add_fr.push_back(p);

		this->object3D.addVertex(OGL_Vertex(x, y, z));
	}
	this->object3D.updateNorm(3);
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
		OGL_Face ogl_face;
		for (int i = 0; i < 3; i++) {
			double x, y, z;
			file >> x >> y >> z;
			EPS::Point* p = new EPS::Point(x, y, z);
			face.push_back(p);
			ogl_face.addPoint(OGL_Point(x, y, z));
		}
		EPS::Face* f = new EPS::Face(face);
		t.push_back(f);

		this->object3D.addFace(ogl_face);
	}
	temp.setpolytope(t);
	store_add_pol.push_back(temp);
	this->object3D.updateNorm(3);
}

void DNN_DS::del_fr(int key)
{
	del_fr_key.push_back(key);
}

void DNN_DS::del_poly(int key)
{
	del_pol_key.push_back(key);
}

void DNN_DS::set_knn(double coor[3], int knn)
{
	EPS::Free_Point temp(coor[0], coor[1], coor[2]);
	Eps_Query=temp;
	Eps_knn = knn;
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
	double eps;
	file >> fn >> pn >> eps;
	list<EPS::Free_Point> fr_temp = {};
	for (int i = 0; i < fn; i++) {
		double x, y, z;
		file >> x >> y >> z;
		EPS::Free_Point p(x, y, z);
		fr_temp.push_back(p);
	}

	vector<EPS::Polytope> pol_temp = {};
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
		pol_temp.push_back(temp);
	}
	EPS::Eps_Graph_3D grid(fr_temp, pol_temp, eps);
	this->Graph = &grid;
	for (auto p : store_add_pol) {
		Graph->add_pol(p);
	}
	Graph->add_freepts(store_add_fr);
	for (auto del : del_pol_key) {
		Graph->delete_pol(del);
	}
	for (auto del : del_fr_key) {
		Graph->delete_freept(del);
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
		for (int i = 0; i < 3; i++) {
			OGL_Edge temp;
			temp.isCustom = true;
			temp.color[0] = 0.5f;
			temp.color[1] = 0.5f;
			temp.color[2] = 0.5f;
			temp.width = 2.0f;
			temp.setStartP(f.getPoint(i % 3));
			temp.setEndP(f.getPoint((i+1) % 3));
			object3D.addEdge(temp);
		}
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