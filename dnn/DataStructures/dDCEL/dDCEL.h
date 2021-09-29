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
	dDCEL() {};
	~dDCEL() {};

	void set_points(std::vector<dPoint*>* _points) { points = _points; }
	void set_edges(std::vector<dEdge*>* _edges) { edges = _edges; }
	void set_faces(std::vector<dFace*>* _faces) { faces = _faces; }
	void set_facets(std::vector<dFace*>* _facets) { facets = _facets; }

	auto get_points() { return points; }
	auto get_edges() { return edges; }
	auto get_faces() { return faces; }
	auto get_facets() { return facets; }
};