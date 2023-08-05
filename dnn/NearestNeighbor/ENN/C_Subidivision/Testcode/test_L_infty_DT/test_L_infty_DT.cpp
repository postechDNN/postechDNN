#include "test_L_infty_DT.h"

#define tolerance 1e-6

bool test_triangulation(std::vector<Point> &sites, std::vector< std::vector<int>> &graph){
    // 1. Test if it is a planar
    int n = sites.size();
    std::vector<Edge> edges;
    std::vector<std::pair<int,int> > edges_sites;

    //collect edges of the graph.
    for(int u = 0; u<n;u++){
        for(int v : graph[u]){
            if(v > u){
                edges.push_back(Edge(sites[u], sites[v]));
                edges_sites.push_back(std::make_pair(u,v));
            }
        }
    }   
    
    //Test if there is a crossing between two edges in their interior.
    for(int i = 0;i<edges.size();i++){
        for( int j = i+1; j<edges.size();j++){
            Edge e_1 =edges[i], e_2 = edges[j];
            if(e_1.crossing(e_2,false) || e_2.crossing(e_1,false)){    //There is a crossing between e_1 and e_2.
                std::cout <<"It is not a planar, because there is an intersection between two edges" <<std::endl;
                std::cout <<"1. " << e_1<< ": "<<edges_sites[i].first <<' '<< edges_sites[i].second<<std::endl;
                std::cout <<"2. "<< e_2<< ": "<<edges_sites[j].first <<' '<< edges_sites[j].second<<std::endl;
                return false;
            } 
        }
    }
    //Now, it is a planar graph. 

    // 2. Test if it is a triangulation
    DCEL dcel(sites,graph);

    for(auto f : dcel.getFaces()){
        if(f->isOutMost() == true){
            if(f->getInners().size() != 1){ //There are at least 2 inner components in an outer face
                std::cout <<"The number of inner components in the outmost face is "<<f->getInners().size()<<std::endl; 
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
                if ( v1.crossProduct(v2) > tolerance){ //There is a left-turn, that is, the boundary does not form a convex polygon
                    std::cout <<"The outer boundary does not form a convex polygon"<<std::endl;
                    return false;
                }
                cur = cur->getNext();
            }while(cur != he);
        }
        else{
            if(!f->getInners().empty()){ //There is an inner component in a face
                std::cout<<"There is an inner component in the face "<<f->getKey()<<std::endl;
                return false;
            }
            if(f->getOutHEdges().size() != 3){   // The outer boundary is not a triangle
                std::cout <<"The face "<<f->getKey()<< " is not a triangle"<<std::endl;
                return false;
            }
            for(auto p : sites){ // test if a face contains a site in its interior.
                if(f->inFace(p, true)){
                    std::cout << "The face "<<f->getKey()<< " contains the site "<<p<< " in its interior"<<std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_L_infty_DT_condition(DCEL &dcel){
    bool DT_prop = true;
    auto faces = dcel.getFaces();
    for(auto f: faces){
        if(f->isOutMost() == true) continue;
        //1. For each a triangle face, find the smallest circumsquare of their vertices. (There can be several circumsquares)
        //1-1. compute a bounding box of a triangle boundary.
        auto vers = f->getOutVertices();
        double min_x, min_y, max_x, max_y;
        min_x = min_y = std::numeric_limits<double>::max();
        max_x = max_y = std::numeric_limits<double>::min();
        for(auto v: vers){
            double x = v->getx(), y =v->gety();
            min_x = min_x > x ? x: min_x;
            max_x = max_x < x ? x : max_x;
            min_y = min_y > y ? y : min_y;
            max_y = max_y < y ? y :max_y;
        }
        //1-2. compute the smallest squares circumscribing the bounding box and test if it contains no vertex in its interior.
        double x_len = max_x - min_x, y_len =max_y - min_y;
        if(x_len < y_len){
            auto vers = dcel.getVertices();
            double left_x = std::numeric_limits<double>::min();
            double right_x= std::numeric_limits<double>::max();
            for(auto v: vers){
                double x = v->getx(), y = v->gety();
                if(min_y+tolerance < y && y <max_y-tolerance){  //inside the interior in the y-slab
                    if(x < min_x -tolerance) //left half
                        left_x = left_x < x ? x : left_x;
                    if(x > max_x + tolerance) //right half
                        right_x = right_x > x ? x :right_x;
                    if(min_x + tolerance < x && x< max_x - tolerance)   //inside bounding box
                        DT_prop = false;
                }
            }
            if(right_x - left_x < y_len- tolerance)
                DT_prop = false;
        }
        else{
            auto vers = dcel.getVertices();
            double bottom_y = std::numeric_limits<double>::min();
            double top_y= std::numeric_limits<double>::max();
            for(auto v: vers){
                double x = v->getx(), y = v->gety();
                if(min_x+tolerance < x && x <max_x-tolerance){  //inside the interior in the y-slab
                    if(y < min_y -tolerance) //bototm half
                        bottom_y = bottom_y < y ? y : bottom_y;
                    if(y > max_y + tolerance) //top half
                        top_y = top_y > y ? y :top_y;
                    if(min_y + tolerance < y && y< max_y -tolerance)    //inside bounding box
                        DT_prop = false;
                }
            }
            if(top_y - bottom_y < x_len- tolerance) DT_prop = false;
        }
    }
    return DT_prop;
}

bool test_L_infty_DT(std::vector<Point> &sites, std::vector< std::vector<int>> &graph){
    if(!test_triangulation(sites,graph))
        return false;
    DCEL dcel(sites,graph);
    if(!test_L_infty_DT_condition(dcel)){
        std::cout << "the triangulation does not satisfy L-infinity Delaunay triangulation condition\n";
        return false;
    }
    return true;
}