#include "C_Subdivision.h"
#include <algorithm>
#include <queue>
#include <limits>
#include <cmath>
#include "Disjoint_Set.h"

Site::Site(){}
Site::Site(Point& p, Site_type t){
    this->x = p.getx();
    this->y = p.gety();
    this->t = t;
}
Site::~Site(){}

Quad::Quad(int r, int c ,int ord, bool is_simple=false):r(r), c(c), ord(ord),growth(nullptr), is_simple(is_simple){}
Quad::~Quad(){}

Box_Edge::Box_Edge(int r,int c,int ord,bool is_vertical):r(r),c(c),ord(ord),is_vertical(is_vertical){}
Box_Edge::~Box_Edge(){}
// Component::Component(std::vector<Quad*>& quads,bool is_simple):quads(quads), is_simple(is_simple) {}
// Component::~Component(){}

//Compute equivalent classes from quad sets.
std::vector<Component > C_Subdivision::compute_equiv_class(std::vector<Quad*>& Q){
    Equiv_Class equiv_C(Q);

    std::vector<std::vector<int> > adj_list(Q.size());

    //Compute intersection_relations between quads of next_Q 
    for(int i = 0; i<Q.size();i++){
        for(int j = i+1;j<Q.size();j++){
            Quad* q_i = Q[i], *q_j = Q[j];
            if(is_intersect_quad(q_i,q_j)){
                //Connect the node for i and the node for j
                adj_list[i].push_back(j);
                adj_list[j].push_back(i);
            }
        }
    }
    //Compute their transitive closure of the intersection graph using BFS.
    std::vector<bool> visited(Q.size(),false);
    for(int i = 0 ; i<Q.size();i++){
        if(!visited[i]){
            //BFS traverse    
            std::queue<int> Queue;
            Queue.push(i);
            while(!Queue.empty()){
                int u = Queue.front();
                Queue.pop();
                visited[u] = true;
                equiv_C.set_union(Q[i],Q[u]);
                for(auto v: adj_list[u]){
                    if(!visited[v]) Queue.push(v);
                }
            }
        }
    }
    
    //Set whether it is a simple or not for each quad in equiv_C
    auto equiv_classes = equiv_C.get_all_sets();
    for(auto S:equiv_classes){
        if(S.size() == 1 && S[0]->children.size()==1){
            S[0]->is_simple = true;
        }
    }
    return equiv_classes;
}

//Return quads which are the growth of given equivalent class S.
std::vector<Quad*> C_Subdivision::growth(std::vector<Quad*>& S){
    std::vector<Quad*> ret;
    //TODO
    //Caution: need to set the growth variable for each quad in S
    //Caution: need to set the children variable for each new quad in ret.
    //Caution: set "is_simple" variable to false in default.

    return ret;
}

//Return true if two quads of the same order intersect each other
bool inline C_Subdivision::is_intersect_quad(Quad* a, Quad* b){
    int a_r = a->r, a_c = a->c;
    int b_r = b->r, b_c = b->c;
    if(std::abs(a_r - b_r)<= 4 && std::abs(a_c-b_c)<=4) return true;
    else return false;
}

void inline C_Subdivision::draw_n_edges(int r,int c, int ord, int is_vertical, int n){
    int is_horizontal = 1- is_vertical;
    for(int i = 0;i<n;i++)
        this->drawn_edges.push_back(Box_Edge(r+is_horizontal*i, c+is_vertical*i,ord,is_vertical));
}

