# Class Polygon

# Members

|Public Variables||
|:---|:---|
|[vers](#vers)|stores vertices of the polygon in clockwise order.|
|[x_min](#x_min), [x_max](#x_max), [y_min](#y_min), [y_max](#y_max)|stores the minimum/maximum x-/y-coordinate value among Polygon vertices.|
|[encl_pts](#encl_pts)|stores Free_Points and Grid_Points that are enclosed by the Polygon.|
|[ord](#ord)|stores the unique number that indicates the Polygon.|

|Constructors||
|:---|:---|
|[Polygon](#Polygon)|produces a Polygon with given set of vertices.|

|Functions||
|:---|:---|
|[intersect](#intersect)|checks if a Polygon edge crosses the edge between two adjacent grid points.|
|[ray](#ray)|shoots a rightward ray from the given point. computes the sum of # of intersections with the Polygon.|

### vers
stores vertices of the polygon in clockwise order.
```
vector<Point> vers;
```

### x_min
stores the minimum x-coordinate value among Polygon vertices.
```
double x_min;
```

### x_max
stores the maximum x-coordinate value among Polygon vertices.
```
double x_max;
```

### y_min
stores the minimum y-coordinate value among Polygon vertices.
```
double y_min;
```

### y_max
stores the maximum y-coordinate value among Polygon vertices.
```
double y_max;
```

### encl_pts
stores Free_Points and Grid_Points that are enclosed by the Polygon.
```
vector<Point> encl_pts;
```

### ord
stores the unique number that indicates the Polygon.
```
int ord;
```

### Polygon
produces a Polygon with given set of vertices.
```
Polygon();
Polygon(vector<Point> _vers, int _ord)
```

### intersect
checks if a Polygon edge crosses the edge between two adjacent grid points.
```
bool intersect(Point p1, Point p2, bool direc)
```

### ray
shoots a rightward ray from the given point. computes the sum of # of intersections with the Polygon.
```
int ray(Point p);
```
