# Class Vector v1.1

Voint class is a class for a 2D vector. Vector stores its x,y values in double type. It inherits from the class Point

## Dependencies in postechDNN
Class Point

## Members
|Protected Variables||
|:---|:---|
|[x](#x)|Stores x value of the vector|
|[y](#y)|Stores y value of the vector|

|Constructors & Destructors||
|:---|:---|
|[Vector(void)](#vector)|Construct a zero vector|
|[Vector(double, double)](#vector-1)|Construct a vector using x,y values|
|[Vector(Point)](#vector-2)|Construct a vector whose x,y values are same with parameter|
|[Vector(Point, Point)](#vector-3)|Construct a vector using difference of vectors|
|[~Vector](#vector-4)|Delete a Vector object.|

|Getters & Setters||
|:---|:---|
|[getx](#getx)|Returns ```x``` value.|
|[setx](#setx)|Sets ```x``` value.|
|[gety](#gety)|Returns ```y``` value.|
|[sety](#sety)|Sets ```y``` value.|

|Functions||
|:---|:---|
|[operator==](#operator)|Test if the given vectors has same x,y values.|
|[operator-](#operator--1)|Returns a vector which is difference of two vectors.|
|[norm(void)](#norm)|Returns a Euclidean norm of vector.|
|[innerProduct(Vector)](#innerProduct)|Return an inner product of two vectors.|
|[outerProduct(Vector)](#outerProduct)|Return an outer product of two vectors.|

### x
Stores x value of the vector as double type.
```
double x;
```
### y
Stores y value of the vector as double type.
```
double y;
```
### Vector
Constructs a Vector object.
```
Vector();
Vector(double _x, double _y);
Vector(Point _p);
Vector(Point _s, Point _t);
```
#### Parameters
*_x*  
x value of the vector.  

*_y*  
y value of the vector.  

*_p*  
Point that the Vector object will copy its coordinate.

*_s*
Point that the Vector object use it as origination for construction

*_t*
Point that the Vector object use it as destination for construction

#### Remarks
A constructor with no input will make a Vector object with (0.0, 0.0)

### ~Vector
Deletes a Vector object.
```
~Vector();
```

### getx
Returns x value
```
double getx();
```

### setx
Sets x value
```
void setx(double _x);
```

#### Parameters
*_x*  
x value to be changed.

### gety
Returns y value
```
double gety();
```

### sety
Sets y value
```
void sety(double _y);
```

#### Parameters
*_y*  
y value to be changed.

### operator==
Checks if two Vectors are same. Error tolerance is 1e-6
```
bool operator==(Vector _v);
```

#### Parameters
*_v*  
Vector object to check whether is has same with this object.

#### Remarks
This function depends on *==* operator between *double* type values.
Because of imprecision of *double* type, we tolerate error up to 1e-6.

### operator-
Returns a Vector which is the difference of two given vectors.
```
Vector operator-(Vector _v);
```

#### Parameters
*_v*  
Vector object to calculate the difference.

### norm
Returns an Euclidean norm of a vector in double type
```
double norm();
```

### innerProduct
Calculate an inner product of two vectors in double type.
```
double innerProduct(Vector _v);
```

#### Parameters
*_v*
Vector object to calculate the inner product

### outerProduct
Calculate an outer product of two vectors in Vector object.
```
Vector outerProduct(Vector _v);
```

#### Parameters
*_v*
Vector object to calculate the outer product
