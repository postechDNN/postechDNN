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
bool Box_Edge::operator<(const Box_Edge& op) const{
    if(this->ord != op.ord) return this->ord < op.ord;
    if(this->is_vertical != op.is_vertical) return this->is_vertical < op.is_vertical;
    if(this->r != op.r) return this->r < op.r;
    return this->c < op.c;
}
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

void inline C_Subdivision::draw_n_edges(int r,int c, int ord, int is_vertical, int n, std::set<Box_Edge>& drawn_edges){
    int is_horizontal = 1- is_vertical;
    for(int i = 0;i<n;i++)
        drawn_edges.insert(Box_Edge(r+is_horizontal*i, c+is_vertical*i,ord,is_vertical));
}

void inline C_Subdivision::erase_n_edges(int r,int c, int ord, int is_vertical, int n, std::set<Box_Edge>& drawn_edges){
    int is_horizontal = 1- is_vertical;
    for(int i = 0;i<n;i++)
        drawn_edges.erase(Box_Edge(r+is_horizontal*i, c+is_vertical*i,ord,is_vertical));
}

//Process the simple component of order ord which is about to be complex component at the order of ord+2.
void C_Subdivision::process_simple_to_complex(Quad* q, int ord,std::set<Box_Edge> &drawn_edges){
    //the variable "ord" is the order of quad q.
    //Draw the boundary box of q and subdivide each of its sides into four edges at the (i-2)-order grid lines.
    draw_n_edges(q->r   , q->c  , ord,false,4,drawn_edges);
    draw_n_edges(q->r+4 , q->c  , ord,false,4,drawn_edges);
    draw_n_edges(q->r   , q->c  , ord,true,4,drawn_edges);
    draw_n_edges(q->r   , q->c+4, ord,true,4,drawn_edges);
}

//Process the simple component which is about to be complex component.
void C_Subdivision::process_complex(std::vector<Quad *>& S, std::vector<Quad *>& children_S, int ord,std::set<Box_Edge> &drawn_edges){ 
    //the variable "ord" is the order of quads in S
    //1) Draw i-boxes to fill the region between the boundaries of R_1 and S.
    //1-1) For each quad q of S, draw edges of order i on the boundary of q. 
    for(auto q: S){
        draw_n_edges(q->r   , q->c  ,ord,false,4,drawn_edges);
        draw_n_edges(q->r+4 , q->c  ,ord,false,4,drawn_edges);
        draw_n_edges(q->r   , q->c  ,ord,true,4,drawn_edges);
        draw_n_edges(q->r   , q->c+4,ord,true,4,drawn_edges);
    }
    //1-2) For each quad q of S, erase edges of order i lying on the interior of quads of S.
    for(auto q:S){
        for(int j = 1;j<=3;j++){
            erase_n_edges(q->r+j,q->c,ord,false,4,drawn_edges);
            erase_n_edges(q->r,q->c+j,ord,true,4,drawn_edges);
        }
    }
    //1-3) For each qaud q of S, break each i-box with an endpoint incident to the core of q into four edges of length 2^(i-2).
    for(auto q:S){
        for(int j = 1;j<=3;j++){
            draw_n_edges(4*q->r + 4*j   , 4*q->c       ,ord-2,false,4,drawn_edges);
            draw_n_edges(4*q->r + 4*j   , 4*q->c+ 12   ,ord-2,false,4,drawn_edges);
            draw_n_edges(4*q->r         , 4*q->c+ 4*j  ,ord-2,true,4,drawn_edges);
            draw_n_edges(4*q->r + 12    , 4*q->c+ 4*j  ,ord-2,true,4,drawn_edges);
        }
    }
    //1-4) For each quad q of S, erase edges of order i-2 lying on R1.
    for(auto q:S){
        for(int j = 1;j<=3;j++){
            erase_n_edges(4*q->r + 4*j  , 4*q->c + 4    ,ord-2,false,8,drawn_edges);
            erase_n_edges(4*q->r + 4    , 4*q->c+ 4*j   ,ord-2,true,8,drawn_edges);
        }
    }

    //2) Draw (i-2)-boxes to fill the region between the boundaries of R_1 and R_2.
    //2-1) Fill (i-2) boxes in R1
    for(auto q: S){
        for(int j = 0; j<= 8;j++){
            draw_n_edges(4*q->r+4+j, 4*q->c+4,ord-2,false,8,drawn_edges);   
            draw_n_edges(4*q->r+4, 4*q->c+4+j,ord-2,true,8,drawn_edges);    
        }
    }
    //2-2) Delete (i-2) boxes in R2
    for(auto q: children_S){
        for(int j=1;j<=3;j++){
            erase_n_edges(q->r+j,q->c,ord-2,false,4,drawn_edges);              
            erase_n_edges(q->r,q->c+j,ord-2,true,4,drawn_edges);              
        }
    }
}
//Build strong 1-conforming subdivision and the output is stored as the set of drawn edges.
void C_Subdivision::draw_one_subdivision(std::set<Box_Edge> &drawn_edges){
    //Initialize the set of quads.
    std::vector<Quad*> Q =init_quads();
    std::vector<Component > equiv_classes = compute_equiv_class(Q);
    int i = 0;
    //build-subdivision.
    while (!Q.empty()){
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
            if(q->is_simple && !q_bar->is_simple)
                process_simple_to_complex(q,i-2,drawn_edges);
        }

        //Step 4. Process complex components.
        for(auto S:next_equiv_classes){
            std::vector<Quad *> children_S; // denoted by S' in the original paper.
            for(auto q: S){
                auto children_q = q->children;
                children_S.insert(children_S.end(),children_q.begin(),children_q.end());
            } 
            if(children_S.size() > 1)  // S is complex
                process_complex(S,children_S,i,drawn_edges);
        }
        //TODO: release the memory assigned to quads.
        Q = next_Q;
        equiv_classes = next_equiv_classes;
    }
}

