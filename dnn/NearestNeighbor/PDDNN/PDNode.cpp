#include "PDNode.h"

PDNode::PDNode() {
	this->setPoint(new Point());
	this->setSite(true);
	this->setAdj(new std::vector<PDNode*>());
	this->dist = 0;
}

PDNode::PDNode(Point* p) {
	this->setPoint(p);
	this->setSite(true);
	this->setAdj(new std::vector<PDNode*>());
	this->dist = 0;
}

PDNode::PDNode(Point* p, std::vector<PDNode*>* adj) {
	this->setPoint(p);
	this->setSite(true);
	this->setAdj(adj);
}

bool PDNode::operator==(PDNode target) {
	return this->getPoint() == target.getPoint();
}


void PDNode::printAdjlist() {
	std::cout << "[" << this->p->getx() << "," << this->p->gety() << "]: ";

	for (int i = 0; i < this->getAdj()->size(); i++) {
		std::cout << "(" << (*this->getAdj())[i]->p->getx() << "," << (*this->getAdj())[i]->p->gety() << ") ";
	}
	std::cout << std::endl;
}

void PDNode::addNode(PDNode* p) {
	this->getAdj()->push_back(p);
	p->getAdj()->push_back(this);
}

Point* PDNode::getPoint() {
	return this->p;
}

void PDNode::setPoint(Point* newPoint) {
	this->p = newPoint;
}

bool PDNode::issite() {
	return this->site;
}

void PDNode::setDist(double _d) {
	this->dist = _d;
}

double PDNode::getDist() {
	return this->dist;
}

void PDNode::setSite(bool newSite) {
	this->site = newSite;
}

std::vector<PDNode*>* PDNode::getAdj() {
	return this->adjlist;
}

void PDNode::setAdj(std::vector<PDNode*>* newAdj) {
	this->adjlist = newAdj;
}