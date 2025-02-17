#pragma once
#include "Object.h"
#include <string>
#include <vector>
#include <utility>
#include <list>

#include "./Util/DD.h"

class DDNN_DS
{
// Function
public:
	DDNN_DS();
	~DDNN_DS();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

// Data structure pointers 
public:
	Object object2D;

	int insertConvexDistPoint(double x, double y);
	void deleteConvexDistPoint();
	int insertPoint(double x, double y);
	void deletePoint();
	int inputQueryPoint(double x, double y);
	void deleteQueryPoint();

	void clearData();
	void clearConvexDistData();
	void preprocessingConvexDist();
	void clearQueryData();
	void preprocessing();
	

	int getConvexDistPointsSize();
	std::pair<double, double> getconvexDistPoint(int i);

	int getConvexDistResultSize();
	std::pair<double, double> getconvexDistResultPoint(int i);

	int getInputPointsSize();
	std::pair<double, double> getInputPoint(int i);

	int getQueryResultSize();
	std::pair<double, double> getQueryResultNeighbor(int i);
	double getQueryResultDist(int i);

	void setKNN(int k);
	//void setEpsilon(double eps);
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
	//double epsilon;
	
	std::vector<ConvexDistPoint> convexDistPoints;
	std::vector<ConvexDistPoint> convexDistResult;

	std::vector<ConvexDistPoint> boundingBox;

	std::vector<ConvexDistPoint> InputPoints;
	ConvexDistPoint* queryPoint;
	ConvexDistance* myd;
	std::vector<std::pair<ConvexDistPoint, double>> queryResult;
};

