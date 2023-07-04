#include "CS_valid_check.h"
#include "WC_region.h"
#include <unordered_map>
#include <vector>
#include <limits>
#include "DCEL/Vector.h"

#define tolerance 1e-6

double euc_dist_edges(Edge &a, Edge &b){
    double ret=0;
    //TODO
    return ret;
}


bool is_left(HEdge& he, Point& p) {
    Vertex* a = he.getOrigin();
    Vertex* b = he.getNext()->getOrigin();
    double ax = a->getx();
    double ay = a->gety();
    double bx = b->getx();
    double by = b->gety();
    double px = p.getx();
    double py = p.gety();

    return (bx - ax) * (py - ay) - (by - ay) * (px - ax) > 0;
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


bool check_c3(std::vector<Point>& pts, DCEL& dcel) {
    std::unordered_map<std::string, int> vnum;
    
    for (auto face : dcel.getFaces()) {
        if (face->isOutMost()) {
            continue;
        }

        std::string key = face->getKey();
        int count = 0;

        for (auto p : pts) {
            bool is_valid = true;

            for (auto he : face->getOutHEdges()) {
                if (!is_left(*he, p)) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid)
                continue;

            Face* cur = NULL;
            for (auto he : face->getInnerHEdges()) {
                if (cur != he->getTwin()->getIncidentFace()) {
                    cur = he->getTwin()->getIncidentFace();
                    //std::cout << "inner face key: " << cur->getKey() << '\n';
                    bool flag = true;

                    for (auto inner_he : cur->getOutHEdges()) {
                        if (!is_left(*inner_he, p)) {
                            flag = false;
                            break;
                        }
                    }

                    if (flag) {
                        is_valid = false;
                        break;
                    }
                }
            }

            if (is_valid) count++;
        }
        
        vnum[key] = count;
        //std::cout << key << ' ' << count << '\n';
    }

    for (auto he : dcel.getHedges()) {
        WC_region wc(dcel,he);
        std::vector<Face*> regions = wc.regions;
        
        int sum = 0;
        for (auto f : regions) {
            sum += vnum[f->getKey()];
        }

        if (sum > 1) {
            //std::cout << he->getKey() << ' ';
            //std::cout << sum << '\n';
            //std::cout << he->getOrigin()->getx() << ' ' << he->getOrigin()->gety();
            //std::cout << ", " << he->getNext()->getOrigin()->getx() << ' ' << he->getNext()->getOrigin()->gety() << '\n';
            //std::cout << he->getIncidentFace()->getKey() << '\n';
            //std::cout << '\n';
            return false;
        }
    }

    return true;
}

bool check_a4(DCEL& dcel, int alpha){
    std::vector<Face*> faces = dcel.getFaces();
    for(auto f:faces){
        std::vector<HEdge*> inners = (f -> getInners());
        std::vector<HEdge *> outers = (f-> getOutHEdges());

        /* Case 1) The outmost face*/
        if(f -> isOutMost()) continue;

        /* Case 2) Square faces */
        else if(inners.empty()){
            std::cout <<"[A4] Square Face" << std::endl;
            double shortest = std::numeric_limits<double>::max();
            double longest = 0; 
            for(auto he:outers){
                double length = (he -> getEdge()).length(); // Check if it works

                if(shortest > length) shortest = length;
                if(longest < length) longest = length;
            }

            // Check if he lengths of edges on the boundary of a square cell differ by at most a factor of 4.
            if(shortest < (longest / 4)) {
                return false;
            }
            
        }
        /* Case 3) Square-annulus faces */
        else{
            std::cout <<"[A4] Check Square-annulus Face" << std::endl;
            double factor = 1/double(double(4) * alpha);

            // A) Check if every edge on the outer square of an annulus has length 1/(4⌈𝛼⌉) times the side length of the outer square.
            double outersidelen = 0;
            double shortest = std::numeric_limits<double>::max();
            for(auto he:outers){
                double length = (he -> getEdge()).length(); // Check if it works
                outersidelen += length;

                if(shortest > length) shortest = length; // Store the shortest edge length
            }

            if (shortest < (outersidelen * factor)){
                return false;
            }

            // B) Check if every edge on the inner square has length 1/(4⌈𝛼⌉) times the side length of the inner square.
            for (auto innerF: inners){
                double innersidelen = 0;
                double shortest = std::numeric_limits<double>::max();
                auto cur = innerF;
                do{
                    cur = cur->getNext();
                    double length = (cur -> getEdge()).length();
                    innersidelen += length;
                    if(shortest > length) shortest = length; 
                }while(cur != innerF);

                if (shortest < (outersidelen * factor)){
                return false;
            }

            }
        }
    }
    return true;
}