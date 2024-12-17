#pragma once
#include "Space.h"
#include "utils.h"
#include "AVLTreeList.h"
#include "Polygon.h"
#include "pch.h"

#include "Object.h"
#include <vector>

class ENN_DS
{
	// Function
public:
	ENN_DS();
	~ENN_DS();
	void add_fr(double[2]);
	void add_fr(CString path);
	void add_poly(CString path);
	void del_fr(int);
	void del_poly(int);
	void set_knn(double[2], int);
	std::vector<OGL_Vertex> get_fr(CString path);
	std::vector<OGL_Edge> do_knn(CString path);
	std::vector<OGL_Face> get_pol(CString path);
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

	void readENN(CString path);

	// Data structure pointers 
public:
	Space* space;
	Object object2D;
	Object object3D;

private:
	std::vector<SimplePolygon> store_add_pol;
	std::vector<Point> store_add_fr;
	std::vector<int> del_pol_key;
	std::vector<int> del_fr_key;
	Point query;
	int knn;
	MEMORYSTATUSEX memory;
	long long int exe_time;
};

