#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Edge.h"
#include "Point.h"
#include <vector>
#include "EdgesIntersection.h"
#include "Graph.h"

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
    std::string fname = "lines_input1.ipe";
    if(argc == 2) fname = std::string(argv[1]); 
    std::vector<Edge> edges = readEdges(fname);
    EdgesIntersection func(edges);
    
    Graph<Point> g =func.getGraph();
    int i=0;
    for(auto it:g.getVertices()){
        std::cout << i << ": "<<it<< ' ';
        for(auto j : g.getAdjlist(i)){
            std::cout <<j<<' ';
        }
        std::cout << std::endl;
        i++;
    }

    return 0;
}