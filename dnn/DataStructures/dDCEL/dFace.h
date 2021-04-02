#pragma once
#include "dPoint.h"
#include "dEdge.h"

class dFace {
protected:
	int d; //d-face
	dEdge* outer;
	std::vector<dEdge*>* inners;
	dFace* incidentFace; //incident Face is (d+1)Face
public:
	dFace();
	~dFace();

	//bool isOutMost();
	void setOuter(dEdge*);
	dEdge* getOuter();
	std::vector<dEdge*>* getInners();
	 void addInner(dEdge*);
	//void setInners(std::vector<dEdge*>*);
};