#pragma once

#include <vector>
#include <string>
#include <utility>


class SPSP {
public:
public:
	SPSP();

	// 1. input polygon
	void addVertex(int x, int y);
	void deleteVertex();
	void printInputPolygon(std::string fileName);

	// 2. Query
	int readPolygon(std::string fileName);
	void preprocessing();
	void addQueryPoint(double x, double y);
	void clearTestPoint();
	void printQuery(std::string fileName);

	double getPathLength();

	std::vector<std::pair<double, double>> getPolygonPoints();
	std::vector<std::pair<double, double>> getQueryPoints();
	std::vector<std::pair<double, double>> getPathPoints();
	std::vector<std::pair<double, double>> getStringPoints();
};