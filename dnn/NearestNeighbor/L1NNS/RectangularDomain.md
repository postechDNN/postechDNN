# Class RectangularDomain v1.1
RectangularDomain class is a class for above rectangular domain.  
RectangularDomain simply stores rectangles and points in vector type.

## Dependencies in postechDNN
DataStructures/DCEL/Point.h  
DataStructures/DCEL/Edge.h

## Members
|Protected Variables||
|:---|:---|
|[obscnt](#obscnt)|Stores the number of rectangular obstacles.|
|[datacnt](#datacnt)|Stores the number of data points.|
|[obstacles](#obstacles)|Stores the pointers of rectangular obstacles.|
|[data](#data)|Stores the pointers of data points.|
|[bbox](#bbox)|Stores the pointer of the bounding box.|

|Constructors & Destructors||
|:---|:---|
|[RectangularDomain](#RectangularDomain)|Constructs a rectangular domain.|
|[~RectangularDomain](#~RectangularDomain)|Deletes a rectangular domain.|

|Getters||
|:---|:---|
|[getobscnt](#getobscnt)|Returns ```obscnt```.|
|[getdatacnt](#getdatacnt)|Returns ```datacnt```.|
|[getobstacles](#getobstacles)|Returns ```obstacles``` vector.|
|[getdata](#getdata)|Returns ```data``` vector.|
|[getobstacle](#getobstacle)|Returns i-th of ```obstacles``` vector.|
|[getdatum](#getdatum)|Returns i-th of ```data``` vector.|
|[getbbox](#getbbox)|Returns ```bbox```.|

|Functions||
|:---|:---|
|[NNS](#NNS)|Finds the nearest neighbor pointer among data points for given query.|
|[FNS](#FNS)|Finds the farthest neighbor pointer among data points for given query.|
|[kNNS](#kNNS)|Finds k-nearest neighbor pointers among data points for given query.|
|[insertion](#insertion)|Inserts obstacles and data to the rectangular domain.|
|[deletion](#deletion)|Deletes obstacles and data from the rectangular domain.|

### obscnt
Stores the number of rectangular obstacles as int type.
```
int obscnt;
```
### datacnt
Stores the number of data points as int type.
```
int datacnt;
```
### obstacles
Stores the pointers of rectangular obstacles as vector type.
```
std::vector<Rect*> obstacles;
```
### data
Stores the pointers of data points as vector type.
```
std::vector<Point*> data;
```
### bbox
Stores the pointer of a bounding box as pointer of Rect type.
```
Rect* bbox;
```
### RectangularDomain
Constructs a rectangular domain with a large bounding box.
```
RectangularDomain();  
RectangularDomain(std::vector<Rect*> rset, std::vector<Point*> pset);
```
#### Parameters
*rset*  
Vector of pointers of rectangular obstacles

*pset*  
Vector of pointers of data points

*readfile*  
Text file containing several two coordinates(data points) or four coordinates(rectangular obstacles)
#### Remarks
A constructor with no input will make a rectangular domain with only a large bounding box.
### ~RectangularDomain()
Deletes a rectangular domain.
```
~RectangularDomain();
```
### getobscnt
Returns the number of obstacles.
```
int getobscnt();
```
### getdatacnt
Returns the number of data.
```
int getdatacnt();
```
### getobstacles
Returns vector of pointers of all obstacles.
```
std::vector<Rect*> getobstacles();
```
### getdata
Returns vector of pointers of all data.
```
std::vector<Point*> getdata();
```
### getobstacle
Returns i-th pointer of obstacles.
```
Rect* getobstacle(int i);
```
#### Parameter
*i*  
index
### getdatum
Returns i-th pointer of data.
```
Point* getdatum(int i);
```
#### Parameter
*i*  
index
### getbbox
Returns the pointer of bounding box.
```
Rect* getbbox();
```
### NNS
Finds the nearest neighbor pointer among data points for given query.
```
Point* NNS(Point* q);
```
#### Parameter
*q*
Pointer of point as a given query
### FNS
Finds the farthest neighbor pointer among data points for given query.
```
Point* FNS(Point* q);
```
#### Parameter
*q*  
Pointer of point as a given query
### kNNS
Finds k-nearest neighbor pointers among data points for given query.
```
Point* FNS(Point* q, int k);
```
#### Parameter
*q*  
Pointer of point as a given query

*k*  
integer value for finding *k*-NNS
### insertion
Inserts obstacles and data to the rectangular domain, and return whether insertion is success or not.
```
int insertion(std::vector<Rect*> rset, std::vector<Point*> pset);  
int insertion(std::vector<Rect*> rset);  
int insertion(std::vector<Point*> pset);
```
#### Parameter
*rset*  
Vector of pointers of rectangular obstacles to be inserted

*pset*  
Vector of pointers of data points to be inserted
### deletion
Deletes obstacles and data from the rectangular domain, and return whether deletion is success or not.
```
int deletion(std::vector<Rect*> rset, std::vector<Point*> pset);  
int deletion(std::vector<Rect*> rset);  
int deletion(std::vector<Point*> pset);  
```
#### Parameter
*rset*  
Vector of pointers of rectangular obstacles to be deleted

*pset*  
Vector of pointers of data points to be deleted