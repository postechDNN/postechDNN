#include "WC_region.h"
#include "DCEL/Vector.h"
#include <set>

constexpr double TOLERANCE_WC_REGION = 1e-6;

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


// TODO: make it for the case of CS_free space
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
        // for(auto bd_he: dcel.getIncomingHEdges(v)){
        //     incident_faces.insert(bd_he->getIncidentFace());
        //     //std::cout<<bd_he->getKey()<<' '<<bd_he->getIncidentFace()->getKey()<< ' ';
        // }
        for(auto bd_he:dcel.getOutgoingHEdges(v)){
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
            Face* f = bd_he->getIncidentFace();
            Face* twin_f = bd_he->getTwin()->getIncidentFace();
            bool inc_f = incident_faces.find(f) == incident_faces.end();
            bool inc_twin_f = incident_faces.find(twin_f) == incident_faces.end();
            if(inc_f ^ inc_twin_f)
                wc_boundary.insert(bd_he);
            // Point org = bd_he->getOrigin()->getPoint();
            // Point dest = bd_he->getTwin()->getOrigin()->getPoint();
            // int inc_flag = false;
            // for(auto v : incident_vertices){
            //     if (org == v->getPoint() || dest == v->getPoint()){
            //         inc_flag = true;
            //     }
            // }
            // if(inc_flag == false) wc_boundary.insert(bd_he);
        }
    }
    this->boundary.insert(this->boundary.end(), wc_boundary.begin(), wc_boundary.end());
}

WC_region_free::WC_region_free(CS_Free& cs_free, HEdge* he){
    this->e = he;

    std::unordered_map<std::string, HEdge_Type> types = cs_free.getEdge_types();

    if (types[he->getKey()] == HEdge_Type::HE_OPQ) {
        return;
    }
    
    bool added = false;

    HEdge* check_opq = he->getNext();
    HEdge* check_trp = he->getTwin()->getPrev();
    Face *f1, *f2, *f3;

    if (types[check_opq->getKey()] == HEdge_Type::HE_OPQ && types[check_trp->getKey()] == HEdge_Type::HE_TRP) {
        added = true;
        f1 = he->getIncidentFace();
        f2 = he->getTwin()->getIncidentFace();
        f3 = check_trp->getTwin()->getIncidentFace();
    }

    check_opq = he->getTwin()->getNext();
    check_trp = he->getPrev();

    if (types[check_opq->getKey()] == HEdge_Type::HE_OPQ && types[check_trp->getKey()] == HEdge_Type::HE_TRP) {
        added = true;
        f1 = he->getIncidentFace();
        f2 = he->getTwin()->getIncidentFace();
        f3 = check_trp->getIncidentFace();
    }

    if (added) {
        std::vector<HEdge*> square_edges;
        std::vector<Face*> faces = { f1, f2, f3 };

        for (Face* f : faces) {
            for (HEdge* he : f->getInnerHEdges()) {
                bool contain = false;
                for (HEdge* e : square_edges) {
                    if (e->getKey().compare(he->getKey()) == 0) {
                        bool contain = true;
                        break;
                    }
                }

                if (!contain) {
                    square_edges.push_back(he);
                }
            }
        }

        for (HEdge* e : square_edges) {
            WC_region wc(*cs_free.getDCEL(), e);

            for (Face* f : wc.regions) {
                for (Face* tf : this->regions) {
                    if (f->getKey().compare(tf->getKey()) != 0) {
                        this->regions.push_back(f);
                    }
                }
            }

            for (HEdge* b : wc.boundary) {
                for (HEdge* tb : this->boundary) {
                    if (b->getKey().compare(tb->getKey()) != 0) {
                        this->boundary.push_back(b);
                    }
                }
            }
        }
    }
    else {
        WC_region wc(*cs_free.getDCEL(), he);
        this->regions = wc.regions;
        this->boundary = wc.boundary;
    }
}