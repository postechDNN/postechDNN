#pragma once
#include "EpsGraph3D.h"
#include "Object.h"

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
	vector<OGL_Vertex> get_fr();
	vector<OGL_Edge> do_knn();
	vector<OGL_Face> get_pol();

	void read3Deps(CString path);

// Data structure pointers 
public:
	Eps_Graph_3D *Graph;
	Object object2D;
	Object object3D;

private:
	Free_Point Eps_Query;
	int Eps_knn;


};