//Construct strong conforming subdivision of points. 
C_Subdivision::C_Subdivision(Point src, std::vector<Point> obs_pts){
    //1. Initialize sites and the set of quads.
    int i = 0;
    init_sites(src, obs_pts);
    std::vector<Quad*> Q =init_quads();
    std::vector<Component > equiv_classes = compute_equiv_class(Q);

    //2. build-subdivision.
    while (!Q.empty())
    {
        //Step 1. Increment order
        i+=2;
        
        //Step 2. Compute Q(i) from Q(i-2)
        std::vector<Quad*> next_Q; //TODO: Is it okay that change the data structure from set to vector

        for(auto S: equiv_classes){
            std::vector<Quad*> g_S = growth(S);
            next_Q.insert(next_Q.end(),g_S.begin(),g_S.end());
        }
        //Construct equivalent relation on next_Q
        std::vector<Component > next_equiv_classes = compute_equiv_class(next_Q);

        //Step 3. Process simple components of equivalent relation in Q(i-2) that are about to merge with some other component.
        for(auto q:Q){
            Quad* q_bar = q->growth;
            if(q->is_simple && !q_bar->is_simple){
                //Draw the boundary box of q and subdivide each of its sides into four edges at the (i-2)-order grid lines.
                draw_n_edges(q->r,q->c,i-2,false,4);
                draw_n_edges(q->r+4,q->c,i-2,false,4);
                draw_n_edges(q->r,q->c,i-2,true,4);
                draw_n_edges(q->r,q->c+4,i-2,true,4);
            }
        }

        //Step 4. Process complex components.
        for(auto S:next_equiv_classes){
            std::vector<Quad *> children_S;
            for(auto q: S){
                auto children_q = q->children;
                children_S.insert(children_S.end(),children_q.begin(),children_q.end());
            } 
            if(children_S.size() > 1){  // S is complex
                //TODO: Compute R_1 and R_2.
                //TODO: Draw (i-2)-boxes to fill the region between the boundaries of R_1 and R_2.
                //TODO: Draw i-boxes to fill the region between the boundaries of R_1 and S.
                //TODO: Break each cell boundary with an endpoint incident to R_1 into four edges of length 2^(i-2), to satisfy Invariant 1.
                ;
            }
        }
        Q = next_Q;
        equiv_classes = next_equiv_classes;
    }
    

}

// Scale and translate sites to make thier coordinates satisfy the following:
// (1) distance between them is not smaller than 1.
// (2) they do not have integer coordinate.
void C_Subdivision::init_sites(Point src, std::vector<Point> obs_pts){
    this->sites.push_back(Site(src,SRC));
    for(auto p : obs_pts){
        this->sites.push_back(Site(p,OBS));
    }

    // Compute the minimum x-distance (y-distance) between sites 
    double min_xdist = std::numeric_limits<double>::infinity();
    double min_ydist = min_xdist;

    std::sort(this->sites.begin(), this->sites.end(),[](Site &a, Site &b){return a.getx()<b.getx();});
    for(int i = 0; i<this->sites.size()-1;i++){
        double d = this->sites[i+1].getx()-this->sites[i].getx();
        if (d < min_xdist) min_xdist = d;
    }

    std::sort(this->sites.begin(), this->sites.end(),[](Site &a, Site &b){return a.gety()<b.gety();});
    for(int i = 0; i<this->sites.size()-1;i++){
        double d = this->sites[i+1].gety()-this->sites[i].gety();
        if (d < min_ydist) min_ydist = d;
    }
    
    // scaling to make the distance between points be larger than 1
    double min_dist = std::min(min_xdist, min_ydist);
    scale_factor = 1.;
    if(min_dist < 1) scale_factor = 1/min_dist;
    
    //translate each coordinate value so that they don't have integer coordinates; 

    double min_x= std::numeric_limits<double>::infinity();
    double min_y= min_x;
    double min_xdist_bound= std::numeric_limits<double>::infinity();
    double min_ydist_bound= min_xdist_bound;

    for(auto site: this->sites){
        double x = site.getx(), y = site.gety();
        double xdist_bound = x-std::floor(x);
        double ydist_bound = y-std::floor(y);
        min_xdist_bound = xdist_bound < min_xdist_bound ? xdist_bound:min_xdist_bound;
        min_ydist_bound = ydist_bound < min_ydist_bound ? ydist_bound:min_ydist_bound;
        min_x = x < min_x ? x :min_x;
        min_y = y < min_y ? y : min_y;
    }
    tr_x_factor = -min_x+min_xdist_bound/2;
    tr_y_factor = -min_y+min_ydist_bound/2;

    for(auto site:this->sites){
        double x = site.getx(), y = site.gety();
        site.setx(x*scale_factor + tr_x_factor);
        site.sety(y*scale_factor + tr_y_factor);
    }
}

inline int find_index_order(double x,int ord){
    for(int i = 0;i<ord;i++) x /= 2;
    if(x<0) return int(x)-1;
    return int(x);
}

//Compute a 0-quad whose core has a site in its upper-left 0-box.
std::vector<Quad*> C_Subdivision::init_quads(){
    std::vector<Quad*> Q;

    //collects 0-quads containing sites
    for(auto p : this->sites){
        double x = p.getx(), y = p.gety();
        int r = find_index_order(x,0), c= find_index_order(y,0);
        Quad* q= new Quad(r-1,c-1,0,true); 
        Q.push_back(q);
    }
    return Q;
}
C_Subdivision::~C_Subdivision(){}