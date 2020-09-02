# Class Point v1.1

Point class is a class for a 2D point. Point stores its x,y coordinate values in double type.

## Dependencies in postechDNN
None

## Members
|Protected Variables||
|:---|:---|
|[x](#x)|Stores x-coordinate value of the point|
|[y](#y)|Stores y-coordinate value of the point|

|Constructors & Destructors||
|:---|:---|
|[Point](#point)|Construct a Point of origin or a specific position.|
|[~Point](#point-1)|Delete a Point object.|

|Getters & Setters||
|:---|:---|
|[getx](#getx)|Returns ```x``` value.|
|[setx](#setx)|Sets ```x``` value.|
|[gety](#gety)|Returns ```y``` value.|
|[sety](#sety)|Sets ```y``` value.|

|Functions||
|:---|:---|
|[operator==](#operator)|Test if the given points has same coordinates.|
|[operator-](#operator--1)|Returns a Point that has corrdinate as the difference of the coordinates of the given points.|
|[distance](#distance)|Returns a Euclidean distance of two points.|

### x
Stores x-coordinate value of the points as double type.
```
double x;
```
### y
Stores y-coordinate value of the points as double type.
```
double y;
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

### ~Point
Deletes a Point object.
```
~Point();
```

### getx
Returns x-coordinate value
```
double getx();
```

### setx
Sets x-coordinate value
```
void setx(double _x);
```

#### Parameters
*_x*  
x-coordinate value to be changed.

### gety
Returns y-coordinate value
```
double gety();
```

### sety
Sets y-coordinate value
```
void sety(double _y);
```

#### Parameters
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

### operator-
Returns a Point that has corrdinate as the difference of the coordinates of the given points.
```
Point operator-(Point _p);
```

#### Parameters
*_p*  
Point object to calculate the difference of the coordinates.

### distance
Returns an Euclidean distance between two Points in double type
```
double distance(Point _p);
```

#### Parameters
*_p*  
Point object to calculate the Euclidean distance.
