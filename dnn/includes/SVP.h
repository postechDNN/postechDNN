#pragma once

#include <vector>
#include <string>
#include <utility>


class SVP {
public:
	std::vector<std::pair<double, double>> minmaxLine;
	std::vector<std::pair<double, double>> minsumLine;
	std::vector<std::vector<std::pair<double, double>>> minmaxStrings;
	std::vector<std::vector<std::pair<double, double>>> minsumStrings;
public:
	SVP();
	~SVP();
	void clearData();

	void setMode(int m); // m:0 minmax, m:1 minsum
	int getMode();

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
	double getSourcePathLength();
	double getTargetPathLength();

	std::vector<std::pair<double, double>> getPolygonPoints();
	int getNumPolygonVertices();
	std::vector<std::pair<double, double>> getQueryPoints();
	int getNumQueryPoint();
};