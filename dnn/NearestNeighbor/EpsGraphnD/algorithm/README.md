# Heuristic $k$-NNS in polygonal domain of arbitrary dimension

This project is an implementation of the heuristic $k$-NNS in polygonal domain of arbitrary dimension.

In this project, pairwise disjoint $d$-dimensional polytopes are obstacles and the algorithm finds approximate $k$-nearest sites among the input point sites from the query point.
Description of the algorithm can be found [here](https://github.com/postechDNN/postechDNN/tree/NDeps/dnn/NearestNeighbor/EpsGraphnD/algorithm/description.pdf).

## How to use?

The algorithm is maintained with the class **EpsGraphNd**. It is a data structure based on quadtree. It supports a $k$-NN query as follows.

- Node* **EpsGraphNd::build(vector<Point*> _points, int _dim, vector<pair<double, double>> _boundingBox, double _eps, int _depth, EpsGraphNdNode* parent = nullptr)**: construct a quadtree given input sites and polytope vertices.  
- **Node* addPoint(Node* node, Point* point, int maxDepth)**: add a new input site to the data structure
- **Node* deletePoint(Node* node, Point* point)**: delete an input site from the data structure
- **vector<pair<int, int>> buildPointGraphOnQuadTree(EpsGraphNd* quadtree, double absoluteValue, double relativeFactor)**: construct a graph on the quadtree, so that $k$-NN queries are resolved using the graph

The classes **Point** and **CPolytope** can be constructed as follows.
- **Point(std::vector<double> xs)**: Construct the point with coordinate **xs**.
- **CPolytope(int n, std::vector<Point> vers, std::vector< std::vector<int>> facets)**: Construct the **n**-dimentional polytope with **vers** as vertices and **facets** as facets.

## Before use it 
### Setup the project
none
### Dependent project
none