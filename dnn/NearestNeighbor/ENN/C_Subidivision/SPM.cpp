#include "SPM.h"

#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <string>
#include <utility>
#include <algorithm>
#include <stdlib.h>

using namespace std;

constexpr double TOLERANCE_SPM = 1e-6;

double distSegToPoint(Edge e, Point p) {
	
	// When e is horizontal
	if (std::abs(e.gets().gety() - e.gett().gety()) < TOLERANCE_SPM) {
		Point left;
		Point right;
		if (e.gets().getx() > e.gett().getx()) {
			right = e.gets();
			left = e.gett();
		}
		else {
			left = e.gets();
			right = e.gett();
		}

		if (left.getx() > p.getx()) {
			return left.distance(p);
		}
		else if (right.getx() < p.getx()) {
			return right.distance(p);
		}
		else {
			return std::abs(left.gety() - p.gety());
		}
	}

	// When e is vertical
	else if (std::abs(e.gets().getx() - e.gett().getx()) < TOLERANCE_SPM) {
		Point top;
		Point bottom;
		if (e.gets().gety() > e.gett().gety()) {
			top = e.gets();
			bottom = e.gett();
		}
		else {
			bottom = e.gets();
			top = e.gett();
		}

		if (top.gety() < p.gety()) {
			return top.distance(p);
		}
		else if (bottom.gety() > p.gety()) {
			return bottom.distance(p);
		}
		else {
			return std::abs(top.getx() - p.getx());
		}
	}

	// Another case
	else {
		return 0;
	}
}

double distGenToPoint(WF_generator gen, Point p) {
	return distSegToPoint(gen.hedge->getEdge(),p) + gen.weight;
}

Hyperbola bisectorTwoGen(WF_generator a, WF_generator b) {
	Hyperbola result;
	return result;
}

// Compute active region for each cell
std::vector<std::vector<HEdge*>> compute_active_regions(std::vector<HEdge*> edges, std::vector<WF_generator> generators) {
	std::vector<std::vector<HEdge*>> result;
	std::map<std::string, WF_generator*> marked_edges;
	std::vector<WF_generator*> sorted_marked_edges;
	for (size_t i = 0; i < generators.size(); i++) {
		marked_edges[generators[i].hedge->getKey()] = &generators[i];
	}

	// Sort generators along boundary of cell
	for (size_t i = 0; i < edges.size(); i++) {
		if (marked_edges.find(edges[i]->getKey()) != marked_edges.end()) {
			sorted_marked_edges.push_back(marked_edges[edges[i]->getKey()]);
		}
	}

	// Compute bisector of a pair of marked generator
	WF_generator* prevMarked = sorted_marked_edges.back();
	for (size_t i = 0; i < sorted_marked_edges.size(); i++) {
		Hyperbola bisector = bisectorTwoGen(*sorted_marked_edges[i], *prevMarked);
		prevMarked = sorted_marked_edges[i];
	}

	return result;
};

// Compute edges of shortest path map (divide and conquer)
std::vector<SPMEdge> compute_spm_edges(std::vector<SFaces> SFaces_results) {
	std::vector<SPMEdge> result;

	for (int i = 0; i < SFaces_results.size(); i++) {
		SFaces* nowF = SFaces_results[i];
		SFaces* nextF = SFaces_results[(i+1)% SFaces_results.size()];
		std::vector<HEdge*> nowEdges = nowF->getEdges();
		std::vector<HEdge*> nextEdges = nextF->getEdges();
		int idx1 = 0;
		int idx2 = 0;
		while (idx1 < nowEdges.size() && idx2 < nowEdges.size()) {

		}
	}

	return result;
}

bool SPMEdge::operator<(const SPMEdge& cmpE) const {
	return distGenToPoint(this->v, this->hyperbola.gets()) < distGenToPoint(cmpE.getV(), cmpE.getHyperbola().gets());
}

