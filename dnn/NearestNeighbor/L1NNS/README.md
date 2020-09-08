# Class Rect v1.1
Rect class is a class for an axis-aligned rectangle.
Rect stores four cooridnates as sides of rectangles, four points as corners, and id to compute operations well.

## Dependencies in postechDNN
DataStructures/DCEL/Point.h

## Members
|Protected Variables||
|:---|:---|
|[l](#l)|Stores the x-coordinate of the left-bottom corner.|
|[r](#r)|Stores the x-coordinate of the right-top corner.|
|[d](#d)|Stores the y-coordinate of the left-bottom corner.|
|[u](#u)|Stores the y-coordinate of the right-top corner.|
|[ld](#ld)|Stores the pointer of the left-bottom corner.|
|[lu](#lu)|Stores the pointer of the left-top corner.|
|[rd](#rd)|Stores the pointer of the right-bottom corner.|
|[ru](#ru)|Stores the pointer of the right-top corner.|
|[id](#id)|Stores id.|

|Constructors & Destructors||
|:---|:---|
|[Rect](#Rect)|Makes a rectangle.|
|[~Rect](#~Rect)|Deletes a rectangle.|

|Getters&Setters||
|:---|:---|
|[getl](#getl)|Returns ```l``` value.|
|[getr](#getr)|Returns ```r``` value.|
|[getd](#getd)|Returns ```d``` value.|
|[getu](#getu)|Returns ```u``` value.|
|[getld](#getld)|Returns ```ld``` pointer.|
|[getlu](#getlu)|Returns ```lu``` pointer.|
|[getrd](#getrd)|Returns ```rd``` pointer.|
|[getru](#getru)|Returns ```ru``` pointer.|
|[getid](#getid)|Returns ```id``` value.|
|[setid](#setid)|Sets ```id``` value.|

|Functions||
|:---|:---|
|[operator==](#operater==)|Tests if two rectangles have same coordinates.|

### l
Stores the x-coordinate of the left-bottom corner as double type.
```
double l;
```
### r
Stores the x-coordinate of the right-top corner as double type.
```
double r;
```
### d
Stores the y-coordinate of the left-bottom corner as double type.
```
double d;
```
### u
Stores the y-coordinate of the right-top corner as double type.
```
double u;
```
### ld
Stores the pointer of the left-bottom corner as Point type.
```
Point* ld;
```
### lu
Stores the pointer of the left-top corner as Point type.
```
Point* lu;
```
### rd
Stores the pointer of the right-bottom corner as Point type.
```
Point* rd;
```
### ru
Stores the pointer of the right-top corner as Point type.
```
Point* ru;
```
### id
Stores id as int type.
```
int id;
```
### Rect
Makes a rectangle.
```
Rect(double _l, double _r, double _d, double _u);
```
#### Parameter
*_l*
x-coordinate of the left-bottom corner
*_r*
x-coordinate of the right-top corner
*_d*
y-coordinate of the left-bottom corner
*_u*
y-coordinate of the right-top corner
### ~Rect
Deletes a rectangle.
```
~Rect();
```
### getl
Returns l value.
```
double getl();
```
### getr
Returns r value.
```
double getr();
```
### getd
Returns d value.
```
double getd();
```
### getu
Returns u value.
```
double getu();
```
### getld
Returns ld pointer.
```
Point* getld();
```
### getlu
Returns lu pointer.
```
Point* getlu();
```
### getrd
Returns rd pointer.
```
Point* getrd();
```
### getru
Returns ru pointer.
```
Point* getru();
```
### getid
Returns id value.
```
int getid();
```
### setid
Sets id value.
```
void setid(int _id);
```
#### Parameter
*_id*
id index
### operator==
Tests if two rectangles have same coordinates.
```
const bool operator==(Rect& _rect);
```
#### Parameter
*_rect*
Rectangle to check whether it has same position with this object or not
#### Remark
This function depends on == operator between *double* type values.