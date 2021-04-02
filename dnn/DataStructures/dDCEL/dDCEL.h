#pragma once
#include"dPoint.h"
#include"dEdge.h"
#include"dFace.h"
#include<vector>

class dDCEL{
private:
	std::vector<dPoint*>* points;
	std::vector<dEdge*>* edges;
	std::vector<dFace*>* faces;
	std::vector<dFace*>* facets;
public:
	dDCEL();
	~dDCEL();

}