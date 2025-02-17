#include "pch.h"
#include "PDDNN_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"

using namespace std;
#define DIV 1024

PDDNN_DS::PDDNN_DS() {
	memory.dwLength = sizeof(memory);
	exe_time = 0;
}

PDDNN_DS::~PDDNN_DS() {

}

long long int PDDNN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int PDDNN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int PDDNN_DS::get_execution_time()
{
	return exe_time;
}

void PDDNN_DS::setKNN(int k) {
	this->knn = k;
}


int PDDNN_DS::getKNN() {
	return this->knn;
}

void PDDNN_DS::makePolygonalDomain() {
	std::vector<SimplePolygon*>* obsSP = new std::vector<SimplePolygon*>();
	for (size_t i = 0; i < this->obstacles.size(); i++) {
		std::vector<Edge*>* edges = new std::vector<Edge*>();
		for (size_t j = 0; j < this->obstacles[i].size(); j++) {
			Edge* edge = new Edge(this->obstacles[i][j], this->obstacles[i][(j+1)% this->obstacles[i].size()]);
			edges->push_back(edge);
		}
		SimplePolygon* obs = new SimplePolygon(edges);
		obsSP->push_back(obs);
	}
	this->pd = new PolygonalDomain(obsSP);
}
bool PDDNN_DS::preprocessing() {
	if (this->pd == nullptr) {
		return 0;
	}
	else {
		this->clearPDGraph();
		std::vector<Point*> points;
		for (auto p : this->inputPoints) {
			points.push_back(new Point(p.getx(), p.gety()));
		}
		this->pdgraph = new PDgraph(this->pd, &points);
		return 1;
	}
}

void PDDNN_DS::query() {
	std::vector<std::pair<Point*, double>>* qr = this->pdgraph->knn(this->queryPoint, this->knn);
	int size = min(qr->size(), this->knn);
	for (int i = 0; i < size; i++) {
		this->queryResult.push_back(std::make_pair(Point((*qr)[i].first->getx(), (*qr)[i].first->gety()), (*qr)[i].second));
	}
}

void PDDNN_DS::clearPolygonalDomain() {
	for (size_t i = 0; i < this->obstacles.size(); i++) {
		this->obstacles[i].clear();
	}
	this->obstacles.clear();
}

void PDDNN_DS::clearAll() {
	// delete this->pd;
	if (this->pd != nullptr) {
		std::vector<SimplePolygon*>* obsSP = this->pd->getObstacles();
		for (size_t i = 0; i < (*obsSP).size(); i++) {
			std::vector<Edge*>* edges = (*obsSP)[i]->getEdges();
			for (size_t j = 0; j < (*edges).size(); j++) {
				delete (*edges)[j];
			}
			edges->clear();
			delete (*obsSP)[i];
		}
		obsSP->clear();
		delete this->pd;
		this->pd = nullptr;
	}

	this->inputPoints.clear();

	this->clearPolygonalDomain();
	this->deleteQueryPoint();
	this->clearQuery();
	this->clearPDGraph();
}

void PDDNN_DS::clearQuery() {
	this->queryResult.clear();
}
void PDDNN_DS::clearPDGraph() {
	if (this->pdgraph != nullptr) {
		delete this->pdgraph;
		this->pdgraph = nullptr;
	}
}

bool PDDNN_DS::insertPolygonVertex(double x, double y) {
	this->polygonVertices.push_back(Point(x, y));
	return 1;
}
bool PDDNN_DS::insertObstacleVertex(double x, double y) {
	this->obstacleTemp.push_back(Point(x, y));
	return 1;
}
bool PDDNN_DS::insertInputPoint(double x, double y) {
	this->inputPoints.push_back(Point(x, y));
	return 1;
}
bool PDDNN_DS::dynamicInsertInputPoint(double x, double y) {
	this->insertInputPoint(x, y);
	if (this->preprocessing()) {
		return 1;
	}
	else {
		this->deleteInputPoint();
		return 0;
	}
}
bool PDDNN_DS::insertQueryPoint(double x, double y) {
	this->deleteQueryPoint();
	this->queryPoint = new Point(x, y);
	this->query();
	return 1;
}

