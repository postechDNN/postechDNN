#pragma once
#include "Object.h"
#include <vector>

#include "../Util/RectangularDomain.h"

class L1NNS_DS
{
// Function
public:
	L1NNS_DS();
	~L1NNS_DS();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

// Data structure pointers 
public:
	Object object2D;
	double lastInput[2];

	void setKNN(int k);
	void setMode(int m);
	int getKNN();

	//void makeRectangularDomain();
	//bool preprocessing();

	void query();
	void clearRectangularDomain();
	void clearAll();
	void clearQuery();
	void clearObstacleInputTemp();

	bool insertObstacleVertex(double x, double y);
	bool insertInputPoint(double x, double y);
	bool insertQueryPoint(double x, double y);

	void deleteObstacle();
	void deleteInputPoint();
	void deleteQueryPoint();

	bool makeObstacle();

	int getObstacleSize();
	std::pair<double, double> getObstacleVertex(int i, int j);
	int getInputPointsSize();
	std::pair<double, double> getInputPoint(int i);
	bool isQueryInserted();
	std::pair<double, double> getQueryPoint();

	bool isObsTempInserted();
	std::pair<double, double> getObsTemp();


	int getQueryResultSize();
	std::pair<double, double> getQueryResultCoord(int i);
	double getQueryResultDist(int i);

	bool readInputData(std::string fileName);
	bool printInputData(std::string fileName);
	bool printQueryResult(std::string fileName);

private:
	MEMORYSTATUSEX memory;
	long long int exe_time;

	int knn;
	int mode; // 1: Nearest , 2: Farthest
	Point* queryPoint;
	
	RectangularDomain* rd;

	std::vector<std::vector<double>> obstacles; // Left Right Up Down
	Point* obsTempS;
	Point* obsTempT;
	std::vector<Point> inputPoints;

	std::vector<std::pair<Point, double>> queryResult;
};

