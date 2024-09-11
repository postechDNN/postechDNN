#include "WF_propagation.h"
#include "WC_region.h"


WF_propagation::WF_propagation(Vertex* s, CS_Free& cs) : src(src), cs_free(cs){
    
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
    auto it = covertime_of_edges.find(e);
    if (it != covertime_of_edges.end()) {
        CoverTime &target = it->second;
        if (target.t > t) {
            target.t = t;
            CoverTime temp(e, t);
            covertime_pq.push(temp);
        }
    } else {
        // Exception
        CoverTime newCoverTime(e, t);
        covertime_of_edges.insert(std::make_pair(e, newCoverTime));
        covertime_pq.push(newCoverTime);
    }
}


CoverTime WF_propagation::get_covertime_of_edge(HEdge *e){
    // TODO
    auto it = covertime_of_edges.find(e);
    if (it != covertime_of_edges.end()) {
        return it->second;
    } else {
        // Exception
        return CoverTime(e, std::numeric_limits<double>::max());
    }
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
        // Extract the edge with the smallest cover time
        CoverTime covertime_e = this->covertime_pq.top();

        // Handle a false alarm of covertime (where the current covertime of the edge is nearly equal to the previously recorded one)
        if (std::abs(this->get_covertime_of_edge(covertime_e.e).t - covertime_e.t ) < 1e-6){
            this->covertime_pq.pop();
            continue; // Skip processing this edge if it's a false alarm
        }

        HEdge* e = covertime_e.e; // The current edge being processed
        double t = covertime_e.t; // The time at which the wavefront reaches this edge


        /*  TODO
            Implement the marking rule for generators (Rule 1)
            - Let v be an endpoint of e
            - If v is a generator, mark v in the faces incident to v
        */ 

        // Compute the input edges for the current edge e (i.e., edges that contribute to the wavefront reaching e)
        IOEdgesContainers input_edges = this->compute_input_e(e);
        // Compute the output edges for the current edge e (i.e., edges that the wavefront will propagate to from e)
        IOEdgesContainers output_edges = this->compute_output_e(e);
        
        std::vector<APX_wavefront> apx_wavefronts; // To store the approximate wavefronts contributing to edge e
        for(auto f : input_edges){
            // Compute the approximate wavefronts at e based on the approximate wavefronts from all edges f in input(e)
            // satisfying covertime(f) < covertime(e).
            CoverTime covertime_f = this->get_covertime_of_edge(f);

            // If the cover time of the input edge f is greater than the cover time of the current edge e, skip it
            if (covertime_f.t > covertime_e.t) continue;

            // Get the approximate wavefront associated with edge f
            std::vector<APX_wavefront> apx_wavefront_of_f = this->get_apx_wavefront_of_edge(f);

            // Append these wavefronts to the list of wavefronts contributing to edge e
            apx_wavefronts.insert(apx_wavefronts.end(), apx_wavefront_of_f.begin(), apx_wavefront_of_f.end());
        }

        // Compute the approximate wavefront at edge e based on the contributing wavefronts from input edges
        std::vector<APX_wavefront> apx_wavefront_of_e =  this->compute_apx_wavefront(e, apx_wavefronts);
        
        // Compute the exact distance from the source to each endpoint of edge e
        this->compute_dist_to_endpoints(e, apx_wavefront_of_e);

        // For each edge g in the output edges of e, compute the time t_g
        // when the approximate wavefront from e first engulfs an endpoint of g
        for(auto g : output_edges){
            double t_g = this->compute_endpoint_engulf_time(g,apx_wavefront_of_e);
            
            // Update the cover time of edge g with the minimum time it takes to cover g, accounting for t_g and the length of g
            CoverTime covertime_g = this->get_covertime_of_edge(g);
            double len_g = g->getEdge().length();
            this->update_covertime_of_edge(g, t_g + covertime_g.t + len_g);
        }
        
        // Remove the processed edge from the priority queue
        this->covertime_pq.pop();
    }
}
