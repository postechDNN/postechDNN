#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Edge.h"
#include "Point.h"
#include <vector>
#include "DCEL.h"
#include "DCEL_IO.h"
#include "DCEL_operation.h"
#define tolerance 1e-6

std::vector<Edge> readEdges(std::string fname){
    std::ifstream fp; 
    std::stringstream _c;
    std::vector<Edge> ret_vec;
    fp.open(fname); 
    if(fp.is_open()){
        _c << fp.rdbuf(); 
        std::string all_txt = _c.str(); 
        int start_pos = all_txt.find("<page>",0);
        std::stringstream _c(all_txt.substr(start_pos));
        std::string buf;
        int n_edges = 0;
        while(std::getline(_c,buf,'\n')){ 
            if(buf.find("<path")!= std::string::npos){ //find path text
                n_edges++;
                double x_1,y_1,x_2,y_2;
                std::getline(_c,buf,'\n');
                std::stringstream stream_str(buf);
                stream_str >> x_1;
                stream_str >> y_1;
                std::getline(_c,buf,'\n');
                stream_str = std::stringstream(buf); 
                stream_str >> x_2;
                stream_str >> y_2;
                ret_vec.push_back(Edge(Point(x_1,y_1),Point(x_2,y_2))); 
                //std::cout << x_1 <<' '<< y_1 <<' '<<x_2 <<' '<< y_2 << std::endl; //TEST
            }
        }

    }
    return ret_vec;
}

int main(int argc, char *argv[]){
    /*std::string fname = "input6_1.txt";
    DCEL d1 = DCEL_IO::text_to_DCEL(fname);
    DCEL d2 = DCEL_IO::text_to_DCEL("input1.txt");
    DCEL d3 = d1.merge(d2);

    auto faces = d3.getFaces();
    for(auto f:faces){
        std::cout <<"Face key: "<<f->getKey()<<std::endl;
        std::cout <<"Outer: ";
        if(f->getOutHEdges().size() == 0) std::cout << "Outmost";
        for(auto e: f->getOutHEdges())
            std::cout<< e->getOrigin()->getKey() << *e->getOrigin() << " ";
        std::cout <<std::endl;
        std::cout <<"Inner: ";
        if(f->getInnerHEdges().size() == 0) std::cout << "None";
        for(auto e: f->getInnerHEdges())
            std::cout<< e->getOrigin()->getKey() << *e->getOrigin()<< " ";
        std::cout <<std::endl;
    }*/
    std::vector<Point > pts={Point(14,-9),Point(15,-9),Point(15,-8),Point(14,-8)};
    std::vector<Vertex* > vertices;
    std::vector<HEdge* > hedges;
    std::vector<HEdge* > twins;

    for(int i = 0 ; i<pts.size();i++)
        vertices.push_back(new Vertex(pts[i]));

    for(int i = 0 ; i<vertices.size();i++){
        int j = (i+1)%vertices.size();
        HEdge *he = new HEdge(), *twin = new HEdge();
        he->setOrigin(vertices[i]), twin->setOrigin(vertices[j]);
        he->setTwin(twin), twin->setTwin(he);
        hedges.push_back(he), twins.push_back(twin);
    }
    for(int i = 0 ; i<hedges.size();i++){
        int j = (i+1)%hedges.size();
        hedges[i]->setNext(hedges[j]);
        hedges[j]->setPrev(hedges[i]);
    }
    for(int i = 0 ; i<twins.size(); i++){
        int j = (i+1)%twins.size();
        twins[i]->setPrev(twins[j]);
        twins[j]->setNext(twins[i]);
    }

    std::vector<HEdge*> ret_hedges;
    ret_hedges.insert(ret_hedges.begin(),hedges.begin(),hedges.end());
    ret_hedges.insert(ret_hedges.begin(),twins.begin(),twins.end());

    std::vector<Face*> ret_faces = ConstructFaces(ret_hedges);
    for(auto f:ret_faces){
        std::cout <<"Face key: "<<f->getKey()<<std::endl;
        std::cout <<"Outer: ";
        if(f->getOutHEdges().size() == 0) std::cout << "Outmost";
        for(auto e: f->getOutHEdges())
            std::cout<< e->getOrigin()->getKey() << *e->getOrigin() << " ";
        std::cout <<std::endl;
        std::cout <<"Inner: ";
        if(f->getInnerHEdges().size() == 0) std::cout << "None";
        for(auto e: f->getInnerHEdges())
            std::cout<< e->getOrigin()->getKey() << *e->getOrigin()<< " ";
        std::cout <<std::endl;
    }
    return 0;
}