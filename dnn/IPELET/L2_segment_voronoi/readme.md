# L2 Segment Voronoi Diagram for IPELET

## Classes and Library
[Point](Point.md)  
[Edge](Edge.md)  
[SimplePolygon](SimplePolygon.md)  
[PolygonClipping](PolygonClipping.md)  
[Segment](Segment.md)  
[SVD_Function](SVD_Function.md)  

## Summary
You can draw L2 segment voronoi diagram with IPELET

## How to use
[Download IPELET](https://www.softpedia.com/get/Office-tools/Other-Office-Tools/Ipe.shtml)  
Draw segments more than 2 with IPELET  
Select segments and run Ipelets->L2SegVoronoi

## How to compile
Please read [readme](../readme.txt) of IPEIO

## Implement Issues

### Algorithms
Compute all of L2 segment voronoi cell of pairs of segments.(Brute force algorithm)  
Compute intersection of voronoi cells of each segment.(Polygon intersection)  

### time complexity
Let ```n``` denote the number of input segments and ```k``` be the complexity of a voronoi cell.  
Brute force algorithm can be computed in O(kn^2).  
Polygon intersections can be computed in O(k^2n).  
Total time complexity is O(kn(k+n)).  
