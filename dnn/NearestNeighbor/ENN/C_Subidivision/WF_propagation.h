#pragma once

#include "DCEL/DCEL.h"
#include "CS_Free.h"
#include "APX_wavefront.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <functional>
#include <algorithm>

class IOEdgesContainers {
public:
    HEdge* e;  // Pointer to a half-edge associated with this container
    std::vector<HEdge*> hedges;  // A vector of half-edges, likely input or output edges
    int in_out; // Indicates whether the edges are input (0) or output (1)

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

// Represents the time at which a wavefront covers a particular edge
class CoverTime{
public:
    double t;  // The time when the edge is covered by the wavefront
    HEdge *e;  // Pointer to the associated half-edge

    // Constructor to initialize the CoverTime with a half-edge and an initial time (default is max double value)
    CoverTime(HEdge* e, float t = std::numeric_limits<double>::max()): e(e), t(t){}
    
    // Update the cover time to the minimum of the current and the new time
    void update(double new_t) {
        t = std::min(t, new_t);
    }

    // Comparison operator to allow ordering in a priority queue (for the wavefront propagation process)
    bool operator>(const CoverTime& other) const {
        return t > other.t;
    }

    ~CoverTime(){} // Destructor
};

class WF_generator {
public:
    Vertex* src;  // Source vertex from which the wavefront is generated
    HEdge* hedge; // each marked generator is in the approximate wavefront of one of the boundary edges of c(cell)
    double weight; // distance from src to hedge 
    // TODO: Implement additional functionalities or properties for the wavefront generator
};

class WF_propagation{
private:
    Vertex* src;  // Source vertex for the wavefront propagation
    CS_Free& cs_free;  // Reference to the conforming subdivision of free space
        
        
    // Priority queue to process edges based on their cover times
    std::priority_queue<CoverTime, std::vector<CoverTime>, std::greater<CoverTime>> covertime_pq;    
    
    
    // Maps to store the shortest distance to vertices and to keep track of which cells (faces) have been influenced by which wavefront generators
    std::unordered_map<Vertex*, double> dist_vertices;  // Distance map for vertices (used in the wavefront propagation)
    std::unordered_map<Face*, std::vector<WF_generator> > marked_cells; 
    std::unordered_map<HEdge*, CoverTime> covertime_of_edges;
public:
    // Constructor initializing the wavefront propagation with a reference to the configuration space
    WF_propagation(Vertex* s, CS_Free& cs);
    
    // Destructor
    ~WF_propagation();  

    // Method to start the wavefront propagation process from the source vertex
    void propagate();

    // Compute approximate wavefronts at a given half-edge based on the incoming wavefronts
    std::vector<APX_wavefront> compute_apx_wavefront(HEdge* e, std::vector<APX_wavefront>& wavefronts);

    // Update the cover time for a specific edge as the wavefront propagates
    void update_covertime_of_edge(HEdge *e, double t);

    // Retrieve the cover time for a specific edge
    CoverTime get_covertime_of_edge(HEdge *e);

    // Get the approximate wavefront associated with a particular edge
    std::vector<APX_wavefront> get_apx_wavefront_of_edge(HEdge *e);

    // Set (store) the approximate wavefront associated with a particular edge
    void set_apx_wavefront_of_edge(HEdge *e);

    // CCW algorithm
    int ccw(int x1, int y1, int x2, int y2, int x3, int y3);

    // Compute the point that has same weighted distance between three point
    Point compute_weighted_dist_point(int idx1, int idx2, int idx3, HEdge *e);

    // Compute the distance from the source to the endpoints of an edge using the current wavefronts
    void compute_dist_to_endpoints(HEdge *, std::vector<APX_wavefront>&);

    // Compute the time it would take for the wavefront to reach and engulf the endpoint of an edge
    double compute_endpoint_engulf_time(HEdge*, std::vector<APX_wavefront>&);

    // Mark a wavefront generator as having influenced a particular cell (face) in the domain
    void mark_generator_to_cell(Face *f, WF_generator generator);

    // Compute the input edges (half-edges) at a particular edge, which will be used to determine how the wavefront enters this region
    IOEdgesContainers compute_input_e(HEdge *e);

    // Compute the output edges (half-edges) at a particular edge, which will be used to determine how the wavefront exits this region
    IOEdgesContainers compute_output_e(HEdge *e);


    std::vector<WF_generator> getMarked_cells(Face*);
};

