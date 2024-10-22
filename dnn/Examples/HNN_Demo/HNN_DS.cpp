#include "pch.h"
#include "HNN_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"

using namespace std;
#define DIV 1024

HNN_DS::HNN_DS() {
	this->memory.dwLength = sizeof(memory);
	this->exe_time = 0;

	this->knn = 1;
	this->epsilon = 0.05;
	this->queryPoint = nullptr;
}

HNN_DS::~HNN_DS() {

}

long long int HNN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int HNN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int HNN_DS::get_execution_time()
{
	return exe_time;
}

int HNN_DS::insertPoint(double x, double y) {
	this->InputPoints.push_back(HNN_Point(x, y));
	return 1;
}

void HNN_DS::deletePoint() {
	if (!this->InputPoints.empty()) {
		this->InputPoints.pop_back();
	}
}

int HNN_DS::inputQueryPoint(double x, double y) {
	this->deleteQueryPoint();
	this->queryPoint = new HNN_Point(x, y);
	this->preprocessing();
	return 1;
}

void HNN_DS::deleteQueryPoint() {
	if (this->queryPoint != nullptr) {
		delete this->queryPoint;
	}
	if (this->myd != nullptr) {
		delete this->myd;
	}
	this->queryResult.clear();
}

void HNN_DS::clearData() {
	if (this->queryPoint != nullptr) {
		delete this->queryPoint;
	}
	if (this->myd != nullptr) {
		delete this->myd;
	}
	this->InputPoints.clear();
	this->queryResult.clear();
}

void HNN_DS::preprocessing() {
	int size = this->InputPoints.size();
	HNN_Point* points = new HNN_Point[size];
	for (int i = 0; i < size; i++) {
		points[i].coords[hnn::X] = InputPoints[i].coords[hnn::X];
		points[i].coords[hnn::Y] = InputPoints[i].coords[hnn::Y];
	}
	this->myd = new CompressedQuadtree<HNN_Point>(hnn::DIM, points, size);

	if (this->isQueryPointInserted()) {
		std::list<std::pair<HNN_Point*, double>> queryList = this->myd->knn(this->knn, *this->queryPoint, this->epsilon);
		this->queryResult.reserve(queryList.size());
		for (auto node : queryList) {
			queryResult.push_back(std::make_pair(node.first, node.second));
		}
	}
}

int HNN_DS::getInputPointsSize() {
	return this->InputPoints.size();
}

std::pair<double, double> HNN_DS::getInputPoint(int i) {
	return make_pair(this->InputPoints[i].coords[hnn::X], this->InputPoints[i].coords[hnn::Y]);
}


int HNN_DS::getQueryResultSize() {
	return this->queryResult.size();
}

std::pair<double, double> HNN_DS::getQueryResultNeighbor(int i) {
	return std::make_pair(this->queryResult[i].first->coords[hnn::X]
		, this->queryResult[i].first->coords[hnn::Y]);
}

double HNN_DS::getQueryResultDist(int i) {
	return this->queryResult[i].second;
}

void HNN_DS::setKNN(int k) {
	this->knn = k;
}

void HNN_DS::setEpsilon(double eps) {
	this->epsilon = eps;
}

int HNN_DS::getKNN() {
	return this->knn;
}

std::pair<double, double> HNN_DS::getQueryPoint() {
	return std::make_pair(this->queryPoint->coords[hnn::X],
		this->queryPoint->coords[hnn::Y]);
}

bool HNN_DS::isQueryPointInserted() {
	if (this->queryPoint == nullptr) return false;
	else return true;
}

bool HNN_DS::readInputData(std::string fileName) {
	std::ifstream readFile(fileName);
	if (readFile.is_open()) {
		int num;
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

bool HNN_DS::printInputData(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->InputPoints.size() << '\n';
		for (auto q : this->InputPoints) {
			file << q.coords[hnn::X] << ' ' << q.coords[hnn::Y] << '\n';
		}
		return 1;
	}
	else {
		return 0;
	}
}

bool HNN_DS::printQueryResult(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->queryResult.size() << '\n';
		for (auto q : this->queryResult) {
			file << q.first->coords[hnn::X] << ' ' << q.first->coords[hnn::Y]
				<< ' ' << q.second << '\n';
		}
		return 1;
	}
	else {
		return 0;
	}
}