SPM::SPM(Vertex* s, WF_propagation& wfp, CS_Free& cs_free, std::vector<SimplePolygon>& obs) : src(src), wfp(wfp), cs_free(cs_free), obs(obs) {
	this->ComputeVertices();
	this->CombineVertices();
}
SPM::~SPM() {
}

void SPM::ComputeVertices() {
	std::vector<Face*> cells = this->cs_free.getDCEL()->getFaces();
	for (auto nowCell : cells) {
		// break cell into active and inactive regions
		std::vector<HEdge*> edges = nowCell->getInnerHEdges();
		std::vector<WF_generator> generators = this->wfp.getMarked_cells(nowCell);
		// Stores marked edges of each active region
		std::vector<std::vector<HEdge*>> active_regions = compute_active_regions(edges,generators); // transparent edge
		
		std::set<std::string> marked_edges;
		for (auto nowGen : generators) {
			marked_edges.insert(nowGen.hedge->getKey());
		}
		for (auto nowRegion : active_regions) {
			// Compute S-Faces for each marked transparent edge
			std::vector<SFaces> SFaces_results;
			for (auto nowGen : nowRegion) {
				if (marked_edges.find(nowGen->getKey()) != marked_edges.end()) {
					IOEdgesContainers input_edges = this->wfp.compute_input_e(nowGen);
					SFaces temp(nowRegion, &input_edges);
					SFaces_results.push_back(temp);
				}
			}

			// Compute the edges(vertices) of SPM (divide and conquer)
			std::vector<SPMEdge> tempEdges = compute_spm_edges(SFaces_results);

			// Collect edges(vertices)
			for (size_t i = 0; i < tempEdges.size(); i++) {
				this->edges.push_back(tempEdges[i]);
			}
		}
	}
}

