#pragma once
#include "dPoint.h"
#include "dEdge.h"

class dFace {
protected:
	int d; //d-face
	std::vector<dEdge*>* innerEdges;
	std::vector<dFace*>* innerFaces;
public:
	dFace(int d);
	~dFace();

	//getters
	std::vector<dEdge*>* getInnerEdges();
	std::vector<dFace*>* getInnerFaces();
	int getd();
	
	//setters
	void addInnerEdge(dEdge*);
	void addInnerFace(dFace*);
	
	//dEdge* getOuter();
	//void setOuter(dEdge*);
	//void setInners(std::vector<dEdge*>*);
};
