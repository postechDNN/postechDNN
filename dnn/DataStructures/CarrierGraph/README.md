# Class CarrierGraph v1.1
CarrierGraph class is a class for a carrier graph explained in "Orthogonal Shortest Route Queries Among Axis Parallel Rectangular Obstacles" by Elgindy & Mitra. 
Carrier graph is only constructed by axis-parallel rectangles and points, and it does not support insertion/deletion of edges not related to the rectangles. 
CarrierGraph stores points and edges. 
An original carrier graph is DAG (Directly Acyclic Graph), but present version represents this graph as undirected graph. 
CarrierGraph consists of several classes. To see the manual of each classes, check out *\<classname\>.md* from this folder.

## Classes
[Rect](Rect.md)  
[CarrierGraph](CarrierGraph.md)  

## Dependencies in postechDNN
DataStructures/DCEL/Point.h  
DataStructures/DCEL/Edge.h

### Setup the project
None
