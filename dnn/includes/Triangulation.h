#pragma once
#include "SimpleBBST.h"
#include "DCEL/DCEL.h"
#include <map>

class TriangleSubdivision{
	private :
		DCEL * origin;
		DCEL * subdivision;
		std::map<Face *,Face *> matching;
	public :
		TriangleSubdivision(DCEL *);
		~TriangleSubdivision();
		DCEL * getSubdivision();
		Face * originFace(Face *);
		
};


