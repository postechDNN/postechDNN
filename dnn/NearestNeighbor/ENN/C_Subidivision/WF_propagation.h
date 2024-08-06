#pragma once

#include "DCEL/DCEL.h"
#include "CS_Free.h"
#include "APX_wavefront.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <functional>

class IOEdgesContainers {
public:
    HEdge* e;
    std::vector<HEdge*> hedges;
    int in_out; // in: 0, out: 1

    // Typedefs for iterator types
    using iterator = std::vector<HEdge*>::iterator;
    using const_iterator = std::vector<HEdge*>::const_iterator;
    
    // Begin and end methods for mutable iterators
    iterator begin() {
        return hedges.begin();
    }
    
    iterator end() {
        return hedges.end();
    }
    
    // Begin and end methods for const iterators
    const_iterator begin() const {
        return hedges.begin();
    }

    const_iterator end() const {
        return hedges.end();
    }
};

class CoverTime{
public:
    double t;
    HEdge *e;

    CoverTime(HEdge* e, float t = std::numeric_limits<double>::max()): e(e), t(t){}
    void update(double new_t){
        t = std::min(t,new_t);
    }

    bool operator>(const CoverTime& other) const {
        return t > other.t;
    }

    ~CoverTime(){}
};

class WF_generator{
public:
    Vertex* src;
    // TODO
};

class WF_propagation{
private:
    Vertex* src;
    CS_Free& cs_free;
    std::priority_queue<CoverTime, std::vector<CoverTime>, std::greater<CoverTime>> covertime_pq;
    std::unordered_map<Vertex*,double> dist_vertices;
    std::unordered_map<Face*, std::vector<WF_generator> > marked_cells; 
    // std::unordered_map<HEdge*, CoverTime> covertime_of_edges;
public:
    WF_propagation(CS_Free&);
    void propagate();
    std::vector<APX_wavefront> compute_apx_wavefront(HEdge* e, std::vector<APX_wavefront>& wavefronts);
    void update_covertime_of_edge(HEdge *e, double t);
    CoverTime get_covertime_of_edge(HEdge *e);
    std::vector<APX_wavefront> get_apx_wavefront_of_edge(HEdge *e);
    void set_apx_wavefront_of_edge(HEdge *e);
    void compute_dist_to_endpoints(HEdge *, std::vector<APX_wavefront>&);
    double compute_endpoint_engulf_time(HEdge*, std::vector<APX_wavefront>&);
    void mark_generator_to_cell(Face *f, WF_generator generator);
    IOEdgesContainers compute_input_e(HEdge *e);
    IOEdgesContainers compute_output_e(HEdge *e);
    ~WF_propagation();
};

