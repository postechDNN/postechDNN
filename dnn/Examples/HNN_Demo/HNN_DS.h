#pragma once
#include "Object.h"
#include <string>
#include <vector>
#include <utility>
#include <list>

#include "./Util/compressed_quadtree.h"

typedef double type_hnn_point;

namespace hnn {
	const int X = 0;
	const int Y = 1;
	const int DIM = 2;
}

struct HNN_Point {
	type_hnn_point coords[hnn::DIM];

	HNN_Point() {
		coords[hnn::X] = 0.0;
		coords[hnn::Y] = 0.0;
	};

	HNN_Point(double x, double y) {
		coords[hnn::X] = x;
		coords[hnn::Y] = y;
	}

	HNN_Point(const HNN_Point& other) {
		coords[hnn::X] = other.coords[hnn::X];
		coords[hnn::Y] = other.coords[hnn::Y];
	}

	type_hnn_point operator[](size_t idx) const { return coords[idx]; }
	type_hnn_point& operator[](size_t idx) { return coords[idx]; }
};


class HNN_DS
{
// Function
public:
	HNN_DS();
	~HNN_DS();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

// Data structure pointers 
public:
	Object object2D;

	int insertPoint(double x, double y);
	void deletePoint();
	int inputQueryPoint(double x, double y);
	void deleteQueryPoint();

	void clearData();
	void preprocessing();

	int getInputPointsSize();
	std::pair<double, double> getInputPoint(int i);

	int getQueryResultSize();
	std::pair<double, double> getQueryResultNeighbor(int i);
	double getQueryResultDist(int i);

	void setKNN(int k);
	void setEpsilon(double eps);
	int getKNN();
	
	std::pair<double, double> getQueryPoint();
	bool isQueryPointInserted();

	bool readInputData(std::string fileName);
	bool printInputData(std::string fileName);
	bool printQueryResult(std::string fileName);

private:
	MEMORYSTATUSEX memory;
	long long int exe_time;


	int knn;
	double epsilon;
	std::vector<HNN_Point> InputPoints;
	HNN_Point* queryPoint;
	CompressedQuadtree<HNN_Point>* myd;
	std::vector<std::pair<HNN_Point*, double>> queryResult;
};

