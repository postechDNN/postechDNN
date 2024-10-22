#include "../pch.h"
#include "L1NNS_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"

using namespace std;
#define DIV 1024

L1NNS_DS::L1NNS_DS() {
	memory.dwLength = sizeof(memory);
	exe_time = 0;

	this->knn = 1;
	this->queryPoint = nullptr;
	this->lastInput[0] = 0.0;
	this->lastInput[1] = 0.0;
	this->obsTempS = nullptr;
	this->obsTempT = nullptr;
}

L1NNS_DS::~L1NNS_DS() {

}

long long int L1NNS_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int L1NNS_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int L1NNS_DS::get_execution_time()
{
	return exe_time;
}

void L1NNS_DS::setKNN(int k) {
	this->knn = k;
}

void L1NNS_DS::setMode(int m) {
	this->mode = m;
}

int L1NNS_DS::getKNN() {
	return this->knn;
}

/*
void L1NNS_DS::makeRectangularDomain() {
	
}
*/
/*
bool L1NNS_DS::preprocessing() {
	
}
*/
void L1NNS_DS::query() {
	std::vector<std::pair<Point*, double>> qr = this->rd->kNNS(this->queryPoint, this->knn);
	for (auto q : qr) {
		Point temp(q.first->getx(), q.first->gety());
		this->queryResult.push_back(std::make_pair(temp,q.second));
	}
}

void L1NNS_DS::clearRectangularDomain() {
	if (this->rd != nullptr) {
		delete this->rd;
		this->rd = nullptr;
	}
}

void L1NNS_DS::clearAll() {
	this->clearRectangularDomain();
	this->clearQuery();
	for (auto obs : this->obstacles) {
		obs.clear();
	}
	this->obstacles.clear();
	this->clearObstacleInputTemp();
	this->inputPoints.clear();

	this->rd = new RectangularDomain();
}

void L1NNS_DS::clearQuery() {
	this->queryResult.clear();
}

void L1NNS_DS::clearObstacleInputTemp() {
	if (this->obsTempS != nullptr) {
		delete this->obsTempS;
		this->obsTempS = nullptr;
	}
	if (this->obsTempS != nullptr) {
		delete this->obsTempS;
		this->obsTempS = nullptr;
	}
}

bool L1NNS_DS::insertObstacleVertex(double x, double y) {
	this->lastInput[0] = x;
	this->lastInput[1] = y;
	if (this->obsTempS == nullptr) {
		this->obsTempS = new Point(x, y);
	}
	else {
		this->obsTempT = new Point(x, y);
		return this->makeObstacle();
	}
}

bool L1NNS_DS::insertInputPoint(double x, double y) {
	this->lastInput[0] = x;
	this->lastInput[1] = y;

	std::vector<Point*> temp;
	temp.push_back(new Point(x, y));
	if (this->rd->insertion(temp)) {
		this->inputPoints.push_back(Point(x, y));
		return 1;
	}
	else {
		return 0;
	}
}
bool L1NNS_DS::insertQueryPoint(double x, double y) {
	this->lastInput[0] = x;
	this->lastInput[1] = y;

	this->deleteQueryPoint();
	this->queryPoint = new Point(x, y);	
	this->query();
	return 1;
}

void L1NNS_DS::deleteObstacle() {
	if (this->obsTempS != nullptr) {
		delete this->obsTempS;
		this->obsTempS = nullptr;
	}
	else {
		if (!this->obstacles.empty()) {
			std::vector<Rect*> temp;
			temp.push_back(new Rect(this->obstacles.back()[0], this->obstacles.back()[1],
				this->obstacles.back()[2], this->obstacles.back()[3]));
			this->rd->deletion(temp);
			this->obstacles.back().clear();
			this->obstacles.pop_back();
		}
	}
}

void L1NNS_DS::deleteInputPoint() {
	if (!this->inputPoints.empty()) {
		std::vector<Point*> temp;
		temp.push_back(new Point(this->inputPoints.back().getx(), this->inputPoints.back().gety()));
		this->rd->deletion(temp);
		this->inputPoints.pop_back();
	}
}
void L1NNS_DS::deleteQueryPoint() {
	if (this->queryPoint != nullptr) {
		delete this->queryPoint;
		this->queryPoint = nullptr;
		this->clearQuery();
	}
}

