#include "SPM.h"

SPM::SPM(Vertex* s, WF_propagation& wfp) : src(src), wfp(wfp) {
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
		// break the interior of a cell c into active and inactive regions




	}
}
void SPM::CombinedVertices() {

}