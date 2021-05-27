#pragma once
#include "dPoint.h"
#include "dEdge.h"
#include "dFace.h"
#include "dDCEL.h"

bool dCoplanar(dEdge* e1, dEdge* e2);
bool dCoplanar(vector<dEdge*> es);
bool dCoplanar(dPoint* p1, dPoint* p2, dPoint* p3);
bool dCoplanar(vector<dPoint*> pts);


bool dCoplanar(dEdge* e1, dEdge* e2){
    return dCoplanar(e1->get_s(),e1->get_t(),e2->get_s(),e2->get_t());
}
bool dCoplanar(vector<dEdge*> es){
    if (es.size()<2) return false;
    for (int i=1;i<es.size();i++){
        if (!dCoplanar(es[0],es[i])) return false;
    }
    return true;
}
//core
bool dCoplanar(dPoint* p1, dPoint* p2, dPoint* p3, dPoint* p4){

}
bool dCoplanar(vector<dPoint*> pts){
    if (pts.size()<4) return false;
    for(int i=3;i<pts.size();i++){
        if (!dCoplanar(pts[0],pts[1],pts[2],pts[i])) return false;
    }
    return true;
}