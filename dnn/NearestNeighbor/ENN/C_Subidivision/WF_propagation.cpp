#include "WF_propagation.h"
#include "WC_region.h"
#include "Hyperbola.h"


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

 std::vector<std::vector<APX_wavefront> > split_APX_wavefront(std::vector<APX_wavefront>& wavefronts){
    //TODO
 }


void mark(Point* v, Face * face){

}

Point compute_bisector_and_edge_intersection(Point a, Point b, HEdge* e){

}
std::vector<APX_wavefront> WF_propagation::compute_apx_wavefront(HEdge* e, std::vector<APX_wavefront>& wavefronts){
    // TODO
    // marking rule for generators (Rule 2, 3, 4)


    APX_wavefront left, right;
    APX_wavefront res = wavefronts[0]; 

    for (auto g:wavefronts ){
        
        // Decide which one is left 
        bool repeat = true; 

        while(repeat){
            std::pair<Point, Point> temp_a = res.compute_claim_range(e, 0);
            std::pair<Point, Point> temp_b = g.compute_claim_range(e, g.get_generators().size()-1);

            if (temp_a.first.getx() - temp_b.first.getx() < 0){
                left = res;
                right = g;
            }  
            else{
                left = g;
                right = res;
            }

            // Compute the two points to compare
            Point a = *(left.get_generators().end());
            std::pair<Point, Point> a_claim = left.compute_claim_range(e, a);
            Point p_a = a_claim.second;

            Point b = *(right.get_generators().begin());
            std::pair<Point, Point> b_claim = right.compute_claim_range(e, b);
            Point p_b = b_claim.first;

            Point x = compute_bisector_and_edge_intersection(a, b, e); 

            // [Marking Rule 3(b)] Mark v in both the cells that have e as an edge if v participates in a bisector event detected during the computaiton of W(e, f)
            Point* p_list[2] = {&a, &b};
            Face* f_list[2] = {e->getIncidentFace(), e->getTwin()->getIncidentFace()};
            for (auto temp_p : p_list){
                for (auto temp_f: f_list){
                    mark(temp_p, temp_f);
                }

            }

            // If x is left to p_a, then delete a from W(left, e)
            if (x.getx() < p_a.getx()){
                std::remove(left.get_generators().begin(), left.get_generators().end(), a);
            }
            // If x is right to p_b, then delete b from W(right, e)
            else if (x.getx() > p_b.getx()){
                std::remove(right.get_generators().begin(), right.get_generators().end(), b);
            }
            else{
                repeat = false;
                res.get_generators().insert(res.get_generators().begin(), left.get_generators().begin(), left.get_generators().end()); 
                res.get_generators().insert(res.get_generators().end(), right.get_generators().begin(), right.get_generators().end());
            }

        }
        

        // [Marking Rule 2] If v claims an endpoint e in W(e), mark v in all cells incident to the claimed endpoint.
        
        std::pair<Point, Point> claim_range = res.compute_claim_range(e, 0);
        Point p = claim_range.first;

        if (p == (e->getOrigin()->getPoint()) | p == (e-> getTwin() ->getOrigin()->getPoint())){
            Face* f_list[2] = {e->getIncidentFace(), e->getTwin()->getIncidentFace()};
            for (auto temp_f: f_list){
                mark(&p, temp_f); // Mark
            }

        }
        claim_range = res.compute_claim_range(e, res.get_generators().size()-1);
        Point q = claim_range.second;

        if (q == (e->getOrigin()->getPoint()) | q == (e-> getTwin() ->getOrigin()->getPoint())){
            Face* f_list[2] = {e->getIncidentFace(), e->getTwin()->getIncidentFace()};
            for (auto temp_f: f_list){
                mark(&q, temp_f); // Mark
            }

        }

        // [Marking Rule 4] If v claims part of an opaque edge when it is propagated from an edge e toward output(e), mark v in both cells with e on their boundary. 
        for(Point p: res.get_generators()){
            Face* f_list[2] = {e->getIncidentFace(), e->getTwin()->getIncidentFace()};
            for (auto temp_f: f_list){
                mark(&p, temp_f); // Mark
            }

        }






        
    }



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

int WF_propagation::ccw(int x1, int y1, int x2, int y2, int x3, int y3){
    int cross_product = (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1);

    if (cross_product > 0){
        return 1;
    }
    else if (cross_product < 0){
        return -1;
    }
    else{
        return 0;
    }
}

Point WF_propagation::compute_weighted_dist_point(int idx1, int idx2, int idx3, HEdge *e){
    // TODO
    // idx1, idx2, idx3로 각 vertex 가져오고(CS_Free, subdiv), p1, p2, p3으로 Point 할당
    
    DCEL* dcel = cs_free.getDCEL();
    std::vector<Vertex*> verlist = dcel->getVertices();

    Vertex* v1 = verlist[idx1];
    Vertex* v2 = verlist[idx2];
    Vertex* v3 = verlist[idx3];

    Point p1 = v1->getPoint();
    Point p2 = v2->getPoint();
    Point p3 = v3->getPoint();

    // p1, p2, p3를 x좌표 기준으로 정렬
    std::vector<Point> points = { p1, p2, p3 };
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.getx() < b.getx();
    });
    Point sorted_p1 = points[0];
    Point sorted_p2 = points[1];
    Point sorted_p3 = points[2];

    // p1, p2 / p2, p3 각각에 대해 weighted distance가 동일한 bisector 구하기

    // p1, p2 weighted distance가 동일한 bisector를 hyperbola로 구현: h1
    Hyperbola h1(p1, p2, p1.weight, p2.weight);

    // p2, p3 weighted distance가 동일한 bisector를 hyperbola로 구현: h2
    Hyperbola h2(p2, p3, p2.weight, p3.weight);



    // h1과 h2 사이의 교점 중 e를 기준으로 p1, p2, p3와 반대방향에 존재하는 교점을 Point로 바꾸기
    std::vector<Point> intersections = h1.intersectionPoints(h2);
    Point answer;

    // 반대방향 존재 확인을 두 선분(e, intersection 중 1개와 p1)이 교차하는지 확인해서 판단
    if (intersections.size() < 2) {
        answer = intersections[0];
    } else {
        Point eStart = e->getOrigin()->getPoint(); // 1
        Point eEnd = e->getNext()->getOrigin()->getPoint(); // 2

        Point iStart = p1; // 3
        Point iEnd = intersections[0]; // 4

        double flag1 = ccw(eStart.getx(), eStart.gety(), eEnd.getx(), eEnd.gety(), iStart.getx(), iStart.gety()) * ccw(eStart.getx(), eStart.gety(), eEnd.getx(), eEnd.gety(), iEnd.getx(), iEnd.gety());
        double flag2 = ccw(iStart.getx(), iStart.gety(), iEnd.getx(), iEnd.gety(), eStart.getx(), eStart.gety()) * ccw(iStart.getx(), iStart.gety(), iEnd.getx(), iEnd.gety(), eEnd.getx(), eEnd.gety());

        if (flag1 < 0 && flag2 < 0) {
            answer = intersections[0];
        } else {
            answer = intersections[1];
        }
    }
    return answer;
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

std::vector<WF_generator> WF_propagation::getMarked_cells(Face* f) {
    return this->marked_cells[f];
}