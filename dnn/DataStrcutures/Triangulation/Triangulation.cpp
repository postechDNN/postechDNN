#include "Triangulation.h"
#include <vector>
#include <map>

TriangleSubdivision::TriangleSubdivision(DCEL * o){
	origin=o;
	subdivision=new DCEL();
	std::vector<Face*>* of= origin.getFaces();
	
}


TriangleSubdivision::~TriangleSubdivision(){
	
	delete(subdivision);
}

DCEL * TriangleSubdivision::getSubdivision(){
	return subdivision;
}

FACE * TriangleSubdivision::originFace(Face * f){
	return matching[f];
}
