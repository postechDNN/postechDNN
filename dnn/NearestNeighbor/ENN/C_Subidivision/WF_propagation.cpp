#include "WF_propagation.h"
#include "WC_region.h"


WF_propagation::WF_propagation(CS_Free& cs) : cs_free(cs){
}

WF_propagation::~WF_propagation(){
    ;
}

IOEdgesContainers WF_propagation::compute_input_e(HEdge *e){
    // TODO
    WC_region_free wc_region(this->cs_free,e);
    // Use WC_region.h
}
IOEdgesContainers WF_propagation::compute_output_e(HEdge *e){
    // TODO
    WC_region_free wc_region(this->cs_free,e);
    // Use WC_region.h
}

std::vector<APX_wavefront> WF_propagation::compute_apx_wavefront(HEdge* e, std::vector<APX_wavefront>& wavefronts){
    // TODO
    // marking rule for generators (Rule 2, 3, 4)
}

void WF_propagation::update_covertime_of_edge(HEdge *e, double t){
    // TODO
}


CoverTime WF_propagation::get_covertime_of_edge(HEdge *e){
    // TODO
}


std::vector<APX_wavefront> WF_propagation::get_apx_wavefront_of_edge(HEdge *e){
    // TODO
}
void WF_propagation::set_apx_wavefront_of_edge(HEdge *e){
    // TODO
}
void WF_propagation::compute_dist_to_endpoints(HEdge *, std::vector<APX_wavefront>&){
    // TODO
}
double WF_propagation::compute_endpoint_engulf_time(HEdge*, std::vector<APX_wavefront>&){
    // TODO
}

void WF_propagation::mark_generator_to_cell(Face *f, WF_generator generator){
    this->marked_cells[f].push_back(generator);
}

void WF_propagation::propagate(){    
    while (!this->covertime_pq.empty()) {
        CoverTime covertime_e = this->covertime_pq.top();
        //Handle a false alarm of covertime
        if (std::abs(this->get_covertime_of_edge(covertime_e.e).t - covertime_e.t ) < 1e-6){
            this->covertime_pq.pop();
            continue;
        }

        HEdge* e = covertime_e.e;
        double t = covertime_e.t;


        /*  TODO
            marking rule for generators (Rule 1)
            let v be an endpoint of e
            If v is a generator, mark v in the faces incident to v
        */ 

        IOEdgesContainers input_edges = this->compute_input_e(e);
        IOEdgesContainers output_edges = this->compute_output_e(e);
        
        std::vector<APX_wavefront> apx_wavefronts;
        for(auto f : input_edges){
            // Compute the approximate wavefronts at e based on the approximate wavefronts from all edges f in input(e)
            // satisfying covertime(f) < covertime(e).
            CoverTime covertime_f = this->get_covertime_of_edge(f);
            if (covertime_f.t > covertime_e.t) continue;
            
            std::vector<APX_wavefront> apx_wavefront_of_f = this->get_apx_wavefront_of_edge(f);
            apx_wavefronts.insert(apx_wavefronts.end(), apx_wavefront_of_f.begin(), apx_wavefront_of_f.end());
        }
        std::vector<APX_wavefront> apx_wavefront_of_e =  this->compute_apx_wavefront(e, apx_wavefronts);
        // Compute d(v,s) exactly for each endpoint v of e
        this->compute_dist_to_endpoints(e, apx_wavefront_of_e);

        // For each edge g in output(e), compute the time t_g
        // when the approximate waverfront from e first engulfs an endpoint of g.
        for(auto g : output_edges){
            double t_g = this->compute_endpoint_engulf_time(g,apx_wavefront_of_e);
            
            // Set covertime(g) = min(covertime(g) + t_g + |g|)
            CoverTime covertime_g = this->get_covertime_of_edge(g);
            double len_g = g->getEdge().length();
            this->update_covertime_of_edge(g, t_g + covertime_g.t + len_g);
        }
        this->covertime_pq.pop();
    }
}
