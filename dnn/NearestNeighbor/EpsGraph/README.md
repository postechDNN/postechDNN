# Epsilon-Graph(EG)

## About EG
It is a graph-based structure that supports dynamic approximate nearest neighbor queries in the plane.  
To be concrete, given background data (P, O, ɛ), it returns k approximate nearest neighbors of p∈P for a given query (p, k), where  
  P := set of points in the plane,  
  O := set of polygonal obstacles, and  
  ɛ := sidelength of orthogonal grid cells.
  
## The algorithm 
An orthogonal square grid enclosing P∪O is constructed.  
To check if grid edges are present or not, the algorithm counts # of intersections between each polygon and rightward rays originating from each gridpoint.  
Then, each point in P casts 'anchor' on each grid point.  
Finally, with the presence of obstacles, the algorithm returns k nearest neighbors obtained by implementing BFS on the grid and  
taking out points anchored to the grid points nearby.

## Related works
There is no corresponding paper.