bool L1NNS_DS::makeObstacle() {
	double minX = min(this->obsTempS->getx(), this->obsTempT->getx());
	double maxX = max(this->obsTempS->getx(), this->obsTempT->getx());
	double minY = min(this->obsTempS->gety(), this->obsTempT->gety());
	double maxY = max(this->obsTempS->gety(), this->obsTempT->gety());

	delete this->obsTempS;
	delete this->obsTempT;
	this->obsTempS = nullptr;
	this->obsTempT = nullptr;

	std::vector<Rect*> temp;
	temp.push_back(new Rect(minX, maxX, minY, maxY));
	if (this->rd->insertion(temp)) {
		std::vector<double> obs = { minX,maxX,minY,maxY };
		this->obstacles.push_back(obs);
		return 1;
	}
	else {
		return 0;
	}
}

int L1NNS_DS::getObstacleSize() {
	return this->obstacles.size();
}

std::pair<double, double> L1NNS_DS::getObstacleVertex(int i, int j) {
	switch (j) {
	case 0: // ld
		return std::make_pair(this->obstacles[i][0], this->obstacles[i][2]);
		break;
	case 1: // rd
		return std::make_pair(this->obstacles[i][1], this->obstacles[i][2]);
		break;
	case 2: // ru
		return std::make_pair(this->obstacles[i][1], this->obstacles[i][3]);
		break;
	case 3: // lu
		return std::make_pair(this->obstacles[i][0], this->obstacles[i][3]);
		break;
	default:
		break;
	}
}

int L1NNS_DS::getInputPointsSize() {
	return this->inputPoints.size();
}
std::pair<double, double> L1NNS_DS::getInputPoint(int i) {
	return std::make_pair(this->inputPoints[i].getx(), this->inputPoints[i].gety());
}
bool L1NNS_DS::isQueryInserted() {
	if (this->queryPoint != nullptr) {
		return true;
	}
	else {
		return false;
	}
}
std::pair<double, double> L1NNS_DS::getQueryPoint() {
	return std::make_pair(this->queryPoint->getx(), this->queryPoint->gety());
}

bool L1NNS_DS::isObsTempInserted() {
	if (this->obsTempS != nullptr) {
		return true;
	}
	else {
		return false;
	}
}
std::pair<double, double> L1NNS_DS::getObsTemp() {
	return std::make_pair(this->obsTempS->getx(), this->obsTempS->gety());
}


int L1NNS_DS::getQueryResultSize() {
	return this->queryResult.size();
}
std::pair<double, double> L1NNS_DS::getQueryResultCoord(int i) {
	return std::make_pair(this->queryResult[i].first.getx(), this->queryResult[i].first.gety());
}
double L1NNS_DS::getQueryResultDist(int i) {
	return this->queryResult[i].second;
}

bool L1NNS_DS::readInputData(std::string fileName) {
	std::ifstream readFile(fileName);
	if (readFile.is_open()) {
		int vnum = 2;
		int onum;
		readFile >> onum;
		for (int i = 0; i < onum; i++) {
			for (int i = 0; i < vnum; i++) { // ld ru
				double x, y;
				readFile >> x >> y;
				this->insertObstacleVertex(x, y);
			}
		}
		readFile >> vnum;
		for (int i = 0; i < vnum; i++) {
			double x, y;
			readFile >> x >> y;
			this->insertInputPoint(x, y);
		}
		readFile.close();
		return 1;
	}
	else {
		return 0;
	}
}

bool L1NNS_DS::printInputData(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->obstacles.size() << '\n';
		for (auto obs : this->obstacles) {
			file << obs[0] << ' ' << obs[2] << '\n'; // ld ru
			file << obs[1] << ' ' << obs[3] << '\n';
		}
		file << this->inputPoints.size() << '\n';
		for (auto q : this->inputPoints) {
			file << q.getx() << ' ' << q.gety() << '\n';
		}
		file.close();
		return 1;
	}
	else {
		return 0;
	}
}

bool L1NNS_DS::printQueryResult(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << this->queryResult.size() << '\n';
		for (auto q : this->queryResult) {
			file << q.first.getx() << ' ' << q.first.gety()
				<< ' ' << q.second << '\n';
		}
		file.close();
		return 1;
	}
	else {
		return 0;
	}
}