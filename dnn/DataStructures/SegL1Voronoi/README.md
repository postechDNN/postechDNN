# Segment $L_1$ Voronoi Diagram

This project is an implementation of a $L_1$ Voronoi diagram when the sites are segments. 
Papadopoulou & Lee. "The $L_\infty$ Voronoi diagram of segments and VLSI applications."

## What is 'SegL1Voronoi'
Import **SegL1Voronoi.h** to use this data structure.  
Voronoi has these elements:
- vector<Site *> site_vector: The list of segment sites.
- vector<Vertex *> vertex_vector: The list of vertices in the voronoi diagram.
- vector<Edge *> edge_vector: The list of edges in the voronoi diagram.
- WaveFront * wavefrontptr: A data structure maintaining the status while constructuing the Voronoi diagram. 
- priority_queue<Event, vector<Event>, greater<Event>> * EventQ: The priority queue maintaining events while constructing the Voronoi diagram. 

Site has these elements:
- float slope: The slope of the segment.
- float c: The y intersept of the segment. 
- pair<float, float> l_endpoint: The left endpoint of the segment. If the segment is vertical, l_endpoint is upper endpoint.
- pair<float, float> r_endpoint: The right endpoint of the segment. If the segment is vertical, l_endpoint is lower endpoint. 


Voronoi supports these kinds of operations below.
- Voronoi(vector<Site *> sv): Create Voronoi class with the sites sv.
- create(): Build the voronoi diagram from the given sites.


## Before use it 
### Setup the project
None

### Dependent project
None