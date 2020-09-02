#include "Triangulation.h"
#include "DCELdeepcopy.h"
#include <vector>
#include <map>


DCELFaceMap * SimplePolygonDecomposition(DCEL *  );
DCELFaceMap * MonotonePolygonDecomposition(DCEL *  );
DCELFaceMap * TriangleDecomposition(DCEL *  );

TriangleSubdivision::TriangleSubdivision(DCEL * o){
	origin=o;
	DCELFaceMap * nM = DCELdeepcopy(o);
	
	DCELFaceMap * nMs = TriangleDecomposition(nM->newDCEL);
    subdivision=nMs->newDCEL;
	for(auto it=nMs->map_result.begin() ; it!=nMs->map_result.end(); it++){
		matching[it->first]=nM->map_result[it->second];
	}
}


TriangleSubdivision::~TriangleSubdivision(){
	
	delete(subdivision);
}

DCEL * TriangleSubdivision::getSubdivision(){
	return subdivision;
}

Face * TriangleSubdivision::originFace(Face * f){
	return matching[f];
}



DCELFaceMap * TriangleDecomposition(DCEL *  o){
	DCELFaceMap * fm= MonotonePolygonDecomposition(o);
	return fm;
}


