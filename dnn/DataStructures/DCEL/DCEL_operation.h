#pragma once

#include "AVLTree.h"
#include <iostream>
#include <string>
#include "./..//..//Algorithms/ESP_plane/Edge.h"
#include "./..//..//Algorithms/ESP_plane/Point.h"
#include <queue>
#include <set>
#include <cmath>
#include "Graph.h"
#include <utility>
#include "DCEL.h"
#include <algorithm>
#include <map>
#include "./../../Algorithms/ESP_plane/RP.h"

#define tolerance 1e-6 //TODO: change to enum
#define __DEBUG_MODE__ 1

	class HEdgeContainer{
	public:
		int key;
		HEdge *hedge;
		static Point *sweep_p;
		HEdgeContainer();
		HEdgeContainer(int k,HEdge *hedge);
		HEdgeContainer(const HEdgeContainer& op);
		double getKey() const;
		bool operator <(const HEdgeContainer& op) const;
		bool operator==(const HEdgeContainer& op) const;
	};


	class intersectEvent{
	public:
		enum class EVENT{START =0 , END, CROSS};
		Point p;
		HEdgeContainer hec;
		EVENT ev;
		intersectEvent(const Point& p, HEdgeContainer& hec ,EVENT ev);
		bool operator<(const intersectEvent op) const;
		bool operator==(intersectEvent& op);
	};

	class FaceNode{
	public: 
		bool is_outer;
		HEdge* he;

		FaceNode(HEdge*,bool);
	};

void FindNewEvent(HEdgeContainer&, HEdgeContainer&, std::priority_queue<intersectEvent>&);
Edge align_edge(Edge e);
std::vector<Face*> ConstructFaces(std::vector<HEdge*>& hedges);

// HWI added
std::vector<HEdge*> RP2HEdges(RP* rp); // convert a RP class into HEdges class
std::vector<Vertex*> RP2Vers(RP* rp);
RP* quad2RP(i_quad*); // make DCEL with a single face (interior of an i_quad)
DCEL* makeDCEL(RP* rp); // make DCEL with a single face (interior of a rectilinear polygon rp)
DCEL* makeDCEL(std::vector<Vertex*> v);