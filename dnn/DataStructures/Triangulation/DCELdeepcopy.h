#pragma once
#include "../DCEL/DCEL.h"
#include <map>

class DCELFaceMap{
	public : 
		DCEL * newDCEL;
		std::map<Face *, Face *> map_result;
		DCELFaceMap(DCEL * t){
			newDCEL=t;
		}
		~DCELFaceMap();
};

DCELFaceMap * DCELdeepcopy(DCEL * o);




