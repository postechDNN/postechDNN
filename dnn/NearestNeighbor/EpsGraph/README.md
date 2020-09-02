# Epsilon-Graph(EG)

## About EG
It is a graph-based structure that supports dynamic approximate nearest neighbor queries in the plane.  
To be concrete, given background data (P, O, ɛ), it returns k approximate nearest neighbors of p∈P for a given query (p, k), where  
  P := set of points in the plane,  
  O := set of polygonal obstacles, and  
  ɛ := sidelength of orthogonal grid cells.
  
## The algorithm 
An orthogonal square grid enclosing P∪O is constructed.  

To check if grid edges are present or not, the algorithm counts the number of intersections between each polygon and rightward rays originating from each gridpoint.
If the number of intersections between a gridpoint and a polygon is an odd number, the gridpoint is enclosed by the polygon and hence isolated.
The other case that can happen is when both of two contiguous polygon edges forming an acute internal angle cross a grid edge, 
forcing the number of intersections between the polygon and two gridpoints regarding the edge to be different.  

Then, each point in P casts 'anchor' on each grid point, so that a nearest neighbor query of a point returns points that are anchored to nearby gridpoints which are obtained by implementing BFS on the grid. 

## Related works
There is no corresponding paper.
