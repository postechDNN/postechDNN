# Class CarrierGraph v1.2
CarrierGraph class is a class for a carrier graph explained in "Orthogonal Shortest Route Queries Among Axis Parallel Rectangular Obstacles" by Elgindy & Mitra.  
CarrierGraph stores points and edges.  
An original carrier graph is DAG (Directly Acyclic Graph), but present version represents this graph as undirected graph.

## Dependencies in postechDNN
DataStructures/DCEL/Edge.h

## Members
|Protected Variables||
|:---|:---|
|[vcnt](#vcnt)|Stores the number of vertices.|
|[ecnt](#ecnt)|Stores the number of edges.|
|[vlist](#vlist)|Stores the pointers of vertices.|
|[elist](#elist)|Stores the pointers of edges.|

|Constructors & Destructors||
|:---|:---|
|[CarrierGraph](#CarrierGraph)|Constructs a carrier graph.|
|[~CarrierGraph](#CarrierGraph-1)|Deletes a carrier graph.|

|Getters||
|:---|:---|

|Functions||
|:---|:---|
|[insertion](#insertion)|Inserts rectangles and points to the carrier graph.|

### vcnt
Stores the number of vertices as int type.
```
int vcnt;
```
### ecnt
Stores the number of edges as int type.
```
int ecnt;
```
### vlist
Stores the pointers of vertices as vector type.
```
std::vector<Point*> vlist;
```
### elist
Stores the pointers of edges as vector type.
```
std::vector<Edge*> elist;
```
### CarrierGraph
Constructs a carrier graph.
```
CarrierGraph();  
CarrierGraph(std::vector<Rect*> rset, std::vector<Point*> pset, int dir);
```
#### Parameters
*rset*  
Vector of pointers of rectangular obstacles

*pset*  
Vector of pointers of data points

*dir*  
Index indicating the direction of the carrier graph to be made  
XPOS = 0: positive x-direction  
XNEG = 1: negative x-direction  
YPOS = 2: positive y-direction  
YPOS = 3: negative y-direction
#### Remarks
A constructor with no input will make an empty graph.
### ~CarrierGraph()
Deletes a carrier graph.
```
~CarrierGraph();
```
### insertion
Inserts rectangles or points to the carrier graph, and return whether insertion is success or not.
```
int insertion(std::vector<Rect*> rset, std::vector<Point*> pset);  
int insertion(std::vector<Rect*> rset);  
int insertion(std::vector<Point*> pset);
```
#### Parameter
*rset*  
Vector of pointers of rectangles to be inserted

*pset*  
Vector of pointers of points to be inserted
