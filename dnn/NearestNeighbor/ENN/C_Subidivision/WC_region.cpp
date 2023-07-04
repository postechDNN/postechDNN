#include "WC_region.h"
#include "DCEL/Vector.h"
#include <set>

bool inline is_colinear(HEdge* a, HEdge* b){
    Point org = *a->getOrigin(), dest = *a->getTwin()->getOrigin();
    Vector vec_he(org,dest);
    Vector vec1(org, *b->getOrigin());
    Vector vec2(org, *b->getTwin()->getOrigin());
    double out_prod1 = vec_he.outerProdct(vec1);
    double out_prod2 = vec_he.outerProdct(vec2);
    if(std::abs(out_prod1) < 1e-6 && std::abs(out_prod2) < 1e-6)
        return true;
    return false;
}


WC_region::WC_region(DCEL& dcel, HEdge* he){
    this->e = he;
    HEdge* twin_he = he->getTwin();

    std::set<Vertex*> incident_vertices;

    auto cur = he;
    do{
        if(is_colinear(cur,he)){
            incident_vertices.insert(he->getOrigin());
            incident_vertices.insert(he->getTwin()->getOrigin());
        }
        cur = cur->getNext();
    }while(cur != he);

    cur = twin_he;
    do{
        if(is_colinear(cur,twin_he)){
            incident_vertices.insert(he->getOrigin());
            incident_vertices.insert(he->getTwin()->getOrigin());
        }
        cur = cur->getNext();
    }while(cur != twin_he);

    std::set<Face*> incident_faces;
    //MODIFY LATER incominghedges
    for(auto v:incident_vertices){
        //std::cout <<v->getKey() <<": ";
        for(auto bd_he: dcel.getIncomingHEdges(v)){
            if(!bd_he->getIncidentFace()->isOutMost())
                incident_faces.insert(bd_he->getIncidentFace());
            //std::cout<<bd_he->getKey()<<' '<<bd_he->getIncidentFace()->getKey()<< ' ';
        }
        for(auto bd_he:dcel.getOutgoingHEdges(v)){
            if(!bd_he->getIncidentFace()->isOutMost())
                incident_faces.insert(bd_he->getIncidentFace());
            //std::cout<<bd_he->getKey()<<' '<<bd_he->getIncidentFace()->getKey()<< ' ';
        }
        //std::cout<<std::endl;
    }
    this->regions.insert(this->regions.end(), incident_faces.begin(),incident_faces.end());
    std::set<HEdge*> wc_boundary;
    for(auto f : incident_faces){
        std::vector<HEdge*> hedges = f->getOutHEdges();
        std::vector<HEdge*> inner_hedges = f->getInnerHEdges();
        hedges.insert(hedges.end(),inner_hedges.begin(),inner_hedges.end());

        for(auto bd_he: hedges){
            Point org = bd_he->getOrigin()->getPoint();
            Point dest = bd_he->getTwin()->getOrigin()->getPoint();
            int inc_flag = false;
            for(auto v : incident_vertices){
                if (org == v->getPoint() || dest == v->getPoint()){
                    inc_flag = true;
                }
            }
            if(inc_flag == false) wc_boundary.insert(bd_he);
        }
    }
    this->boundary.insert(this->boundary.end(), wc_boundary.begin(), wc_boundary.end());
}