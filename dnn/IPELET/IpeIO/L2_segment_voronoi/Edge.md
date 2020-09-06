# Class Edge v1.2
## Dependencies in postechDNN
None

## Members
|Protected Variables||
|:---|:---|
|[s](#s)|Stores an endpoint of Edge.|
|[t](#t)|Stores an endpoint of Edge.|

|Constructors & Destructors||
|:---|:---|
|[Edge()](#Edge)|Construct an Edge.|
|[Edge(Point*, Point*)](#Edge-1)|Using two endpoints, construct an Edge.|
|[~Edge](#~Edge)|Delete an Edge.|

|Getters & Setters||
|:---|:---|
|[getOrigin](#getOrigin)|Returns ```s``` endpoint.|
|[getDest](#getDest)|Returns ```t``` endpoint.|

|Functions||
|:---|:---|
|[operator==](#operator==)|Return true if two edge have same endpoints.|
|[operator<](#operator<)|Returns true if edge left of the given edge.|
|[intersect](#intersect)|Returns true if two edges are intersect.|
|[getDir](#getDir)|Returns direction of edge.|
|[turn_direction](#turn_direction)|Turns direction of edge with given point.|


### s
Stores the endpoint of edge.
```
Point *s;
```
### t
Stores the endpoint of edge.
```
Point *t;
```

### Edge
Constructs an edge object.
```
Edge();
Edge(Point *_s, Point *_t);
```

#### Parameters
*_s*  
Pointer of the point object which is an endpoint of edge. 

*_t*  
Pointer of the point object which is an endpoint of edge.

#### Remarks
A constructor with no input will make a Edge object with two zero points.

### \~Edge
Deletes an Edge object.
```
~Edge();
```

### getOrigin
Returns one of endpoints.
```
Point *getOrigin();
```

### getDest
Returns one of endpoints.
```
Point *getDest();
```

### operator ==
Returns true if two edges have same endpoints.
```
bool operator==(Edge& _e);
```

#### Parameters
*_e*  
An edge to be compared.

### operator<
Returns true if edge left of the given edge.
```
bool operator<(Edge& _e);
```

#### Parameters
*_e*  
An edge to be compared.

### intersect
Returns true if two edges are intersect.
```
bool intersect(Edge& _e, double* _ix, double* _iy);
```

#### Parameters
*_ix*  
An address of stores x-coordinate of intersection point.

*_iy*  
An address of stores y-coordinate of intersection point.

*_e*  
An edge to be compared.

### getDir
Returns direction of edge.
```
Point getDir();
```

### turn_direction
Turns direction of edge with given point.
```
double turn_direction(Point _p);
```

#### Parameters
*_p*  
A contorl point of turning direction.