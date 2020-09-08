# Struct edge

|Public Variables||
|:---|:---|
|[p1](#p1), [p2](#p2)|represents each endpoint of the edge.|

### p1
represents an endpoint of the edge.
```
Point p1;
```

### p2
represents another endpoint of the edge.
```
Point p2;
```

# Class Eps_Graph

## Members

|Public Variables||
|:---|:---|
|[eps](#eps)|stores the sidelength of a unit square in the Eps_Graph.|
|[x_min](#x_min), [x_max](#x_max), [y_min](#y_min), [y_max](#y_max)|stores the minimum/maximum x-/y-coordinate value among Polygon vertices and Free_Points contained in the Eps_Graph.|
|[upper_left](#upper_left)|stores the upper left Grid_Point of the Eps_Graph.|
|[row_num](#row_num), [col_num](#col_num)|stores the number of rows/columns of the grid graph.|
|[grid](#grid)|stores Grid_Points in the Eps_Graph.|
|[edges](#edges)|stores grid edges.|
|[fr_pts](#fr_pts)|stores Free_Points.|
|[pols](#pols)|stores Polygons as obstacles.|
|[dist](#dist)|stores BFS distances of Grid_Points from a source Grid_Point.|
|[visited](#visited)|tells if BFS algorithm has visited each Grid_Point.|
|[closest](#closest)|stores Grid_Points in the increasing order of BFS distance.|

|Constructors||
|:---|:---|
|[Eps_Graph](#Eps_Graph)|produces a Polygon with given set of vertices.|

|Functions||
|:---|:---|
|[init_grid](#init_grid)|initalizes the Eps_Graph.|
|[get_gridpt](#get_gridpt)|returns the Grid_Point specified by its row and column number.|
|[ind2num](#ind2num)|converts an integer index into matrix-based index.|
|[num2ind](#num2ind)|converts a matrix-based index into an integer index.|
|[add_edge](#add_edge)|adds an edge between two adjacent Grid_Points.|
|[delete_edge](#delete_edge)|deletes an edge between two adjacent Grid_Points.|
|[cmpNadd](#cmpNadd)|checks if the line connecting the gridpoint and its neighboring one is blocked by any Polygon. If it's not, add an edge between them.|
|[cmpNadd_SinPol](#cmpNadd_SinPol)|does [cmpNadd](#cmpNadd) with a single Polygon.|
|[add_freepts](#add_freepts)|adds a Free_Point in the Eps_Graph.|
|[delete_freept](#delete_freept)|deletes a Free_Point in the Eps_Graph.|
|[anchor](#anchor)|finds a nearest Grid_Point that is not enclosed by any Polygon, for a given Free_Point. it also associates each other using pointers.|
|[query_anchor](#query_anchor)|returns the nearest Grid_Point, for a given query Point.|
|[add_pol](#add_pol)|adds a Polygon in the Eps_Graph.|
|[delete_pol](#delete_pol)|deletes a Polygon in the Eps_Graph.|
|[eff_region](#eff_region)|the minimum rectangular grid region enclosing the polygon.|
|[BFS](#BFS)|implements BFS on the grid graph.|
|[kNN](#kNN)|returns the approximate k-Nearest Neighbor for a given query Point.|
|[print_grid](#print_grid)|print the presence of the edge between every adjacent pair of Grid_Points.|
|[print_edges](#print_edges)|print the coordinates of two endpoints, for all existing edges.|
|[print_dist](#print_dist)|print the BFS distances.|
|[print_kNN](#print_kNN)|implements kNN, and prints the result.|

### eps
stores the sidelength of a unit square in the Eps_Graph.
```
double eps;
```

### x_min
stores the minimum x-coordinate value among Polygon vertices and Free_Points contained in the Eps_Graph.
```
double x_min;
```

### x_max
stores the maximum x-coordinate value among Polygon vertices and Free_Points contained in the Eps_Graph.
```
double x_max;
```

### y_min
stores the minimum y-coordinate value among Polygon vertices and Free_Points contained in the Eps_Graph.
```
double y_min;
```

### y_max
stores the maximum y-coordinate value among Polygon vertices and Free_Points contained in the Eps_Graph.
```
double y_max;
```

### upper_left
stores the upper left Grid_Point of the Eps_Graph.
```
Point upper_left;
```

### row_num
stores the number of rows of the grid graph.
```
int row_num;
```

### col_num
stores the number of columns of the grid graph.
```
int col_num;
```

### grid
stores Grid_Points in the Eps_Graph.
```
vector<Grid_Point> grid;
```

### edges
stores grid edges.
```
vector<edge> edges;
```

### fr_pts
stores Free_Points.
```
list<Free_Point> fr_pts;
```

### pols
stores Polygons as obstacles.
```
vector<Polygon> pols;
```

### dist
stores BFS distances of Grid_Points from a source Grid_Point.
```
vector<int> dist;
```

### visited
tells if BFS algorithm has visited each Grid_Point.
```
vector<bool> visited;
```

### closest
stores Grid_Points in the increasing order of BFS distance.
```
vector<int> closest;
```

### Eps_Graph
produces a Polygon with given set of vertices.
```
Eps_Graph();
Eps_Graph(list<Free_Point>, vector<Polygon>, double)
```

### init_grid
initalizes the Eps_Graph.
```
void init_grid();
```

### get_gridpt
returns the Grid_Point specified by its row and column number.
```
Grid_Point get_gridpt(indices);
```

### ind2num
converts an integer index into matrix-based index.
```
int ind2num(indices);
int ind2num(int, int);
```

### num2ind
converts a matrix-based index into an integer index.
```
indices num2ind(int);
```

### add_edge
adds an edge between two adjacent Grid_Points.
```
void add_edge(indices, indices);
void add_edge(int, int, bool);
```

### delete_edge
deletes an edge between two adjacent Grid_Points.
```
void delete_edge(indices, indices);
```

### cmpNadd
checks if the line connecting the gridpoint and its neighboring one is blocked by any Polygon. If it's not, add an edge between them.
```
bool cmpNadd(indices, bool);
```

### cmpNadd_SinPol
does [cmpNadd](#cmpNadd) with a single Polygon.
```
bool cmpNadd(indices, bool, int);
```

### add_freepts
adds a Free_Point in the Eps_Graph.
```
void add_freepts(vector<Free_Point>);
```

### delete_freept
deletes a Free_Point in the Eps_Graph.
```
void delete_freept(int);
```

### anchor
finds a nearest Grid_Point that is not enclosed by any Polygon, for a given Free_Point. it also associates each other using pointers.
```
void anchor(Free_Point&)
```

### query_anchor
returns the nearest Grid_Point, for a given query Point.
```
Grid_Point query_anchor(Free_Point);
```

### add_pol
adds a Polygon in the Eps_Graph.
```
void add_pol(Polygon);
```

### delete_pol
deletes a Polygon in the Eps_Graph.
```
void delete_pol(int);
```

### eff_region
the minimum rectangular grid region enclosing the polygon.
```
indices* eff_region(Polygon);
```

### BFS
implements BFS on the grid graph.
```
void BFS(Grid_Point);
```

### kNN
returns the approximate k-Nearest Neighbor for a given query Point.
```
vector<Free_Point> kNN(Free_Point, int);
```

### print_grid();
print the presence of the edge between every adjacent pair of Grid_Points.
```
void print_grid();
```

### print_edges();
print the coordinates of two endpoints, for all existing edges.
```
void print_edges();
```

### print_dist();
print the BFS distances.
```
void print_dist();
```

### print_kNN();
implements kNN, and prints the result.
```
void print_kNN();
```
