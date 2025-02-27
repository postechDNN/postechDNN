#include "pch.h"
#include "DDNN_DS.h"

#include "../Util/DD.h"

#include <limits>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <algorithm>

using namespace std;
#define DIV 1024

DDNN_DS::DDNN_DS() {
	this->memory.dwLength = sizeof(memory);
	this->exe_time = 0;

	this->knn = 1;
	//this->epsilon = 0.05;
	this->myd = nullptr;
	this->queryPoint = nullptr;
	
	this->boundingBox.push_back(ConvexDistPoint(-std::numeric_limits<double>::lowest(), -std::numeric_limits<double>::lowest()));
	this->boundingBox.push_back(ConvexDistPoint(-std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()));
	this->boundingBox.push_back(ConvexDistPoint(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()));
	this->boundingBox.push_back(ConvexDistPoint(std::numeric_limits<double>::lowest(), -std::numeric_limits<double>::lowest()));
}

DDNN_DS::~DDNN_DS() {

}

long long int DDNN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int DDNN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int DDNN_DS::get_execution_time()
{
	return exe_time;
}

int DDNN_DS::insertConvexDistPoint(double x, double y) {
	this->convexDistPoints.push_back(ConvexDistPoint(x, y));
	this->preprocessingConvexDist();
	return 1;
}

void DDNN_DS::deleteConvexDistPoint() {
	if (!this->convexDistPoints.empty()) {
		this->convexDistPoints.pop_back();
		this->preprocessingConvexDist();
	}
}

int DDNN_DS::insertPoint(double x, double y) {
	if (this->convexDistResult.empty()) {
		return 0;
	}
	else {
		this->InputPoints.push_back(ConvexDistPoint(x, y));
		return 1;
	}
}

void DDNN_DS::deletePoint() {
	if (!this->InputPoints.empty()) {
		this->InputPoints.pop_back();
	}
}

int DDNN_DS::inputQueryPoint(double x, double y) {
	if (this->convexDistResult.empty()) {
		return 0;
	}
	else {
		this->clearQueryData();
		this->queryPoint = new ConvexDistPoint(x, y);
		this->preprocessing();
		return 1;
	}
}

void DDNN_DS::deleteQueryPoint() {
	this->clearQueryData();
}

void DDNN_DS::clearData() {
	this->clearConvexDistData();
	this->InputPoints.clear();
	this->clearQueryData();
}

void DDNN_DS::clearConvexDistData() {
	if (this->myd != nullptr) {
		delete this->myd;
		this->myd = nullptr;
	}
	
	this->convexDistPoints.clear();
	this->convexDistResult.clear();
}

void DDNN_DS::preprocessingConvexDist() {
	if (this->myd != nullptr) {
		delete this->myd;
		this->myd = nullptr;
	}
	this->convexDistResult.clear();
	std::vector<ConvexDistPoint> dp;
	for (auto p : this->boundingBox) {
		// dp.push_back(p);
	}
	for (auto p : this->convexDistPoints) {
		dp.push_back(p);
	}
	myd = new ConvexDistance(dp);
	for (int i = 0; i < myd->distPolygon.size(); i++) {
		this->convexDistResult.push_back(myd->distPolygon[i]);
	}
}

void DDNN_DS::clearQueryData() {
	if (this->queryPoint != nullptr) {
		delete this->queryPoint;
		queryPoint = nullptr;
	}
	if (!this->queryResult.empty()) {
		this->queryResult.clear();
	}
}

void DDNN_DS::preprocessing() {
	std::vector<ConvexDistPoint> temp = this->myd->distPolygon;
	
	for (int i = 0; i < this->myd->distPolygon.size(); i++) {
		this->myd->distPolygon[i] = this->myd->distPolygon[i] - *this->queryPoint;
	}
	std::sort(this->myd->distPolygon.begin(), this->myd->distPolygon.end());

	std::vector<std::pair<double, int>> distList; // distance and idx of inputPoints
	for (size_t i = 0; i < this->InputPoints.size(); i++) {
		distList.push_back(std::make_pair(this->myd->GetDist(this->InputPoints[i],*this->queryPoint), i));
	}
	std::sort(distList.begin(),distList.end());


	if (this->isQueryPointInserted()) {
		int size = min(this->knn, this->InputPoints.size());
		for (int i = 0; i < size; i++) {
			queryResult.push_back(std::make_pair(this->InputPoints[distList[i].second]
				, distList[i].first));
		}
	}

	this->myd->ResetDist(temp);
}

int DDNN_DS::getConvexDistPointsSize() {
	return this->convexDistPoints.size();
}

std::pair<double, double> DDNN_DS::getconvexDistPoint(int i) {
	return make_pair(this->convexDistPoints[i].pos[0], this->convexDistPoints[i].pos[1]);
}

int DDNN_DS::getConvexDistResultSize() {
	return this->convexDistResult.size();
}

std::pair<double, double> DDNN_DS::getconvexDistResultPoint(int i) {
	return make_pair(this->convexDistResult[i].pos[0], this->convexDistResult[i].pos[1]);
}

int DDNN_DS::getInputPointsSize() {
	return this->InputPoints.size();
}

std::pair<double, double> DDNN_DS::getInputPoint(int i) {
	return make_pair(this->InputPoints[i].pos[0], this->InputPoints[i].pos[1]);
}


int DDNN_DS::getQueryResultSize() {
	return this->queryResult.size();
}

std::pair<double, double> DDNN_DS::getQueryResultNeighbor(int i) {
	return std::make_pair(this->queryResult[i].first.pos[0]
		, this->queryResult[i].first.pos[1]);
}

double DDNN_DS::getQueryResultDist(int i) {
	return this->queryResult[i].second;
}

void DDNN_DS::setKNN(int k) {
	this->knn = k;
}

/*
void DDNN_DS::setEpsilon(double eps) {
	this->epsilon = eps;
}
*/

int DDNN_DS::getKNN() {
	return this->knn;
}

std::pair<double, double> DDNN_DS::getQueryPoint() {
	return std::make_pair(this->queryPoint->pos[0],
		this->queryPoint->pos[1]);
}

bool DDNN_DS::isQueryPointInserted() {
	if (this->queryPoint == nullptr) return false;
	else return true;
}

bool DDNN_DS::readInputData(std::string fileName) {
	std::ifstream readFile(fileName);
	if (readFile.is_open()) {
		int num;
		readFile >> num;
		for (int i = 0; i < num; i++) {
			double x, y;
			readFile >> x >> y;
			this->insertConvexDistPoint(x, y);
		}

		readFile >> num;
		for (int i = 0; i < num; i++) {
			double x, y;
			readFile >> x >> y;
			this->insertPoint(x, y);
		}
		readFile.close();
		return 1;
	}
	else {
		return 0;
	}
}

bool DDNN_DS::printInputData(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->convexDistPoints.size() << '\n';
		for (auto q : this->convexDistPoints) {
			file << q.pos[0] << ' ' << q.pos[1] << '\n';
		}
		file << this->InputPoints.size() << '\n';
		for (auto q : this->InputPoints) {
			file << q.pos[0] << ' ' << q.pos[1] << '\n';
		}
		return 1;
	}
	else {
		return 0;
	}
}

bool DDNN_DS::printQueryResult(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->queryResult.size() << '\n';
		for (auto q : this->queryResult) {
			file << q.first.pos[0] << ' ' << q.first.pos[1]
				<< ' ' << q.second << '\n';
		}
		return 1;
	}
	else {
		return 0;
	}
}