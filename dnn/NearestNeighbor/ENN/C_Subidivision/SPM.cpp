#include "SPM.h"

#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <string>
#include <utility>
#include <algorithm>

using namespace std;

constexpr double TOLERANCE_SPM = 1e-6;

double distSegToPoint(Edge e, Point p) {
	// Horizontal
	if (std::abs(e.gets().gety() - e.gett().gety()) < TOLERANCE_SPM) {
		Point* left;
		Point* right;
		if (e.gets().getx() > e.gett().getx()) {
			right = &e.gets();
			left = &e.gett();
		}
		else {
			left = &e.gets();
			right = &e.gett();
		}

		if (left->getx() > p.getx()) {
			return left->distance(p);
		}
		else if (bottom->getx() < p.getx()) {
			return right->distance(p);
		}
		else {
			return std::abs(left->gety() - p.gety());
		}
	}

	// Vertical
	else if (std::abs(e.gets().getx() - e.gett().getx()) < TOLERANCE_SPM) {
		Point* top;
		Point* bottom;
		if (e.gets().gety() > e.gett().gety()) {
			top = &e.gets();
			bottom = &e.gett();
		}
		else {
			bottom = &e.gets();
			top = &e.gett();
		}

		if (top->gety() < p.gety()) {
			return top->distance(p);
		}
		else if (bottom->gety() > p.gety()) {
			return bottom->distance(p);
		}
		else {
			return std::abs(top->getx() - p.getx());
		}
	}

	else {
		return;
	}
}

double distGenToPoint(WF_generator gen, Point p) {
	return distSegToPoint(gen.hedge->getEdge(),p) + gen.weight;
}

bool cmpX(PlaneSweepEvent a, PlaneSweepEvent b) {
	return a.getP().getx() < b.getP().getx();
}

std::vector<std::vector<HEdge*>> compute_active_cells(std::vector<HEdge*> edges, std::vector<WF_generator> generators) {
	std::vector<std::vector<HEdge*>> result;
	return result;
};

bool SPMEdge::operator<(const SPMEdge& cmpE) const {
	return distGenToPoint(this->v, this->hyperbola.gets()) < distGenToPoint(cmpE.getV(), cmpE.getHyperbola().gets());
}

SPM::SPM(Vertex* s, WF_propagation& wfp, std::vector<SimplePolygon> obs) : src(src), wfp(wfp), obs(obs) {
	this->ComputingSPM();
}
SPM::~SPM() {
}

void SPM::ComputingSPM() {
	this->ComputeVertices();
	
	this->CombinedVertices();
}

void SPM::ComputeVertices() {
	std::vector<Face*> cells = this->wfp.cs_free.getDCEL()->getFaces();
	for (auto nowCell : cells) {
		// break cell into active and inactive regions
		std::vector<HEdge*> edges = nowCell->getInnerHEdges();
		std::vector<WF_generator> generators = this->wfp.getMarked_cells();
		// Stores marked edges of each active region
		std::vector<std::vecter<HEdge*>> active_regions = compute_active_cells(edges,generators); // transparent edge
		
		for (auto nowRegion : active_regions) {
			// Compute S-Faces for each marked transparent edge
			std::vector<std::vector<SFaces>> SFaces_results;
			for (auto nowGen : nowRegion) {
				std::vector<SFaces> temp_result;
				IOEdgesContainers input_edges = this->wfp.compute_input_e(nowGen.hedge);
				for (auto nowInputEdge : input_edges.hedges) {
					SFaces temp(nowInputEdge);
					temp_result.push_back(temp);
				}
				SFaces_results.push_back(temp_result);
			}


			// Compute the vertices of SPM (divide and conquer)


			// Collect vertices
		}
	}
}

void SPM::CombinedVertices() {
	// 1. Standard plane sweep for computing arcs
	// Take the endpoints belonging to the bisector of a generator pair (v, w)
	std::map<std::pair<string, string>, int> generatorPairIdx;
	std::vector<std::vector<SPMEdge>> classifiedEdges;

	// Sort edges along the hyperbola determined by the weighted generators v and w.
	for (size_t i = 0; i < classifiedEdges.size(); i++) {
		for (size_t j = 0; j < classifiedEdges[i].size(); j++) {
			if (distGenToPoint(classifiedEdges[i][j].getV(), classifiedEdges[i][j].getHyperbola().gets()) >
				distGenToPoint(classifiedEdges[i][j].getV(), classifiedEdges[i][j].getHyperbola().gett())) {
				classifiedEdges[i][j].flip();
			}
		}
		sort(classifiedEdges[i].begin(), classifiedEdges[i].end());
	}
	std::vector<SPMEdge> arcs;
	for (size_t i = 0; i < classifiedEdges.size(); i++) {
		Hyperbola hyperbola(classifiedEdges[i].front().getHyperbola().gets(), classifiedEdges[i].back().getHyperbola().gett(),
			classifiedEdges[i].front().getHyperbola().geta(), classifiedEdges[i].front().getHyperbola().getb());
		SPMEdge temp(hyperbola, classifiedEdges[i].getV(), classifiedEdges[i].getW());
		arcs.push_back(temp);
	}


	// 2. Standard plane sweep for combining arcs and obstacle edges
	std::priority_queue<PlaneSweepEvent> pq;
	// Initialize prioirty queue 1 (push Hyperbola arcs)
	int idx = 0;
	for (size_t i = 0; i < classifiedEdges.size(); i++) {
		PlaneSweepEvent tempS;
		PlaneSweepEvent tempT;
		tempS.setP(classifiedEdges[i].getHyperbola().gets());
		tempS.setHyperbolaArc(&classifiedEdges[i].getHyperbola());
		tempS.setType(true);
		tempS.setIdx(idx);
		tempT.setP(classifiedEdges[i].getHyperbola().gett());
		tempT.setType(true);
		tempT.setHyperbolaArc(&classifiedEdges[i].getHyperbola());
		tempT.setIdx(idx);
		pq.push(tempS);
		pq.push(tempT);
		idx++;
	}

	// Initialize prioirty queue 2 (push obstacles' edges)
	for (size_t i = 0; i < this->obs.size(); i++) {
		for (size_t j = 0; j < this->obs[i].size(); j++) {
			PlaneSweepEvent tempS;
			PlaneSweepEvent tempT;
			tempS.setP(obs[i][j].gets());
			tempS.setLineSegment(&obs[i][j]);
			tempS.setType(false);
			tempS.setIdx(idx);
			tempT.setP(obs[i][j].gett());
			tempT.setType(false);
			tempT.setLineSegment(&obs[i][j]);
			tempT.setIdx(idx);
			pq.push(tempS);
			pq.push(tempT);
			idx++;
		}
	}

	// plane sweep
	std::map<int, PlaneSweepEvent*> 
	while (!pq.empty()) {
		PlaneSweepEvent nowEvent = pq.top();
		pq.pop();
	}

	// Convert to DCEL
}





