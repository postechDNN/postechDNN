#pragma once
#include <vector>
#include <string>
#include <utility>

class DNN_Util {
public:
	int knn;
	bool polygonInputDone;
	std::vector<std::pair<double, double>> points_set;
	std::vector<std::vector<std::pair<double, double>>> Strings;
public:
	DNN_Util();
	~DNN_Util();

	void clearData();

	// 1. input polygon and points
	void addVertexPolygon(int x, int y);
	void deleteVertexPolygon();
	int addPoint(int x, int y); // 0: Add Failed, 1: Add Success
	void deletePoint();
	void printInputData(std::string fileName);

	// 2. Query
	int readInputData(std::string fileName); // 0: Read Failed, 1: Read Success
	void preprocessing();
	int addQueryPoint(double x, double y); // 0: Add Failed, 1: Add Success
	void deleteQueryPoint();
	void printQuery(std::string fileName);

	double getPathLength();
	double getPathLength(std::vector<std::pair<double, double>> path_points);

	std::vector<std::pair<double, double>> getPolygonVertices();
	std::pair<double, double> getPolygonVerticesBack();
	int getNumPolygonVertices();
	std::vector<std::pair<double, double>> getInputPoints();
	std::pair<double, double> getInputPointsBack();
	int getNumInputPoints();
	std::vector<std::pair<double, double>> getQueryPoint();
	int getNumQueryPoint();

	std::vector<std::pair<double, double>> getPathPoints();
	std::vector<std::pair<double, double>> getStringPoints();

	std::vector<std::pair<double, int>> getQueryResult();
};