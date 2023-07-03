#include <iostream>
#include <algorithm>
#include <vector>
#include "C_Subdivision.h"
#include "DCEL/DCEL_IO.h"
#include "DCEL/Vector.h"

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

std::vector<Face*> well_covering_region(DCEL& dcel, HEdge* he){
    std::vector<Face*> regions;

    HEdge* twin_he = he->getTwin();
    //Two incident faces
    Face* f= he->getIncidentFace(), *twin_f = twin_he->getIncidentFace();

    std::set<Vertex*> incident_vertices;

    //Find collinear edges with he in the incident faces.
    for(auto bd_he:f->getOutHEdges()){
        if(is_colinear(he,bd_he)){
            incident_vertices.insert(bd_he->getOrigin());
            incident_vertices.insert(bd_he->getTwin()->getOrigin());
            //std::cout <<"Collinear"<< he->getEdge() <<' '<<bd_he->getEdge()<<std::endl;
        }
    }
    for(auto bd_he:twin_f->getOutHEdges()){
        if(is_colinear(he,bd_he)){
            incident_vertices.insert(bd_he->getOrigin());
            incident_vertices.insert(bd_he->getTwin()->getOrigin());
            //std::cout <<"Collinear"<< he->getEdge() <<' '<<bd_he->getEdge()<<std::endl;
        }
    }
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
    return std::vector<Face*>(incident_faces.begin(), incident_faces.end());
}

int main(){
    std::vector<Point> pts = {Point(0.5,0.5),Point(4.5,4.5), Point(100,100),Point(100.5,100.5)};
    C_Subdivision csubdiv(pts);

    std::cout<<"scale_factor: "<<csubdiv.scale_factor<<std::endl;
    std::cout<<"translation_x_factor: "<<csubdiv.tr_x_factor<<std::endl;
    std::cout<<"translation_y_factor: "<<csubdiv.tr_y_factor<<std::endl;

    //std::cout<<csubdiv.scale_factor <<' '<<csubdiv.tr_x_factor<<' '<<csubdiv.tr_y_factor<<std::endl;
    for(auto p : csubdiv.sites){
        std::cout<<p<<std::endl;
    }

    DCEL dcel = csubdiv.build_d_subdivision(1);
    //DCEL_IO::print_DCEL(dcel);
    std::string he_key = "e2_1";
    HEdge *he = dcel.getHedge(he_key);
    if(!he){
        std::cout <<"There is no key of half edges in DCEL"<<std::endl;
        return 0;
    }
    std::vector<Face*> regions = well_covering_region(dcel, he);
    
    std::cout << "Well-covering region of "<<he_key<<": ";
    for(auto f:regions){
        std::cout<<f->getKey()<<' ';
    }
    std::cout<<std::endl;

    return 0;
}