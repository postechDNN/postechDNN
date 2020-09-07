# Class SimplePolygon v1.2

## Remarks
This class for simple polygon without holes

## Dependencies in postechDNN
None

## Members
|Private Variables||
|:---|:---|
|[n_vertices](#n_vertices)|Stores the number of vertices.|
|[vertices](#vertices)|Stores vectices of polygon in order of clockwise.|
|[edges](#edges)|Stores edges of polygon in order of vectices order.|

|Constructors & Destructors||
|:---|:---|
|[SimplePolygon(std::vector<Point>&, bool)](#Edge)|Construct a simple polygon with clockwise vertices.|
|[SimplePolygon(std::vector<Edge>&)](#Edge-1)|Construct a simple polygon with clockwise edges.|
|[~SimplePolygon](#~SimplePolygon)|Delete a simple polygon.|

|Getters & Setters||
|:---|:---|
|[getVertices](#getVertices)|Returns ```vertices``` vector.|
|[getEdges](#getEdges)|Returns ```edges``` vector.|

|Functions||
|:---|:---|
|[inPolygon](#inPolygon)|Returns true if given point lying inside of simple polygon.|


### n_vertices
Stores the number of vertices.
```
int n_vertices;
```
### vertices
Stores vectices of polygon in order of clockwise.
```
std::vector<Point> vertices;
```
### edges
Stores edges of polygon in order of vectices order.
```
std::vector<Edge> edges;
```

### SimplePolygon
Constructs an simple polygon object.
```
SimplePolygon(std::vector<Point>& _v, bool _p = true);
SimplePolygon(std::vector<Edge>& _e);
```

#### Parameters
*_v*  
Point array that sorted in order of clockwise. 

*_p*
If vectices sorted in order of counter clockwise, then input true.

*_e*  
Edge array that sorted in order of clockwise. 

#### Remarks
A constructor with no input will make a Edge object with two zero points.

### \~SimplePolygon
Deletes an Edge object.
```
~SimplePolygon();
```

### getVertices
Returns vectices of simple polygon
```
std::vector<Point> getVertices();
```

### getEdges
Returns edges of simple polygon
```
std::vector<Edge> *getEdges();
```

### inPolygon
Returns true if given point lying inside of simple polygon.
```
int inPolygon(Point _p);
```

#### Parameters
*_p*  
A Point object that determine whether lying inside of polygon.
