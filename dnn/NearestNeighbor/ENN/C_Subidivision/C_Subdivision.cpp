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

    int size = S.size();
    if (size == 0) return S;

    // Adjacency matrix for Quads 
    std::vector<std::vector<bool>> graph(size, std::vector<bool>(size, false));
    int ord = S[0]->ord;

    // Check every pair of Quads if they intersects
    for(int i=0; i<size-1; i++){
        for(int j=i+1; j<size; j++){
               
            Quad* quadA = S[i];
            Quad* quadB = S[j];

            int rA = quadA -> r;
            int cA = quadA -> c;
            int rB = quadB -> r;
            int cB = quadB -> c; 
            
            //std::cout <<"Test (" << quadA->r <<", " <<quadA->c << ") and (" <<quadB->r <<", "<<quadB->c<<")" << std::endl;
            

            if ((std::abs(rA - rB) <= 4) && (std::abs(cA-cB) <=4)){
                //std::cout <<"Graph edge between (" << quadA->r <<", " <<quadA->c << ") and (" <<quadB->r <<", "<<quadB->c<<")" << std::endl;
                graph[i][j] = true;
                graph[j][i] = true;
            } 
        }
    }

    // Compute a maximal matching in the graph computed above.
    std::vector<bool> matched(size, false);
    for(int i=0; i<size; i++){
        if (matched[i]) continue;

        for(int j=0; j<size; j++){
            if (!graph[i][j]) continue;
            if (matched[j]) continue;

            // Try to match with the first neighbor hasn't been matched yet 
            Quad* quadA = S[i];
            Quad* quadB = S[j];

            int rA = quadA -> r;
            int cA = quadA -> c;
            int rB = quadB -> r;
            int cB = quadB -> c;

            std::vector<int> candRA = {int(std::ceil(rA/4 -2)), int(std::floor(rA/4-1))};
            std::vector<int> candCA = {int(std::ceil(cA/4 +1)), int(std::floor(cA/4+2))};
            std::vector<int> candRB = {int(std::ceil(rB/4 -2)), int(std::floor(rB/4-1))};
            std::vector<int> candCB = {int(std::ceil(cB/4 +1)), int(std::floor(cB/4+2))};

            std::vector<int> candR, candC;
            std::set_intersection(candRA.begin(), candRA.end(), candRB.begin(), candRB.end(), std::back_inserter(candR));
            std::set_intersection(candCA.begin(), candCA.end(), candCB.begin(), candCB.end(), std::back_inserter(candC));

            if (candR.empty() || candC.empty()) continue;

            int newR = candR[0];
            int newC = candC[0];

            Quad *newQ = new Quad(newR, newC, ord+2, false); // Delete? 

            quadA->growth = newQ;
            quadB->growth = newQ;

            ret.push_back(newQ); 
            // Stop
            matched[j] = true;
            matched[i] = true;
            break;
        }
    }

    // For unmatched Quads, compute growth(q)
    for(int i=0; i<size; i++){
        if(matched[i]) continue;

        // Compute a new (i+2) quad 
        Quad* q = S[i];
        int newR = int(std::ceil(q->r/4 -2));
        int newC = int(std::ceil(q->c/4 +1));

        Quad * newQ = new Quad(newR, newC, ord+2, false);
        q->growth = newQ;
        ret.push_back(newQ);
    }
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
    while (Q.size()>1){
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

grid_graph::grid_graph(){}
grid_graph::~grid_graph(){
    for(auto ptr:this->grid_pts) delete ptr;
}

//if there does not exist such point, the function insert a new grid point in x_list and y_list
//return a pointer of a new grid point if it was inserted, otherwise return null pointer.

grid_graph::grid_point* grid_graph::add_grid_point(int x,int y){
    list_header header_x_cmp(x), header_y_cmp(y);
    point_container point_x_cmp(x,false), point_y_cmp(y,false);
    grid_point *new_ptr =nullptr;

    auto it_x = this->x_list.insert(header_x_cmp).first;

    auto prev_it_x = it_x->list.lower_bound(point_y_cmp);
    auto in_ret_x = it_x->list.insert(point_y_cmp);
    
    if(in_ret_x.second){ //element is newly inserted
        new_ptr = new grid_point(x,y,this->grid_pts.size());
        this->grid_pts.push_back(new_ptr);
        in_ret_x.first->ptr = new_ptr;

        if(prev_it_x!=it_x->list.begin()){  //Maintain a connectivity between grid points.
            prev_it_x--;
            in_ret_x.first->next_incident = prev_it_x->next_incident;
        }
    }

    auto it_y = this->y_list.insert(header_y_cmp).first;

    auto prev_it_y = it_y->list.lower_bound(point_x_cmp);
    auto in_ret_y = it_y->list.insert(point_x_cmp);

    if(new_ptr){    //Then a new grid point was inserted in the previous.
        in_ret_y.first->ptr = new_ptr;
        
        if(prev_it_y!=it_y->list.begin()){  //Maintain a connectivity between grid points.
            prev_it_y--;
            in_ret_y.first->next_incident = prev_it_y->next_incident;
        }
    }

    return new_ptr;

    // auto it_x = this->x_list.lower_bound(header_x_cmp);
    // bool is_found = false;

    // if(it_x != this->x_list.end() && *it_x == header_x_cmp){
    //     //Find an element in x_list whose key is x
    //     point_container point_cmp(y,false);
    //     auto it_y = it_x->list.lower_bound(point_cmp);
    //     if(it_y != it_x->list.end() && *it_y == point_cmp) 
    //         is_found = true;    //the grid point (x,y) already exists. 
    //     else{
    //         grid_point *new_pt = new grid_point(x,y);
    //         point_cmp.ptr = new_pt;
    //         it_x->list.insert(point_cmp);
    //     }
    // }
    // else{
    //     auto it_x =(this->x_list.insert(header_x_cmp)).first;
    //     point_container point_cmp(y,false);
    //     grid_point *new_pt = new grid_point(x,y);
    //     point_cmp.ptr = new_pt;
    //     it_x->list.insert(point_cmp);
    // }
    // return !is_found;

}

// Connect edge between two grid points (x1,y1) ~ (x2,y2), assuming that the grid points are already inserted. 
// The line through those points is either vertical or horizontal.
// There may be serveral points on the line segment defined those points.
// Then, we carefully connect edges between adjacent points.
void grid_graph::connect(int x1,int y1,int x2,int y2){
    if(x1 == x2){   //vertical edge.
        int min_y = std::min(y1,y2), max_y = std::max(y1,y2);
        list_header header_x(x1);
        auto it_x = this->x_list.find(header_x);
        if(it_x != this->x_list.end()){
            point_container point_min_y(min_y,false), point_max_y(max_y,false);
            auto it_start=it_x->list.find(point_min_y);
            auto it_end = it_x->list.find(point_max_y);
            for(auto it = it_start;it != it_end;it++){
                it->next_incident = true;
            }
        }
    }
    else{           //horizontal edge.
        int min_x = std::min(x1,x2), max_x = std::max(x1,x2);
        list_header header_y(y1);
        auto it_y = this->y_list.find(header_y);
        if(it_y != this->y_list.end()){
            point_container point_min_x(min_x,false), point_max_x(max_x,false);
            auto it_start=it_y->list.find(point_min_x);
            auto it_end = it_y->list.find(point_max_x);
            for(auto it = it_start;it != it_end;it++){
                it->next_incident = true;
            }
        }
    }
}

//Make a graph using drawn edges, where there are edges which are overlapped.
//vertices and adjacency of the graph are filled into "vertices" and "graph" variables, respectively.
void C_Subdivision::build_graph(std::set<Box_Edge>& drawn_edges, std::vector<Point>& vertices, std::vector<std::vector<int> >&graph){
    grid_graph Grid_graph;

    //connect drawn edges in decreasing order. (connect edge of larger order first)
    for(auto it=drawn_edges.end(); it != drawn_edges.begin();){
        --it;
        int ord = it->ord;
        int r = it->r, c = it->c;
        bool is_vertical= it->is_vertical;
        int x1 = r>>ord, y1 = c>>ord;
        int x2 = x1, y2 = y1;
        if(is_vertical) y2 += 1>>ord;
        else x2 += 1 >>ord;
        
        Grid_graph.add_grid_point(x1,y1);
        Grid_graph.add_grid_point(x2,y2);
        Grid_graph.connect(x1,y1,x2,y2);
    }

    //Set the information of vertices
    for(auto ptr:Grid_graph.grid_pts)
        vertices.push_back(Point(ptr->x,ptr->y));

    //Set adjacency of the graph.
    graph.resize(vertices.size(),std::vector<int>());
    for(auto x_list :Grid_graph.x_list){
        for(auto it=x_list.list.begin();it != x_list.list.end();){
            int org_idx = it->ptr->idx;
            bool incident = it->next_incident;
            it++;
            if(incident){  // There is a vertical edge.
                int dest_idx = it->ptr->idx;
                graph[org_idx].push_back(dest_idx);
                graph[dest_idx].push_back(org_idx);
            }
        }
    }
    for(auto y_list :Grid_graph.y_list){
        for(auto it=y_list.list.begin();it != y_list.list.end();){
            int org_idx = it->ptr->idx;
            bool incident = it->next_incident;
            it++;
            if(incident){  // There is a vertical edge.
                int dest_idx = it->ptr->idx;
                graph[org_idx].push_back(dest_idx);
                graph[dest_idx].push_back(org_idx);
            }
        }
    }

    return;
}

//Build strong d-conforming subdivision and the output is stored as the set of drawn edges.
//Return the output as DCEL.
DCEL C_Subdivision::build_d_subdivision(int d){
    //1. Build strong 1-conforming subdivision
    std::set<Box_Edge> drawn_edges;
    this->draw_one_subdivision(drawn_edges);
    
    //TEST
    // for(auto e:drawn_edges){
    //     std::cout <<"drawn_edges"<<'('<<e.ord<<e.is_vertical <<"): ("<<e.r <<','<<e.c <<")("<<e.r + 1- e.is_vertical<<','<<e.c + e.is_vertical<<")"<<std::endl;
    // }

    //2. Make a graph using the drawn edges from 1-conforming subdivision.
    std::vector<Point> vertices;
    std::vector<std::vector<int> > graph;
    
    this->build_graph(drawn_edges,vertices,graph);

    //3. Subdivide each edge of the graph into d' equal-length pieces, 
    //where d' is the ceiling of d. 
    //TEST
    // std::cout<< vertices.size()<<std::endl;
    // for(int i = 0;i<graph.size();i++){
    //     std::cout << i<< vertices[i]<<": ";
    //     for(auto u:graph[i]) std::cout <<u<<' ';
    //     std::cout<<std::endl;
    // }
    return DCEL(vertices, graph);
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