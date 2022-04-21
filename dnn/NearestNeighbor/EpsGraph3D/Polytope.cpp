#include "Polytope.h"
#include <queue>
#include <assert.h>



using namespace std;

// Face

bool Face::below(Point* p) {
	// Determine if the face is below from p along z-axis.

	// Check the face contains intersection of z-line contining the point and the plain expanded by the face

	// Exception
}


// Polytope
bool Polytope::isIn(Point* p) {
	int num_belowface = 0; 
	for (int i = 0; i < num_faces; i++)
	{
		if (this->faces[i]->below(p)) {
			num_belowface++;
		}
	}
	if (num_belowface % 2 == 1) {
		return true;
	}
	else { return false; }
};