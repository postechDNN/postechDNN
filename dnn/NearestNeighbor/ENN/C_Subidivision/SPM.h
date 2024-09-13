#pragma once

#include <vector>
#include "WF_propagation.h"
//#include "DCEL/DCEL.h"

// Shortest path map
class SPM{ 
//variables
private:
	Vertex* src;
	WF_propagation& wfp;


//functions
public:
	SPM(Vertex* s, WF_propagation& wfp);
	~SPM();

	void ComputingSPM(); // main function for computing SPM
	void ComputeVertices(); // compute the vertices of SPM inside each cell
	void CombinedVertices(); // Plane sweep to assemble SPM edges
};