void SPM::CombineVertices() {
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
	std::vector<Hyperbola> arcs;
	for (size_t i = 0; i < classifiedEdges.size(); i++) {
		Hyperbola hyperbola(classifiedEdges[i].front().getHyperbola().gets(), classifiedEdges[i].back().getHyperbola().gett(),
			classifiedEdges[i].front().getHyperbola().getTranslation(), classifiedEdges[i].front().getHyperbola().getRotation(),
			classifiedEdges[i].front().getHyperbola().geta(), classifiedEdges[i].front().getHyperbola().getb());
		//SPMEdge temp(hyperbola, classifiedEdges[i].front().getV(), classifiedEdges[i].front().getW());
		arcs.push_back(hyperbola);
	}


	// 2. Standard plane sweep for combining arcs and obstacle edges
	std::priority_queue<PlaneSweepEvent> pq;
	// Initialize prioirty queue 1 (push Hyperbola arcs)
	for (size_t i = 0; i < arcs.size(); i++) {
		HArcEdge* temp = new HArcEdge();
		temp->setHyperbolaArc(&arcs[i]);
		temp->setIsArc(true);

		PlaneSweepEvent tempS;
		PlaneSweepEvent tempT;
		tempS.setP(temp->getHyperbolaArc()->gets());
		tempS.setType(0);
		tempS.setEdge1(temp);

		tempT.setP(temp->getHyperbolaArc()->gett());
		tempT.setType(1);
		tempT.setEdge1(temp);
		
		pq.push(tempS);
		pq.push(tempT);
	}

	// Initialize prioirty queue 2 (push obstacles' edges)
	for (size_t i = 0; i < this->obs.size(); i++) {
		std::vector<Edge> edges = this->obs[i].getEdges();
		for (size_t j = 0; j < edges.size(); j++) {
			Point s;
			Point t;
			if (edges[i].gets().gety() > edges[i].gett().gety()) {
				s = edges[i].gets();
				t = edges[i].gett();
			}
			else {
				t = edges[i].gets();
				s = edges[i].gett();
			}
			Edge* tempEdge = new Edge(s,t);

			HArcEdge* temp = new HArcEdge();
			temp->setLineSegment(tempEdge);
			temp->setIsArc(false);

			PlaneSweepEvent tempS;
			PlaneSweepEvent tempT;
			tempS.setP(temp->getLineSegment()->gets());
			tempS.setType(0);
			tempS.setEdge1(temp);

			tempT.setP(temp->getLineSegment()->gett());
			tempT.setType(1);
			tempT.setEdge1(temp);

			pq.push(tempS);
			pq.push(tempT);
		}
	}

	// plane sweep
	std::vector<HArcEdge*> planeSweepResult;
	std::set<HArcEdge*> HArcEdgeList; // sorted by x-coordinate order
	while (!pq.empty()) {
		PlaneSweepEvent nowEvent = pq.top();
		pq.pop();

		int event_type = nowEvent.getType();
		// Start endpoint of an edge
		if (event_type == 0) {
			auto nowEdge = nowEvent.getEdge1();
			HArcEdgeList.insert(nowEdge);
			if (HArcEdgeList.size() > 1) {
				auto it = HArcEdgeList.find(nowEdge);
				if (nowEdge->isPositiveSlope()) {
					if (it != HArcEdgeList.begin()) {
						it--;
						Point intersection = nowEdge->intersection(**it);
						PlaneSweepEvent temp;
						temp.setP(intersection);
						temp.setEdge1(nowEdge);
						temp.setEdge2(*it);
						temp.setType(2);
						pq.push(temp);
					}
				}
				else {
					if (++it != --HArcEdgeList.end()) {
						Point intersection = nowEdge->intersection(**it);
						PlaneSweepEvent temp;
						temp.setP(intersection);
						temp.setEdge1(nowEdge);
						temp.setEdge2(*it);
						temp.setType(2);
						pq.push(temp);
					}
				}
			}
		}
		// End endpoint of an edge
		else if (event_type == 1) { 
			planeSweepResult.push_back(*nowEvent.getEdge1());
			auto it = HArcEdgeList.find(nowEvent.getEdge1());
			HArcEdgeList.erase(it);
		}
		// Intersection point of a pair of two edges
		else {
			HArcEdge* temp1 = new HArcEdge(nowEvent.getEdge1());
			HArcEdge* temp2 = new HArcEdge(nowEvent.getEdge2());
			temp1.sett(nowEvent.getP());
			temp2.sett(nowEvent.getP());
			planeSweepResult.push_back(temp1);
			planeSweepResult.push_back(temp2);
			
			auto it1 = HArcEdgeList.find(nowEvent.getEdge1());
			HArcEdgeList.erase(it1);
			auto it2 = HArcEdgeList.find(nowEvent.getEdge2());
			HArcEdgeList.erase(it2);

			temp1.sets(nowEvent.getP());
			temp2.sets(nowEvent.getP());
			HArcEdgeList.insert(nowEvent.getEdge1());
			HArcEdgeList.insert(nowEvent.getEdge2());
		}
	}

	// Convert to DCEL
	this->dcel = new SPMDCEL(planeSweepResult);
}




SPMDCEL::SPMDCEL(std::vector<HArcEdge*> planeSweepResult) {
	// Find next and prev for each HArc
	int idx = 0;
	for (auto nowEdge : planeSweepResult) {
		std::string key = "e_" + std::to_string(idx++);
		nowEdge->setKey(key);
		this->hedges[key] = nowEdge;

		std::vector<HArcEdge*> neis = nowEdge->getNeighbor();
		
		Point t = nowEdge->gett();
		std::vector<HArcEdge*> alignNeis;
		for (auto nowNei : neis) {
			Point NeiS = nowNei->gets();
			if (t == NeiS) alignNeis.push_back(nowNei);
		}
		
		double minAngle = nowEdge->angle(*alignNeis.front());
		HArcEdge* min = alignNeis.front();
		for (size_t i = 1; i < alignNeis.size(); i++) {
			double temp = nowEdge->angle(*alignNeis[i]);
			if (minAngle > temp) {
				minAngle = temp;
				min = alignNeis[i];
			}
		}

		nowEdge->setNext(min);
		min->setPrev(nowEdge);
	}
}
