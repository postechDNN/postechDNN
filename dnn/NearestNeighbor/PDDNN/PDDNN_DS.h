#pragma once
#include "Object.h"
#include <vector>

#include "./Util/PDgraph.h"

class PDDNN_DS
{
// Function
public:
	PDDNN_DS();
	~PDDNN_DS();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

// Data structure pointers 
public:
	Object object2D;

	void setKNN(int k);
	int getKNN();

	void makePolygonalDomain();
	bool preprocessing();

	void query();
	void clearPolygonalDomain();
	void clearAll();
	void clearQuery();
	void clearPDGraph();

	bool insertPolygonVertex(double x, double y);
	bool insertObstacleVertex(double x, double y);
	bool insertInputPoint(double x, double y);
	bool dynamicInsertInputPoint(double x, double y);
	bool insertQueryPoint(double x, double y);

	void deletePolygonVertex();
	void deleteObstacleVertex();
	void deleteInputPoint();
	void deleteQueryPoint();

	bool makeObstacle();

	int getPolygonSize();
	std::pair<double, double> getPolygonVertex(int i);
	int getObstaclesNum();
	int getObstacleSize(int i);
	std::pair<double, double> getObstacleVertex(int i, int j);
	int getInputPointsSize();
	std::pair<double, double> getInputPoint(int i);
	bool isQueryInserted();
	std::pair<double, double> getQueryPoint();

	int getObstacleTempSize();
	std::pair<double, double> getObstacleTempVertex(int i);

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
	Point* queryPoint;
	PDgraph* pdgraph;
	PolygonalDomain* pd;

	std::vector<Point> polygonVertices;
	std::vector<std::vector<Point>> obstacles;
	std::vector<Point> obstacleTemp;
	std::vector<Point> inputPoints;

	std::vector<std::pair<Point, double>> queryResult;
};

