#include "test_L_infty_DT.h"

#define tolerance 1e-6

bool test_L_infty_DT(std::vector<Point> sites, std::vector< std::vector<int>> graph){
    // 1. Test if it is a planar
    int n = sites.size();
    std::vector<Edge> edges;
    std::vector<std::pair<int,int> > edges_sites;
    for(int u = 0; u<n;u++){
        for(int v : graph[u]){
            if(v > u){
                edges.push_back(Edge(sites[u], sites[v]));
                edges_sites.push_back(std::make_pair(u,v));
            }
        }
    }   
    for(int i = 0;i<edges.size();i++){
        for( int j = i+1; j<edges.size();j++){
            Edge e_1 =edges[i], e_2 = edges[j];
            if(e_1.crossing(e_1,false)){    //There is a crossing between e_1 and e_2.
                std::cout <<"It is not a planar, because there is an intersection between two edges" <<std::endl;
                std::cout <<"1. " << e_1<< ": "<<edges_sites[i].first <<' '<< edges_sites[i].second<<std::endl;
                std::cout <<"2. "<< e_2<< ": "<<edges_sites[j].first <<' '<< edges_sites[j].second<<std::endl;
                return false;
            } 
        }
    }
    // 2. Test if it is a triangulation
    DCEL dcel(sites,graph);

    for(auto f : dcel.getFaces()){
        if(f->isOutMost()){
            if(f->getInners().size() != 1){ //There are at least 2 inner components in an outer face 
                return false;
            }
            HEdge *he = f->getInners()[0];
            HEdge *cur = he;
            do{
                HEdge* prev = cur->getPrev();
                Point p1 = *prev->getOrigin();
                Point p2 = *cur->getOrigin();
                Point p3 = *cur->getTwin()->getOrigin();
                Vector v1(p1,p2), v2(p2,p3);
                if ( v1.crossProduct(v2) < -tolerance){ //There is a right-turn, that is, the boundary does not form a convex polygon
                    return false;
                }
                cur = cur->getNext();
            }while(cur != he);
        }
        else{
            if(!f->getInners().empty()){ //There is an inner component in a face
                return false;
            }
            if(f->getOutHEdges().size() != 3){   // The outer boundary is not a triangle
                return false;
            }
            for(auto p : sites){ // test if a face contains a site in its interior.
                if(f->inFace(p, true)){
                    return false;
                }
            }
        }
    }

    // TODO: 3. Test if it holds the definition of the L-infty Delaunay Triangulation.
    return true;
}