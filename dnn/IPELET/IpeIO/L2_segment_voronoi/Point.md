# Class Point v1.2

Point class is a class for a 2D point. Point stores its x,y coordinate values in double type.

## Dependencies in postechDNN
None

## Members
|Private Variables||
|:---|:---|
|[pos](#pos)|Stores 2D coordinate(x and y) value of the point|

|Constructors & Destructors||
|:---|:---|
|[Point](#Point)|Construct a Point of origin or a specific position.|
|[~Point](#~Point)|Delete a Point object.|

|Getters & Setters||
|:---|:---|
|[getX](#getX)|Returns ```pos[0]``` value.|
|[setX](#setX)|Sets ```pos[0]``` value.|
|[getY](#getY)|Returns ```pos[1]``` value.|
|[setY](#setY)|Sets ```pos[1]``` value.|
|[setPos](#setPos)|Sets ```pos``` value.|

|Functions||
|:---|:---|
|[operator==](#operator==)|Test if the given points has same coordinates.|
|[operator!=](#operator!=)|Test if the given points has different coordinates.|
|[operator=](#operator=)|Returns a Point that has the same cordinate of the given points.|
|[operator-](#operator-)|Returns a Point that has corrdinate as the difference of the coordinates of the given points.|
|[operator+](#operator+)|Returns a Point that has corrdinate as sum of two points.|
|[operator\*](#operator*)|Returns scalar or vector multification with given value.|
|[operator<](#operator<)|Returns left or bottom point of two points.|
|[dot_product](#dot_product)|Returns value of dot product of two points.|
|[norm](#norm)|Returns normal.|
|[computeDist](#computeDist)|Returns a Euclidean distance of two points.|
|[translation](#translation)|Translates Point with given value.|
|[rotation](#rotation)|Rotates Point with given angle.|
|[ccw](#ccw)|Returns direction of three points.|
|[getMedian](#getMedian)|Returns median point of two points.|


### pos
Stores x-coordinate value and y-coordinate value of the points as double type.
```
double pos[2];
```
### Point
Constructs a Point object.
```
Point();
Point(double _x, double _y);
Point(Point* _p);
```
#### Parameters
*_x*  
x-coordinate value of the point.  

*_y*  
y-coordinate value of the point.  

*_p*  
Point pointer that the Point object will copy its coordinate.

#### Remarks
A constructor with no input will make a Point object with position (0.0, 0.0)

### \~Point
Deletes a Point object.
```
~Point();
```

### getX
Returns x-coordinate value
```
double getX();
```

### setY
Sets x-coordinate value
```
void setX(double _x);
```

#### Parameters
*_x*  
x-coordinate value to be changed.

### getY
Returns y-coordinate value
```
double getY();
```

### setY
Sets y-coordinate value
```
void setY(double _y);
```

#### Parameters
*_y*  
y-coordinate value to be changed.

### setPos
Sets x-coordinate and y-coordinate value
```
void setPos(double _x, double _y);
```

#### Parameters
*_x*  
x-coordinate value to be changed.

*_y*  
y-coordinate value to be changed.

### operator==
Checks if two Points have same position
```
bool operator==(Point _p);
```

#### Parameters
*_p*  
Point object to check whether is has same position with this object.

#### Remarks
This function depends on *==* operator between *double* type values.

### operator!=
Checks if two Points have different position
```
bool operator!=(Point _p);
```

#### Parameters
*_p*  
Point object to check whether is has different position with this object.

### operator=
Returns a Point that has the same cordinate of the given points.
```
Point operator=(Point _p);
```

#### Parameters
*_p*  
Point object to copy the coordinates.

### operator-
Returns a Point that has corrdinate as the difference of the coordinates of the given points.
```
Point operator-(Point _p);
```

#### Parameters
*_p*  
Point object to calculate the difference of the coordinates.

### operator+
Returns a Point that has corrdinate as sum of two points.
```
Point operator+(Point _p);
```

#### Parameters
*_p*  
Point object to calculate the sum of the coordinates.

### operator*
Returns scalar or vector multification with given value.
```
Point operator*(double _d);
double operator*(Point _p);
```

#### Parameters
*_d*  
Double value to calculate the vector multification.

*_p*  
Point object to calculate the scalar multification.

### operator<
Returns left point. If two points have same x-coordinate, then returns bottom point.
```
bool operator<(Point _p);
```

#### Parameters
*_p*  
Point object to compare position.

### dot_product
Returns value of dot product of two points.
```
double dot_product(Point _p);
```

#### Parameters
*_p*  
Point object to calculate dot product.

### norm
Returns normal.
```
double norm();
```

### computeDist
Returns an Euclidean distance between two Points in double type
```
double computeDist(Point _p);
```

#### Parameters
*_p*  
Point object to calculate the Euclidean distance.

### translation
Translates Point with given value.
```
void translation(double _t[2]);
```

#### Parameters
*_t[2]*  
The double values of translate x-coordinate and y-coordinate.

### rotation
Rotates Point with given angle.
```
void rotation(double _r);
```

#### Parameters
*r*  
The double values of rotate angle(radian value).

### ccw
Returns direction of three points.
```
int ccw(Point _p1, Point _p2);
```

#### Parameters
*_p1*  
first Point object to calculate direction.

*_p2*  
Second Point object to calculate direction.

#### Remarks
Counter clockwise == 1, Colinear == 0, Clockwise == -1

### getMedian
Returns pointer of median Point object of two points.
```
Point* getMedian(Point _p);
```

#### Parameters
*_p*  
Point object to calculate median point.

