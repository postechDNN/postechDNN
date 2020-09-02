# Class Edge v1.1
## Dependencies in postechDNN

## Members
|Protected Variables||
|:---|:---|
|[s](#s)|Stores an endpoint of Edge.|
|[t](#t)|Stores an endpoint of Edge.|

|Constructors & Destructors||
|:---|:---|
|[Edge(void)](#Edge)|Construct an Edge of endpoints (0.0,0.0), (0.0,0.0).|
|[Edge(Point*, Point*)](#Edge-1)|Using two endpoints, construct an Edge.|
|[Edge(Edge*)](#Edge-2)|Construct an Edge which is same with parameter.|
|[~Edge](#Edge-3)|Delete an Edge.|

|Getters & Setters||
|:---|:---|
|[gets](#gets)|Returns ```s``` endpoint.|
|[sets](#sets)|Sets ```s``` endpoint.|
|[gett](#gett)|Returns ```t``` endpoint.|
|[sett](#sett)|Returns ```t``` endpoint.|

|Functions||
|:---|:---|
|[operator==](#operator)|Return true if two edge have same endpoints.|
|[on](#on)|Return true if a point lies on an edge.|
|[crossing](#crossing)|Return an intersection of two edges.|
|[length](#length)|Return an Euclidean length of an edge.|

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
Edge(Edge *_e);
```

#### Parameters
*_s*  
Pointer of the point object which is an endpoint of edge. 

*_t*  
Pointer of the point object which is an endpoint of edge.

*_e*  
Pointer of the edge object which is copied to the new edge object.

#### Remarks
A constructor with no input will make a Edge object with two zero points.

### ~Edge
Deletes an Edge object.
```
~Edge();
```

### gets
Returns one of endpoints.
```
Point *gets();
```

### sets
Sets one of endpoints.
```
void sets(Point *_p);
```

#### Parameters
*_p*  
A pointer of point to be stored as endpoint.

### gett
Returns one of endpoints.
```
Point *gett();
```

### sett
Sets one of endpoints.
```
void sett(Point *_p);
```

#### Parameters
*_p*  
A pointer of point to be stored as endpoint.

### operator ==
Return true if two edges have same endpoints.
```
bool operator==(Edge _e);
```

#### Parameters
*_e*  
An edge to be compared.

### on
Return true if a point lies on an edge.
```
bool on(Point *p);
```

#### Parameters
*p*  
A point object to be tested.

### crossing
Return an intersection of two edges.
```
Point *crossing(Edge *_e, bool closed = true);
```

#### Parameters
*_e*  
An edge to be determined whether it intersects with this object. 

*closed*  
If it is true, two edges are closed. Otherwise they are open.

#### Remarks
Default of *closed* is true. 

### length
Return an Euclidean length of an edge.
```
double length();
```
