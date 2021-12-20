# Class dDCEL v1.0
dDCEL class is a class for a dDCEL(d-dimensional doubly-connected edge list). dDCEL stores pointers of its face, half-edge, vertex records in pointer of vector type.

## Members
|Private Variables||
|:---|:---|
|[points](#points)|Stores the dPoint objects.|
|[edges](#edges)|Stores the dEdge objects.|
|[faces](#faces)|Stores the dFace objects of .|
|[facets](#facets)|Stores the pointers of faces.|

|Constructors & Destructors||
|:---|:---|
|[dDCEL](#ddcel)|Construct an empty dDCEL.|
|[~dDCEL](#ddcel-1)|Delete a DCEL object.|

|Getters & Setters||
|:---|:---|
|[set_points](#set_points)|Sets ```points``` vector.|
|[set_edges](#set_edges)|Sets ```edges``` vector.|
|[set_faces](#set_faces)|Sets ```faces``` vector.|
|[set_facets](#set_facets)|Sets ```facets``` vector.|
|[getFaces](#get_points)|Returns ```points``` vector.|
|[getFaces](#get_edges)|Returns ```edges``` vector.|
|[getFaces](#get_faces)|Returns ```faces``` vector.|
|[getFaces](#get_facets)|Returns ```facets``` vector.|

### points
Stores the points as pointer of vector type.
```
std::vector<dPoint*>* points;
```
### edges
Stores the edges as pointer of vector type.
```
td::vector<dEdge*>* edges;
```
### faces
Stores the faces as pointer of vector type.
```
std::vector<dFace*>* faces;
```
### facets
Stores the facets as pointer of vector type.
```
std::vector<dFace*>* facets;
```
### dDCEL
Constructs a dDCEL object.
```
dDCEL();
```
### ~dDCEL
Deletes the dDCEL object.
```
~dDCEL();
```

### set_points
Sets pointer of points vector.
```
void set_points(std::vector<dPoint*>* _points);
```
#### Parameters
*_points*  
Pointer of vector that contains the points pointer.
### set_edges
Sets pointer of edges vector.
```
void set_edges(std::vector<dEdge*>* _edges);
```
#### Parameters
*_edges*  
Pointer of vector that contains the edges pointer.
### set_faces
Sets pointer of faces vector.
```
void set_faces(std::vector<dFace*>* _faces);
```
#### Parameters
*_faces*  
Pointer of vector that contains the faces pointer.
### set_facets
Sets pointer of facets vector.
```
void set_facets(std::vector<dFace*>* _facets);
```
#### Parameters
*_facets*  
Pointer of vector that contains the facets pointer.
### get_points
Returns pointer of vector points.
```
auto get_points();
```
### get_edges
Returns pointer of vector edges.
```
auto get_edges()
```
### get_faces
Returns pointer of vector faces.
```
auto get_faces()
```
### get_facets
Returns pointer of vector facets.
```
auto get_facets()
```