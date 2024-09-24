#include "WC_region.h"
#include "DCEL/Vector.h"
#include <set>

bool inline is_colinear(HEdge* a, HEdge* b){
    Point org = *a->getOrigin(), dest = *a->getTwin()->getOrigin();
    Vector vec_he(org,dest);
    Vector vec1(org, *b->getOrigin());
    Vector vec2(org, *b->getTwin()->getOrigin());
    double out_prod1 = vec_he.crossProduct(vec1);
    double out_prod2 = vec_he.crossProduct(vec2);
    if(std::abs(out_prod1) < 1e-6 && std::abs(out_prod2) < 1e-6)
        return true;
    return false;
}


//make it for the case of CS_free space
WC_region::WC_region(DCEL& dcel, HEdge* he){
    this->e = he;
    HEdge* twin_he = he->getTwin();

    std::set<Vertex*> incident_vertices;

    auto cur = he;
    do{
        if(is_colinear(cur,he)){
            incident_vertices.insert(cur->getOrigin());
            incident_vertices.insert(cur->getTwin()->getOrigin());
        }
        cur = cur->getNext();
    }while(cur != he);

    cur = twin_he;
    do{
        if(is_colinear(cur,twin_he)){
            incident_vertices.insert(cur->getOrigin());
            incident_vertices.insert(cur->getTwin()->getOrigin());
        }
        cur = cur->getNext();
    }while(cur != twin_he);

    std::set<Face*> incident_faces;
    //MODIFY LATER incominghedges
    for(auto v:incident_vertices){
        for(auto bd_he:dcel.getOutgoingHEdges(v)){
            incident_faces.insert(bd_he->getIncidentFace());
        }
    }
    this->regions.insert(this->regions.end(), incident_faces.begin(),incident_faces.end());
    std::set<HEdge*> wc_boundary;
    for(auto f : incident_faces){
        std::vector<HEdge*> hedges = f->getOutHEdges();
        std::vector<HEdge*> inner_hedges = f->getInnerHEdges();
        hedges.insert(hedges.end(),inner_hedges.begin(),inner_hedges.end());

        for(auto bd_he: hedges){
            Face* f = bd_he->getIncidentFace();
            Face* twin_f = bd_he->getTwin()->getIncidentFace();
            bool inc_f = incident_faces.find(f) == incident_faces.end();
            bool inc_twin_f = incident_faces.find(twin_f) == incident_faces.end();
            if(inc_f ^ inc_twin_f)
                wc_boundary.insert(bd_he);
        }
    }
    this->boundary.insert(this->boundary.end(), wc_boundary.begin(), wc_boundary.end());
}

WC_region_free::WC_region_free(CS_Free& cs_free, HEdge* he){
    // TODO
}