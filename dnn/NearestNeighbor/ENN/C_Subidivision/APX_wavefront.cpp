#include "APX_wavefront.h"

bool WF_generator::operator==(WF_generator _g) {
	return this->src->getPoint() == _g.src->getPoint();
}

APX_wavefront::APX_wavefront() {

}

APX_wavefront::~APX_wavefront() {
	;
}

std::vector<WF_generator> APX_wavefront::get_generators(){
    return this->wavefront;
}

std::pair<WF_generator, WF_generator> APX_wavefront::compute_claim_range(HEdge* e, int idx){
    std::pair<WF_generator, WF_generator> ret;
    return ret;
}

std::pair<WF_generator, WF_generator> APX_wavefront::compute_claim_range(HEdge* e, WF_generator & g){
    std::pair<WF_generator, WF_generator> ret;
    return ret;
}