void PDDNN_DS::deletePolygonVertex() {
	if (!this->polygonVertices.empty()) {
		this->polygonVertices.pop_back();
	}
}
void PDDNN_DS::deleteObstacleVertex() {
	if (!this->obstacleTemp.empty()) {
		this->obstacleTemp.pop_back();
	}
}
void PDDNN_DS::deleteInputPoint() {
	if (!this->inputPoints.empty()) {
		this->inputPoints.pop_back();
	}
}
void PDDNN_DS::deleteQueryPoint() {
	if (this->queryPoint != nullptr) {
		delete this->queryPoint;
		this->queryPoint = nullptr;
		this->clearQuery();
	}
}

bool PDDNN_DS::makeObstacle() {
	if (this->obstacleTemp.size() > 2) {
		this->obstacles.push_back(this->obstacleTemp);
		this->obstacleTemp.clear();
	}
	else {
		return 0;
	}
}

int PDDNN_DS::getPolygonSize() {
	return this->polygonVertices.size();
}
std::pair<double, double> PDDNN_DS::getPolygonVertex(int i) {
	return std::make_pair(this->polygonVertices[i].getx(), this->polygonVertices[i].gety());
}
int PDDNN_DS::getObstaclesNum() {
	return this->obstacles.size();
}
int PDDNN_DS::getObstacleSize(int i) {
	return this->obstacles[i].size();
}
std::pair<double, double> PDDNN_DS::getObstacleVertex(int i, int j) {
	return std::make_pair(this->obstacles[i][j].getx(), this->obstacles[i][j].gety());
}
int PDDNN_DS::getInputPointsSize() {
	return this->inputPoints.size();
}
std::pair<double, double> PDDNN_DS::getInputPoint(int i) {
	return std::make_pair(this->inputPoints[i].getx(), this->inputPoints[i].gety());
}
bool PDDNN_DS::isQueryInserted() {
	if (this->queryPoint != nullptr) {
		return true;
	}
	else {
		return false;
	}
}
std::pair<double, double> PDDNN_DS::getQueryPoint() {
	return std::make_pair(this->queryPoint->getx(), this->queryPoint->gety());
}

int PDDNN_DS::getObstacleTempSize() {
	return this->obstacleTemp.size();
}
std::pair<double, double> PDDNN_DS::getObstacleTempVertex(int i) {
	return std::make_pair(this->obstacleTemp[i].getx(), this->obstacleTemp[i].gety());
}

int PDDNN_DS::getQueryResultSize() {
	return this->queryResult.size();
}
std::pair<double, double> PDDNN_DS::getQueryResultCoord(int i) {
	return std::make_pair(this->queryResult[i].first.getx(), this->queryResult[i].first.gety());
}
double PDDNN_DS::getQueryResultDist(int i) {
	return this->queryResult[i].second;
}

bool PDDNN_DS::readInputData(std::string fileName) {
	std::ifstream readFile(fileName);
	if (readFile.is_open()) {
		int vnum;
		/*
		readFile >> vnum;
		for (int i = 0; i < vnum; i++) {
			double x, y;
			readFile >> x >> y;
			this->insertPolygonVertex(x,y);
		}
		*/
		int onum;
		readFile >> onum;
		for (int i = 0; i < onum; i++) {
			readFile >> vnum;
			for (int i = 0; i < vnum; i++) {
				double x, y;
				readFile >> x >> y;
				this->insertObstacleVertex(x, y);
			}
			this->makeObstacle();
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

bool PDDNN_DS::printInputData(std::string fileName) {
	std::ofstream file(fileName);
	if (file.is_open()) {
		/*
		file << this->polygonVertices.size() << '\n';
		for (auto q : this->polygonVertices) {
			file << q.getx() << ' ' << q.gety() << '\n';
		}
		*/
		file << this->obstacles.size() << '\n';
		for (auto obs : this->obstacles) {
			file << obs.size() << '\n';
			for (auto q : obs) {
				file << q.getx() << ' ' << q.gety() << '\n';
			}
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

bool PDDNN_DS::printQueryResult(std::string fileName) {
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