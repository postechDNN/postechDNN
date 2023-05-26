#pragma once

#include "DCEL/DCEL.h"
#include "DCEL/Point.h"
#include "Disjoint_Set.h"
#include <set>

enum Site_type{SRC, OBS};
class Site: public Point{
protected:
    Site_type t;  
public:
    Site();
    Site(Point&, Site_type);
    ~Site();
};

class Quad{
public:
    int r,c;  // indices of left-upper corner (e.g. coordinates of left-upper corner is (r*(2^i), c*(2^i)) )
    int ord;  // the order of quad (e.g. the quad of order i has a side of length 2^(i+2))
    Quad* growth; // growth of this quad as computed in Step 2.
    std::vector<Quad*> children; // small quads each of whose growth is this quad. 
    bool is_simple;

    Quad(int,int,int,bool);
    ~Quad();
};

typedef std::vector<Quad*> Component;
typedef Disjoint_Set<Quad*> Equiv_Class;

//Class of drawn edges
class Box_Edge{
public:
    int r,c;  // indices of left corner (e.g. coordinates of left-upper corner is (r*(2^i), c*(2^i)) )
    int ord;
    bool is_vertical;  // true if it is vertical

    Box_Edge(int,int,int,bool);
    ~Box_Edge();
    bool operator<(const Box_Edge& op) const;
};


class grid_graph{
public:
    struct grid_point{
        int x,y;
        int idx;
        grid_point(int x,int y,int idx):x(x),y(y),idx(idx){}
    };
    struct point_container{
        int key;
        mutable bool next_incident;
        mutable grid_point *ptr;
        point_container(int key = 0, bool next_incident= false, grid_point *ptr = nullptr):key(key), next_incident(next_incident), ptr(ptr){}
        bool operator<(const point_container& op) const{return this->key < op.key;}
        bool operator==(const point_container& op) const{return this->key == op.key;}
    };
    struct list_header{
        int key;
        mutable std::set<point_container> list;
        list_header(int key=0):key(key){}
        bool operator<(const list_header& op) const{return this->key < op.key;}
        bool operator==(const list_header& op) const{return this->key == op.key;}
        //void insert_point_container(point_container& op) {this->list.insert(op);}
    };
    std::set<list_header> x_list,y_list;
    std::vector<grid_point*> grid_pts;

    grid_graph();
    ~grid_graph();

    grid_point* add_grid_point(int,int);
    void connect(int,int,int,int);
};



class C_Subdivision{
public:     //NEED TO CONVERT
    std::vector<Site> sites;
    double scale_factor;
    double tr_x_factor;
    double tr_y_factor;

    //void init_sites(Point, std::vector<Point>);
    std::vector<Quad*> init_quads();
    std::vector<Component > compute_equiv_class(std::vector<Quad*>&);
    std::vector<Quad*> growth(std::vector<Quad*>&);
    void inline draw_n_edges(int,int, int, int, int,std::set<Box_Edge>&);
    void inline erase_n_edges(int,int, int, int, int,std::set<Box_Edge>&);
    bool inline is_intersect_quad(Quad*, Quad*);
    void process_simple_to_complex(Quad*,int,std::set<Box_Edge> &);
    void process_complex(std::vector<Quad *>&,std::vector<Quad *>&,int,std::set<Box_Edge> &);
    void draw_one_subdivision(std::set<Box_Edge>&);
    void build_graph(std::set<Box_Edge>&, std::vector<Point>&, std::vector<std::vector<int>> &);

    public:
    C_Subdivision(Point, std::vector<Point>);
    ~C_Subdivision();
    //TODO
    DCEL build_d_subdivision(int);
};