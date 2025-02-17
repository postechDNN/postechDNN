#pragma once

#include <vector>
#include <string>
#include <utility>


class SPSP {
public:
	std::vector<std::vector<std::pair<double, double>>> Strings;
public:
	SPSP();
	~SPSP();
	void clearData();

	// 1. input polygon
	void addVertex(int x, int y);
	void deleteVertex();
	void printInputPolygon(std::string fileName);

	// 2. Query
	int readPolygon(std::string fileName);
	void preprocessing();
	int addQueryPoint(double x, double y);
	void clearTestPoint();
	void printQuery(std::string fileName);
	double getPathLength();

	std::vector<std::pair<double, double>> getPolygonPoints();
	int getNumPolygonVertices();
	std::vector<std::pair<double, double>> getQueryPoints();
	int getNumQueryPoint();
	std::vector<std::pair<double, double>> getPathPoints();
	std::vector<std::pair<double, double>> getStringPoints();
};