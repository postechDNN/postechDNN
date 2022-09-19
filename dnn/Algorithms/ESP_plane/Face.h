#pragma once
#include <vector>
#include "Edge.h"

	class Face {
	public:
		// vector<Face*> incident_faces;
		std::vector<Edge*> incident_edges;
	};