#include "CS_valid_check.h"
#include "WC_region.h"

#define tolerance 1e-6

double euc_dist_edges(Edge &a, Edge &b){
    double ret=0;
    //TODO
    return ret;
}

bool check_a1(DCEL& dcel) {
    bool ret=true;

    for (auto he:dcel.getHedges()){
        Vertex*a=he->getOrigin();
        Vertex* b = he->getNext()->getOrigin();
        double ax = a->getx();
        double ay = a->gety();
        double bx = b->getx();
        double by = b->gety();

        if (std::abs(ax - bx) > tolerance && std::abs(ay - by) > tolerance){
            ret=false;
            }
    }
    return ret;
}

bool check_w3(DCEL& dcel, int alpha){
    for(auto he: dcel.getHedges()){
        WC_region wc(dcel,he);
        Edge e = wc.e->getEdge();
        double len_e = e.length();
        for(auto hf: wc.boundary){
            Edge f = hf->getEdge();
            double len_f = f.length();
            if (euc_dist_edges(e,f) < alpha* std::max(len_e,len_f)) return false;
        }
    }
    return true;
}
