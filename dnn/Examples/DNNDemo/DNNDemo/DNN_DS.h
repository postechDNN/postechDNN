#pragma once
#include "EpsGraph3D/framework.h"
#include "Object.h"
#include <vector>

class DNN_DS
{
// Function
public:
	DNN_DS();
	~DNN_DS();
	void add_fr(double[3]);
	void add_fr(CString path);
	void add_poly(CString path);
	void del_fr(int);
	void del_poly(int);
	void set_knn(double[3], int);
	std::vector<OGL_Vertex> get_fr();
	std::vector<OGL_Edge> do_knn();
	std::vector<OGL_Face> get_pol();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

	void read3Deps(CString path);

// Data structure pointers 
public:
	EPS::Eps_Graph_3D *Graph;
	Object object2D;
	Object object3D;

private:
	EPS::Free_Point Eps_Query;
	int Eps_knn;
	MEMORYSTATUSEX memory;
	long long int exe_time;
};

