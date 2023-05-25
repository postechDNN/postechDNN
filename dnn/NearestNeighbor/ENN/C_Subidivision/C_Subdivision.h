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

class C_Subdivision{
protected:
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