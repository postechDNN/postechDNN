#pragma once

#include "DCEL/DCEL.h"
#include "DCEL/Point.h"
#include "Disjoint_Set.h"

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
    //getter and setter
    //void set_growth(Quad*);
    //Quad* get_growth();
    //void set_rc(int,int);
    //std::pair<int,int> get_rc();
    //int set_ord(int);
    //int get_ord();
    //void set_growth(Quad*);
    //Quad* get_growth();
    //std::vector<Quad*> get_children();
    //void add_child(Quad*);
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
};

class C_Subdivision{
protected:
    std::vector<Site> sites;
    std::vector<Box_Edge> drawn_edges;
    double scale_factor;
    double tr_x_factor;
    double tr_y_factor; 

    void init_sites(Point, std::vector<Point>);
    std::vector<Quad*> init_quads();
    std::vector<Component > compute_equiv_class(std::vector<Quad*>&);
    std::vector<Quad*> growth(std::vector<Quad*>&);
    void inline draw_n_edges(int,int, int, int, int);
    bool inline is_intersect_quad(Quad*, Quad*);

    public:
    C_Subdivision(Point, std::vector<Point>);
    ~C_Subdivision();
};