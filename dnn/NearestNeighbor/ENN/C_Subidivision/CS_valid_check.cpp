#include "CS_valid_check.h"
#include "WC_region.h"
#include <unordered_map>
#include <vector>
#include <limits>
#include <set>
#include "DCEL/Vector.h"

#define tolerance 1e-6
#define space_const_limit 150


bool check_strong_csdiv(std::vector<Point>& pts, DCEL& dcel, int alpha){
    std::cout<<"--------------Check if it is a strong "<<alpha<<"-conforming subdivision--------------\n";
    int pass = 1;

    std::cout<<"C1. Each cell of S contains at most one point of V in its closure: ";
    if(check_c1(pts, dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout <<"C2. Each edge of S is strongly well-covered with parameter "<<alpha<<std::endl;
    std::cout <<"\tW1. The set of cells C(e) is well defined: ";
    if(check_w1(dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    } 
    std::cout <<"\tW2. The total complexity of all the cells in C(e) is less than alpha*space_const_limit("<<alpha<<"*"<<space_const_limit<<"): ";
    if(check_w2(dcel,alpha)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }
    std::cout <<"\tW3. If f is a boundary edge of well-covering region, euclidean distance between e and f is at least "<<alpha<<"* max(|e|,|f|): ";
    if(check_w3(dcel,alpha)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout <<"C3. The well-covering region of every edge of S contains at most one vertex of V: ";
    if(check_c3(pts,dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }
    return pass;
}

bool check_add_properties(std::vector<Point>& pts, DCEL& dcel, int alpha){
    std::cout<<"--------------Check if it satisfies additional properties for a "<<alpha<<"-conforming subdivision--------------\n";
    int pass = 1;

    std::cout<<"A1. all edges of the subdivision are horizontal or vertical: ";
    if(check_a1(dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout<<"A2. each face is either a square or square-annulus: ";
    if(check_a2(dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout<<"A3. each annulus has the minimum clearance property: ";
    if(check_a3(dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout<<"A4. each face has the uniform edge property: ";
    if(check_a4(dcel,alpha)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }

    std::cout<<"A5. every data point is contained in the interior of a square face: ";
    if(check_a5(pts, dcel)) std::cout<<"PASS\n";
    else{
        std::cout <<"FAIL\n";
        pass = 0;
    }
    return pass;
}

double euc_dist_edges(Edge &a, Edge &b){
    if (a.crossing(b, true) != nullptr)
        return 0.0;

    double ret = 0.0;
    double tmp = 0.0;

    Point as = a.gets(), at = a.gett(), bs = b.gets(), bt = b.gett();

    ret = as.distance(bs);

    tmp = as.distance(bt);
    ret = tmp < ret ? tmp : ret;

    tmp = at.distance(bs);
    ret = tmp < ret ? tmp : ret;

    tmp = at.distance(bt);
    ret = tmp < ret ? tmp : ret;

    // as-at <-> bs
    if (Vector(as, at).innerProdct(Vector(as, bs)) > 0 && Vector(at, as).innerProdct(Vector(at, bs)) > 0) {
        tmp = abs(Vector(bs, as).outerProdct(Vector(bs, at))) / as.distance(at);
        ret = tmp < ret ? tmp : ret;
    }

    // as-at <-> bt
    if (Vector(as, at).innerProdct(Vector(as, bt)) > 0 && Vector(at, as).innerProdct(Vector(at, bt)) > 0) {
        tmp = abs(Vector(bt, as).outerProdct(Vector(bt, at))) / as.distance(at);
        ret = tmp < ret ? tmp : ret;
    }

    // bs-bt <-> as
    if (Vector(bs, bt).innerProdct(Vector(bs, as)) > 0 && Vector(bt, bs).innerProdct(Vector(bt, as)) > 0) {
        tmp = abs(Vector(as, bs).outerProdct(Vector(as, bt))) / bs.distance(bt);
        ret = tmp < ret ? tmp : ret;
    }

    // bs-bt <-> at
    if (Vector(bs, bt).innerProdct(Vector(bs, at)) > 0 && Vector(bt, bs).innerProdct(Vector(bt, at)) > 0) {
        tmp = abs(Vector(at, bs).outerProdct(Vector(at, bt))) / bs.distance(bt);
        ret = tmp < ret ? tmp : ret;
    }

    return ret;
}

bool check_w1(DCEL& dcel){
    for(auto he: dcel.getHedges()){
        WC_region wc(dcel,he);
        if(wc.regions.empty()){
            std::cout << "well-covering region of "<<he->getKey()<<" is empty\n";
            return false;
        }
    }
    return true;
}

bool check_w2(DCEL& dcel, int alpha){
    
    for(auto he: dcel.getHedges()){
        WC_region wc(dcel,he);
        std::set<Vertex*> vertices;
        std::set<HEdge*> hedges;
        for(auto f: wc.regions){
            int num_inners= f->getInners().size();
            for(int i = 0 ; i< num_inners; i++){
                auto vers = f->getInnerVertices(i);
                auto hes = f->getInnerHEdges(i);
                vertices.insert(vers.begin(),vers.end());
                hedges.insert(hes.begin(), hes.end());
            }
            auto vers = f->getOutVertices();
            auto hes = f->getOutHEdges();
            vertices.insert(vers.begin(), vers.end());
            hedges.insert(hes.begin(), hes.end());
        }
        int num_vertices = vertices.size();
        int num_hedges = hedges.size();
        int num_faces = wc.regions.size();
        double complexity = num_vertices + num_hedges + num_faces;
        if(complexity/(double)alpha > space_const_limit){
            std::cout << "Complexity of conforming subdivision(" << complexity<< ") is larger than alpha * space_const_limit("<< space_const_limit * alpha<<")\n";
            return false;
        }
    }
    

    return true;
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

bool check_c1(std::vector<Point>& pts, DCEL& dcel){
    for(auto f : dcel.getFaces()){
        int cnt = 0;
        for(auto p : pts)
            cnt += f->inFace(p);
        //std::cout<<cnt<<std::endl;
        // if(f->getInners().size()>0){
        //     std::cout <<"Annulus\n";
        //     std::cout<<"out\n";
        //     for(auto v:f->getOutVertices())
        //         std::cout<<*v<<std::endl;
        //     std::cout<<"in\n";
        //     for(auto v:f->getInnerVertices(0))
        //         std::cout<<*v <<std::endl;
        // }
        if(cnt >= 2){
            std::cout<<"cnt >= 2\n";
            for(auto v:f->getOutVertices())
                std::cout<<*v<<std::endl;
            
            if(f->getInners().size()>0)
            for(auto v:f->getInnerVertices(0))
                std::cout<<*v <<std::endl;
            //return false;
        }
    }
    return true;
}


bool check_c2(DCEL& dcel, int alpha){
    return check_w1(dcel) & check_w2(dcel,alpha) & check_w3(dcel,alpha);
}

bool check_c3(std::vector<Point>& pts, DCEL& dcel) {
    std::unordered_map<std::string, int> vnum;

    for (auto face : dcel.getFaces()) {
        if (face->isOutMost()) {
            continue;
        }

        int count = 0;

        for (auto p : pts) {
            if (face->inFace(p, true))
                count++;
        }

        vnum[face->getKey()] = count;
    }

    for (auto he : dcel.getHedges()) {
        WC_region wc(dcel, he);
        std::vector<Face*> regions = wc.regions;

        int sum = 0;
        for (auto f : regions) {
            sum += vnum[f->getKey()];
        }

        if (sum > 1) {
            return false;
        }
    }

    return true;
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


bool check_a2(DCEL& dcel) {
    std::vector<Face*> faces = dcel.getFaces();
    for (auto f : faces) {
        /* Skip the outmost face*/
        if (f->isOutMost()) continue;

        std::vector<HEdge*> inners = f->getInners();

        if(inners.size()>1) return false;

        // Get all edge of inner face
        std::vector<HEdge*> innerEdge;
        if (!inners.empty()) {
            auto cur = inners[0];
            do {
                innerEdge.push_back(cur);
                cur = cur->getNext();
            } while (cur != inners[0]);
        }

        std::vector<HEdge*> squares[2] = { innerEdge , f->getOutHEdges() };
        /* Check inner, outer square */
        for (int i = 0; i < 2; i++) { 
            if (squares[i].empty()) continue;

            // Compute boundary of sqaure
            double bnd[4] = { std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() }; // minx maxx miny maxy
            for (auto edge : squares[i]) {
                double nowX[2] = { edge->getEdge().gets().getx(), edge->getEdge().gett().getx() };
                double nowY[2] = { edge->getEdge().gets().gety(), edge->getEdge().gett().gety() };
                for (int j = 0; j < 2; j++) {
                    if (bnd[0] > nowX[j]) bnd[0] = nowX[j];
                    if (bnd[1] < nowX[j]) bnd[1] = nowX[j];
                    if (bnd[2] > nowY[j]) bnd[2] = nowY[j];
                    if (bnd[3] < nowY[j]) bnd[3] = nowY[j];
                }
            }

            // Compute length of each side
            double lengths[4] = { 0,0,0,0 }; // left, right, top, bottom
            for (auto edge : squares[i]) {
                double nowX[2] = { edge->getEdge().gets().getx(), edge->getEdge().gett().getx() };
                double nowY[2] = { edge->getEdge().gets().gety(), edge->getEdge().gett().gety() };
                // Left edge
                if (std::abs(nowX[0] - bnd[0]) < tolerance && std::abs(nowX[1] - bnd[0]) < tolerance) {
                    lengths[0] += std::abs(nowY[0] - nowY[1]);
                }
                // Right edge
                else if (std::abs(nowX[0] - bnd[1]) < tolerance && std::abs(nowX[1] - bnd[1]) < tolerance) {
                    lengths[1] += std::abs(nowY[0] - nowY[1]);
                }
                // Top edge
                else if (std::abs(nowY[0] - bnd[3]) < tolerance && std::abs(nowY[1] - bnd[3]) < tolerance) {
                    lengths[2] += std::abs(nowX[0] - nowX[1]);
                }
                // Bottom edge
                else if (std::abs(nowY[0] - bnd[2]) < tolerance && std::abs(nowY[1] - bnd[2]) < tolerance) {
                    lengths[3] += std::abs(nowX[0] - nowX[1]);
                }
                // False (Not square)
                else return false;
            }

            // length check            
            for (int j = 0; j < 3; j++) {
                if (std::abs(lengths[j] - lengths[j + 1]) > tolerance) return false; 
            }
        }
    }
    return true;
}

bool check_a3(DCEL& dcel) {
    std::vector<Face*> faces = dcel.getFaces();
    for (auto f : faces) {
        std::vector<HEdge*> inners = (f->getInners());
        std::vector<HEdge*> outers = (f->getOutHEdges());
        /* Case 1) The outmost face*/
        if (f->isOutMost()) continue;
        /* Case 2) Square faces */
        else if (inners.empty()) continue;
        /* Case 3) Square-annulus faces */
        else {

            // Get all edge of inner square
            std::vector<HEdge*> innerEdge;
            auto cur = inners[0];
            do {
                innerEdge.push_back(cur);
                cur = cur->getNext();
            } while (cur != inners[0]);

            // Compute boundary of inner square
            double inner[4] = { std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() }; // minx maxx miny maxy
            for (auto edge : innerEdge) {
                double nowX[2] = { edge->getEdge().gets().getx(), edge->getEdge().gett().getx() };
                double nowY[2] = { edge->getEdge().gets().gety(), edge->getEdge().gett().gety() };
                for (int i = 0; i < 2; i++) {
                    if (inner[0] > nowX[i]) inner[0] = nowX[i];
                    if (inner[1] < nowX[i]) inner[1] = nowX[i];
                    if (inner[2] > nowY[i]) inner[2] = nowY[i];
                    if (inner[3] < nowY[i]) inner[3] = nowY[i];
                }
            }

            // Compute boundary of outer square
            double outer[4] = { std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() }; // minx maxx miny maxy
            for (auto edge : outers) {
                double nowX[2] = { edge->getEdge().gets().getx(), edge->getEdge().gett().getx() };
                double nowY[2] = { edge->getEdge().gets().gety(), edge->getEdge().gett().gety() };
                for (int i = 0; i < 2; i++) {
                    if (outer[0] > nowX[i]) outer[0] = nowX[i];
                    if (outer[1] < nowX[i]) outer[1] = nowX[i];
                    if (outer[2] > nowY[i]) outer[2] = nowY[i];
                    if (outer[3] < nowY[i]) outer[3] = nowY[i];
                }
            }

            // Check minimum clearance property
            double sideLength = std::abs(outer[1] - outer[0])/4;

            if (outer[0] + sideLength - tolerance > inner[0]) return false;
            if (outer[1] - sideLength + tolerance < inner[1]) return false;
            if (outer[2] + sideLength - tolerance > inner[2]) return false;
            if (outer[3] - sideLength + tolerance < inner[3]) return false;
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
            //std::cout <<"[A4] Square Face" << std::endl;
            double shortest = std::numeric_limits<double>::max();
            double longest = 0; 
            for(auto he:outers){
                double length = (he -> getEdge()).length(); // Check if it works

                if(shortest > length) shortest = length;
                if(longest < length) longest = length;
            }

            // Check if he lengths of edges on the boundary of a square cell differ by at most a factor of 4.
            if(shortest < ((longest / 4) - tolerance)) {
                return false;
            }
            
        }
        /* Case 3) Square-annulus faces */
        else{
            //std::cout <<"[A4] Check Square-annulus Face" << std::endl;
            double factor = 1./double(double(4) * alpha);

            // A) Check if every edge on the outer square of an annulus has length 1/(4⌈𝛼⌉) times the side length of the outer square.
            double outersidelen = 0;
            double shortest = std::numeric_limits<double>::max();
            for(auto he:outers){
                double length = (he -> getEdge()).length(); // Check if it works
                outersidelen += length;

                if(shortest > length) shortest = length; // Store the shortest edge length
            }

            if (shortest < ((outersidelen/4 * factor)-tolerance)){
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

                if (shortest < ((innersidelen/4 * factor)-tolerance)){
                    return false;
                }

            }
        }
    }
    return true;
}

bool check_a5(std::vector<Point>& pts,DCEL& dcel){ return false;}