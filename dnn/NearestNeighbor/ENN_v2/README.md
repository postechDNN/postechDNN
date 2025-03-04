# 2d Exact Nearest Neighbor Search

This project is an implementation of the exact nearest neighbor search algorithm on the plane in the polygonal domain. 
In this project, simple polygons are obstacles and the algorithm find the nearest site among the point sites from the query point. 
The implementation of the algorithm is based on the paper written by Lee(Proximity and reachability in the plane, 1978). 
The algorithm in this paper is explained in the Chapter 15 of the book [Computational Geometry: Algorithms and Applications](https://github.com/morris821028/Computational-Geometry/blob/master/pdf/Computational%20Geometry%20Algorithms%20and%20Applications%2C%203rd%20Ed%20-%20de%20Berg%20et%20al.pdf) written by de Berg et.al.
Also, we are implementing the nearest neighbor search algorithm based on [the paper](https://epubs.siam.org/doi/abs/10.1137/S0097539795289604) written by Hershberger and Suri.

## How to use?

The algorithm is maintained with the class **VisGraph**. **VisGraph** supports these kinds of queries below.
- **VisGraph(const vector<Point>& srcs, const vector<SimplePolygon>& obstacles)**: Construct the query structure using the vector of 2d points **sites**, the vector of simple polygons **obstacles**. 
- **void VisGraph::add_Polygon(SimplePolygon poly)**: Add a polygon **poly** and modify **VisGraph**.
- **void VisGraph::del_Polygon(int i)**: Delete the polygon with index **i** and modify **VisGraph**.
- **void add_vert(Point p)**: Add a site point **p** and modify **VisGraph**.
- **void del_vert(int i)**: Delete the site with index **i** and modify **VisGraph**.
- **tuple<Point, int, double>  query(Point p)**: Find the nearest site **q** from **p** and return **q**, the index of **q** and the distance from p to **q**.

The classes **Point** and **SimplePolygon** can be constructed as follows.
- **Point(double x, double y)**: Construct the point with coordinate **(x,y)**.
- **Box(vector<Point> &v)**: Construct the polygon with the vertices in **v** with the same order. 

## Before use it 
### Setup the project
none
### Dependent project
datastructure/DCEL
