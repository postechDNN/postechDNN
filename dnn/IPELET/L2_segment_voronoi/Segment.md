# Class Segment v1.2
## Dependencies in postechDNN
None

## Members
|Private Variables||
|:---|:---|
|[start](#start)|Stores an endpoint of Segment.|
|[end](#end)|Stores an endpoint of Segment.|
|[slope](#slope)|Stores the slope of Segment.|
|[intercept](#intercept)|Stores the intercept of Segment.|


|Constructors & Destructors||
|:---|:---|
|[Segment()](#Segment)|Construct a Segment.|
|[Segment(Point, Point)](#Segment)|Using two endpoints, construct a Segment.|
|[~Segment](#~Segment)|Delete a Segment.|

|Getters & Setters||
|:---|:---|
|[getStart](#getStart)|Returns ```start``` endpoint.|
|[setStart](#setStart)|Sets ```start``` endpoint.|
|[getEnd](#getDest)|Returns ```end``` endpoint.|
|[setEnd](#setEnd)|Sets ```end``` endpoint.|
|[getSlope](#getSlope)|Returns ```slope``` value.|
|[setSlope](#setSlope)|Sets ```slope``` value.|
|[getIntercept](#getIntercept)|Returns ```intercept``` value.|
|[setIntercept](#setIntercept)|Sets ```intercept``` value.|

|Functions||
|:---|:---|
|[operator=](#operator=)|Returns a Segment object that has the same endpoints of the given segment.|
|[getTop](#getTop)|Returns the top endpoint of segment.|
|[getBottom](#getBottom)|Returns the bottom endpoint of segment.|
|[getLeft](#getLeft)|Returns the left endpoint of segment.|
|[getRight](#getRight)|Returns the right endpoint of segment.|
|[computeSlope](#computeSlope)|Calculate the slope of segment.|
|[computeIntercept](#computeIntercept)|Calculate the intercept of segment.|
|[getBisector](#getBisector)|Returns bisector of two segments.|
|[getPerLine](#getPerLine)|Returns perpendicular line with given object.|
|[getIntersect](#getIntersect)|Returns intersect point of two segments.|
|[getBisecPoint](#getBisecPoint)|Returns bisector point with given point.|
|[getMedian](#getMedian)|Returns median point of two endpoints.|
|[ccw](#ccw)|Returns direction of given point with two endpoints.|
|[isOnSeg](#isOnSeg)|Returns true if given point lying on the segment.|

### start
Stores the endpoint of edge.
```
Point start;
```
### end
Stores the endpoint of edge.
```
Point end;
```
### slope
Stores the slope of edge.
```
double slope;
```
### intercept
Stores the intercept of edge.
```
double intercept;
```

### Segment
Constructs a Segment object.
```
Segment();
Segment(Point _s, Point _t);
```

#### Parameters
*_s*  
Pointer of the point object which is an endpoint of edge. 

*_t*  
Pointer of the point object which is an endpoint of edge.

### \~Segment
Deletes a Segment object.
```
~Segment();
```

### getStart
Returns ```start``` endpoint.
```
Point getStart();
```

### setStart
Sets ```start``` endpoint.
```
void setStart(Point _p);
```

#### Parameters
*_p*  
Point object to be ```start``` endpoint.

### getEnd
Returns ```end``` endpoint.
```
Point getEnd();
```

### setStart
Sets ```end``` endpoint.
```
void setEnd(Point _p);
```

#### Parameters
*_p*  
Point object to be ```end``` endpoint.

### getSlope
Returns ```slope``` value.
```
double getSlope();
```

### setSlope
Sets ```slope``` endpoint.
```
void setSlope(double _d);
```

#### Parameters
*_d*  
Double vlaue to be ```slope``` value.

### getIntercept
Returns ```intercept``` value.
```
double getIntercept();
```

### setIntercept
Sets ```intercept``` endpoint.
```
void setIntercept(double _d);
```

#### Parameters
*_d*  
Double vlaue to be ```intercept``` value.

### operator ==
Returns true if two edges have same endpoints.
```
bool operator==(Edge& _e);
```

#### Parameters
*_e*  
A segment to be compared.

### operator=
Returns a Segment object that has the same endpoints of the given segment.
```
Segment operator=(Segment _s);
```

#### Parameters
*_s*  
A segment to be copy the endpoints, slope and intercept.

### getTop
Returns the top endpoint of segment.
```
Point getTop();
```

### getBottom
Returns the bottom endpoint of segment.
```
Point getBottom();
```

### getLeft
Returns the left endpoint of segment.
```
Point getLeft();
```

### getRight
Returns the right endpoint of segment.
```
Point getRight();
```

### computeSlope
Calculate the slope of segment.
```
void computeSlope();
```

### computeIntercept
Calculate the intercept of segment.
```
void computeIntercept();
```

### getBisector
Returns address of Segment object that stores bisector of two segments.
```
Segment* getBisector(Segment _s);
```

#### Parameters
*_s*  
A Segment object to calculate bisector.

### getPerLine
Returns address of Segment object that stores perpendicular line with given object.
```
Segment* getPerLine(Point _p);
Segment* getPerLine(Segment _s);
```

#### Parameters
*_p*  
A Point object to calculate perpendicular line.

*_s*  
A Segment object to calculate perpendicular line.

### getIntersect
Returns address of Point object that stores intersection point of two segments.
```
Point* getIntersect(Segment _s);
```

#### Parameters
*_s*  
A Segment object to calculate intersection point.

### getBisecPoint
Returns address of Point object that stores bisector point of two segments.
```
Point* getBisecPoint(Segment _s, Point _p);
```

#### Parameters
*_p*  
A Point object to calculate bisector point.

*_s*  
A Segment object to calculate bisector point.

### getMedian
Returns address of Point object that stores median point of two endpoints.
```
Point* getMedian();
```

### ccw
Returns direction of given point with two endpoints.
```
int ccw(Point _p);
```

#### Parameters
*_p*  
A Point object to calculate direction.

### ccw
Returns true if given point lying on the segment.
```
bool isOnSeg(Point _p);
```

#### Parameters
*_p*  
A Point object to determine whether lying on the segment.
