# Struct indices

|Public Variables||
|:---|:---|
|[row](#row), [column](#column)|store the row and column number of a Grid_Point on the Epsilon-Graph, respectively.|

### row
stores the row number of a Grid_Point on the Epsilon-Graph.
```
int row;
```

### column
stores the column number of a Grid_Point on the Epsilon-Graph.
```
int column;
```

# Struct incid_pts

|Public Variables||
|:---|:---|
|[right](#right), [lower](#lower), [left](#left), [upper](#upper)|tells if a Grid_Point is incident to its right/lower/left/upper neighbor Grid_Point on the Epsilon-Graph.|

### right
tells if a Grid_Point is incident to its right neighbor Grid_Point on the Epsilon-Graph. If its right neighbor Grid_Point doesn't exist, right = false.
```
bool right;
```

### lower
tells if a Grid_Point is incident to its lower neighbor Grid_Point on the Epsilon-Graph. If its right neighbor Grid_Point doesn't exist, lower = false.
```
bool lower;
```

### left
tells if a Grid_Point is incident to its left neighbor Grid_Point on the Epsilon-Graph. If its right neighbor Grid_Point doesn't exist, left = false.
```
bool left;
```

### upper
tells if a Grid_Point is incident to its upper neighbor Grid_Point on the Epsilon-Graph. If its right neighbor Grid_Point doesn't exist, upper = false.
```
bool upper;
```

# Class Point

## Members

|Public Variables||
|:---|:---|
|[x](#x), [y](#y)|store the x- and y- coordinate of the Point, respectively.|
|[encl](#encl)|stores the ord(order) of the polygon that encloses the Point.|

|Constructor||
|:---|:---|
|[Point](#Point)|produces a Point with a given x- and y-coordinate.|

|Functions||
|:---|:---|
|[operator==](#operator==)|checks if the Point coincides with another Point.|
|[print](#print)|prints the x- and y-coordinate of the Point.|


### x
stores the x-coordinate of the Point.
```
double x;
```

### y
stores the y-coordinate of the Point.
```
double y;
```

### encl
stores the ord(order) of the polygon that encloses the Point.
In other words, the Point lies on the interior of the polygon.
If there is no such polygon, encl = -1.
```
int encl;
```

### Point
produces a Point with a given x- and y-coordinate.
```
Point();
Point(double _x, double _y);
```
#### Remarks
The constructor with no input produces a Point (0, 0).

### operator==
checks if the Point coincides with another Point.
```
bool operator==(const Point& other) const;
```

### print
prints the x- and y-coordinate of the Point.
```
void print();
```

# Class Free_Point : public Point

|Public Variables||
|:---|:---|
|[host](#host)|stores the index of the Grid_Point to which the Free_Point anchors.|

### host
stores the index of the Grid_Point to which the Free_Point anchors. If such Grid_Point does not exist, host = -1.
```
int host;
```

|Constructor||
|:---|:---|
|[Free_Point](#Free_Point)|produces a Free_Point with a given x- and y-coordinate.|

### Free_Point
produces a Free_Point with a given x- and y-coordinate.
```
Free_Point();
Free_Point(double _x, double _y);
```
#### Remarks
The constructor with no input produces a Free_Point (0, 0).

# Class Grid_Point : public Point

|Public Variables||
|:---|:---|
|[ind](#ind)|stores the row and column number of the Grid_Point on the Epsilon-Graph.|
|[num](#num)|stores the index of the Grid_Point on the Epsilon-Graph. makes one-to-one correspondence with [ind](#ind).|
|[ip](#ip)|tells if the Grid_Point is incident to incident gridpoints on the Epsilon-Graph.|
|[anchored](#anchored)|stores the Free_Points anchored to it.|
|[cros](#cros)|stores # of crossings of each polygon with the ray moving rightward and originating from the Grid_Point.|

### ind
stores the row and column number of the Grid_Point on the Epsilon-Graph.
```
indices ind;
```

### num
stores the index of the Grid_Point on the Epsilon-Graph. makes one-to-one correspondence with [ind](#ind).
```
int num;
```

### ip
tells if the Grid_Point is incident to incident gridpoints on the Epsilon-Graph.
```
incid_pts ip;
```

### anchored
stores the [Free_Point](#Free_Point)s anchored to it.
```
vector<Free_Point*> anchored;
```

### cros
stores # of crossings of each polygon with the ray moving rightward and originating from the Grid_Point.
```
vector<int> cros;
```

|Constructor||
|:---|:---|
|[Grid_Point](#Grid_Point)|produces a Grid_Point on the Epsilon-Graph, with given row and column number.|

### Grid_Point
produces a Grid_Point(vertex) on the Epsilon-Graph, with given row and column number.
```
Grid_Point();
Grid_Point(int _row_num, int _col_num, double x_min, double y_max, double eps, int eg_colnum);
```
#### Remarks
The constructor with no input produces a Grid_Point (0, 0).