//Make a graph using drawn edges, where there are edges which are overlapped.
//vertices and adjacency of the graph are filled into "vertices" and "graph" variables, respectively.
void C_Subdivision::build_graph(std::set<Box_Edge>& drawn_edges, std::vector<Point>& vertices, std::vector<std::vector<int> >&graph){
    ;//TODO
}

//Build strong d-conforming subdivision and the output is stored as the set of drawn edges.
//Return the output as DCEL.
DCEL C_Subdivision::build_d_subdivision(int d){
    //1. Build strong 1-conforming subdivision
    std::set<Box_Edge> drawn_edges;
    this->draw_one_subdivision(drawn_edges);
    
    //TEST
    for(auto e:drawn_edges){
        std::cout <<"drawn_edges"<<e.ord <<' '<<e.r <<' '<<e.c <<' '<<e.is_vertical<<std::endl;
    }

    //2. Make a graph using the drawn edges from 1-conforming subdivision.
    std::vector<Point> vertices;
    std::vector<std::vector<int> > graph;
    this->build_graph(drawn_edges,vertices,graph);

    //3. Subdivide each edge of the graph into d' equal-length pieces, 
    //where d' is the ceiling of d. 
    //TODO
    
    DCEL dcel;
    dcel.build_by_graph(vertices, graph);
    return dcel;
}

// Scale and translate sites to make thier coordinates satisfy the following:
// (1) distance between them is not smaller than 1.
// (2) they do not have integer coordinate. 
C_Subdivision::C_Subdivision(Point src, std::vector<Point> obs_pts){
    //init_sites(src, obs_pts);
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
        min_x = x < min_x ? x :min_x;
        min_y = y < min_y ? y : min_y;
    }

    for(auto site: this->sites){
        double x = site.getx(), y = site.gety();
        double xdist_bound = std::floor(x-min_x+1.)-(x-min_x);
        double ydist_bound = std::floor(y-min_y+1.)-(y-min_y);
        min_xdist_bound = xdist_bound < min_xdist_bound ? xdist_bound:min_xdist_bound;
        min_ydist_bound = ydist_bound < min_ydist_bound ? ydist_bound:min_ydist_bound;

    }
    tr_x_factor = -min_x+min_xdist_bound/2;
    tr_y_factor = -min_y+min_ydist_bound/2;
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
        double x = p.getx()*this->scale_factor + this->tr_x_factor;
        double y = p.gety()*this->scale_factor + this->tr_y_factor;
        int r = find_index_order(x,0), c= find_index_order(y,0);
        Quad* q= new Quad(r-1,c-1,0,true); 
        Q.push_back(q);
    }
    return Q;
}

C_Subdivision::~C_Subdivision(){}