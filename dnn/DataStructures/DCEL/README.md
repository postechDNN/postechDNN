# Doubly-Connected Edge List(DCEL)
DCEL class is a class for a DCEL(doubly-connected edge list). DCEL stores pointers of its face, half-edge, vertex records in pointer of vector type.
## What is 'DCEL'?
DCEL is a basic data structure to store geometric objects in 2D space. DCEL consists of several classes.
## Dependencies in postechDNN
DataStructures/AVLTree
## Members
|Protected Variables||
|:---|:---|
|[num_faces](#num_faces)|Stores the number of faces.|
|[num_hedges](#num_hedges)|Stores the number of half-edges.|
|[num_vertices](#num_vertices)|Stores the number of vertices.|
|[faces](#faces)|Stores the pointers of faces.|
|[hedges](#hedges)|Stores the pointers of hedges.|
|[faces](#vertices)|Stores the pointers of vertices.|
|[lmost](#lmost)|Stores the pointer of the left-most vertex.|
|[tmost](#tmost)|Stores the pointer of the top-most vertex.|
|[bmost](#bmost)|Stores the pointer of the bottom-most vertex.|
|[rmost](#rmost)|Stores the pointer of the right-most vertex.|

|Constructors & Destructors||
|:---|:---|
|[DCEL](#DCEL)|Construct a DCEL of one outer face or of a dcel file.|
|[~DCEL](#DCEL-1)|Delete a DCEL object.|

|Getters & Setters||
|:---|:---|
|[getFaces](#getFaces)|Returns ```faces``` vector.|
|[setFaces](#setFaces)|Sets ```faces``` vector.|
|[getHedges](#getHedges)|Returns ```hedges``` vector.|
|[setHedges](#setHedges)|Sets ```hedges``` vector.|
|[getVertices](#getVertices)|Returns ```vertices``` vector.|
|[setVertices](#setVertices)|Sets ```vertices``` vector.|
|[getLmost](#getLmost)|Returns ```lmost``` vertex.|
|[getRmost](#getRmost)|Returns ```rmost``` vertex.|
|[getTmost](#getTmost)|Returns ```tmost``` vertex.|
|[getBmost](#getBmost)|Returns ```bmost``` vertex.|

|Functions||
|:---|:---|
|[addEdge](#addEdge)|Add the edge that connects the given vertices.|
|[deleteEdge](#deleteEdge)|Delete the given edge.|
|[mergeDCEL](#mergeDCEL)||
|[searchHedge](#searchHedge)|Return the hedge object with the given key.|
|[searchVertex](#searchVertex)|Return the vertex object with the given key.|
|[searchFace](#searchFace)|Return the face object with the given key.|
|[getOutgoingHEdges](#getOutgoingHEdges)|Return a vector containing all hedge objects that has given vertex as origin.|
|[getIncomingHEdges](#getIncomingHEdges)|Return a vector containing all hedge objects whose twin has given vertex as origin.|
|[inPolygon](#inPolygon)|Return 1 if given point is in the simple polygon defined by given hedges, 0 if the point is on the boundary of the simple polygon, -1 otherwise.|
|[DCELtotext](#DCELtotext)|Create a text file of DCEL.|
|[printVertexTab](#printVertexTab)|Print the vertex table of DCEL.|
|[printHedgeTab](#printHedgeTab)|Print the hedge table of DCEL.|
|[printFaceTab](#printFaceTab)|Print the face table of DCEL.|

### num_faces
Stores the number of faces as int type.
```
int num_faces;
```
### num_hedges
Stores the number of hedges as int type.
```
int num_hedges;
```
### num_vertices
Stores the number of vertices as int type.
```
int num_vertices;
```
### faces
Stores the pointer of faces as pointer of vector type.
```
std::vector<Face*>* faces;
```
### hedges
Stores the pointer of hedges as pointer of vector type.
```
std::vector<HEdge*>* hedges;
```
### vertices
Stores the pointer of vertices as pointer of vector type.
```
std::vector<Vector*>* vertices;
```
### lmost
Stores the left-most vertex as pointer of Vertex type.
```
Vertex* lmost;
```
### tmost
Stores the top-most vertex as pointer of Vertex type.
```
Vertex* tmost;
```
### bmost
Stores the bottom-most vertex as pointer of Vertex type.
```
Vertex* bmost;
```
### rmost
Stores the right-most vertex as pointer of Vertex type.
```
Vertex* rmost;
```
### DCEL
Constructs a DCEL object.
```
DCEL();
DCEL(FILE* readFile);
```
#### Parameters
*readFile*  
Text file of the DCEL.  
#### Remarks
A constructor with no input will make a DCEL object with one outer-face(of).
### ~DCEL
Deletes a DCEL object.
```
~DCEL();
```
### getFaces
Returns pointer of vector faces.
```
std::vector<Face*>* getFaces();
```
### setFaces
Sets faces.
```
void setFaces(std::vector<Face*>* _f);
```
#### Parameters
*_f*  
Pointer of vector that contains the face records.
### getHedges
Returns pointer of vector hedges.
```
std::vector<Hedge*>* getHedges();
```
### setHedges
Sets hedges.
```
void setHedges(std::vector<HEdge*>* _h);
```
#### Parameters
*_h*  
Pointer of vector that contains the hedge records.
### getVertices
Returns pointer of vector vertices.
```
std::vector<Vertex*>* getVertices();
```
### setVertices
Sets vertices.
```
void setVertices(std::vector<Vertex*>* _v);
```
#### Parameters
*_v*  
Pointer of vector that contains the vertex records.
### getLmost
Returns pointer of the left-most vertex.
```
Vertex* getLmost();
```
### getRmost
Returns pointer of the right-most vertex.
```
Vertex* getRmost();
```
### getTmost
Returns pointer of the top-most vertex.
```
Vertex* getTmost();
```
### getBmost
Returns pointer of the bottom-most vertex.
```
Vertex* getBmost();
```
### addEdge
Add the edge that connects the given vertices.
```
void addEdge(Vertex* _v1, Vertex* _v2);
```
#### Parameters
*_v1*  
Pointer of vertex object that connects one side of the added edge.
*_v2*  
Pointer of vertex object that connects the other side of the added edge.
#### Remarks
This function only receives vertiecs that are already in the member variable *vertices* of this object. 
### deleteEdge
Delete the given edge.
```
void deleteEdge(HEdge* _e);
```
#### Parameters
*_e*  
Pointer of the desired edge.
#### Remarks
This function only deletes edges that are already in the member variable *hedges* of this object.
Even though it receives one half-edge, it will also delete its twin.
### mergeDCEL
```
DCEL* mergeDCEL(DCEL*);
```
#### Parameters

#### Remarks

### searchHedge
Return the hedge object with the given key.
```
HEdge* searchHedge(char* key);
```
#### Parameters
*key*  

#### Remarks

### searchVertex
Return the vertex object with the given key.
```
Vertex* searchVertex(char* key);
```
#### Parameters
*key*  

#### Remarks

### searchFace
Return the face object with the given key.
```
Face* searchFace(char* key);
```
#### Parameters
*key*  

#### Remarks


### getOutgoingHEdges
Return a vector containing all hedge objects that has given vertex as origin.
```
std::vector<HEdge*> getOutgoingHEdges(Vertex* _v);
```
#### Parameters
*_v*
Pointer of the vertex object to compute all half-edges that has it as origin.

#### Remarks


### getIncomingHEdges
Return a vector containing all hedge objects whose twin has given vertex as origin.
```
std::vector<HEdge*> getIncomingHEdges(Vertex* _v);
```
#### Parameters
*_v*
Pointer of the vertex object to compute all half-edges whose twin has it as origin.

#### Remarks

### inPolygon
Return 1 if given point is in the polygon defined by given hedges, 0 if the point is on the boundary of the simple polygon, -1 otherwise.
```
int inPolygon(std::vector<HEdge*>* hedges, Point p);
```
#### Parameters
*hedges*
Pointer of the vector that contains pointers of half-edges defining the simple polygon.
*p*
Point object to compute whether it is in the simple polygon or not.

#### Remarks
*hedges* should be a non-intersecting, continuous polygonal chain. That is, the edges should be given in the order that it appears in the boundary of the simple polygon.

### DCELtotext
Create a text file of DCEL.
```
void DCELtotext(FILE* readfile);
```
#### Parameters
*readfile*

#### Remarks

### printVertexTab
Print the vertex table of DCEL.
```
void printVertexTab();
```
#### Remarks

### printHedgeTab
Print the half-edge table of DCEL.
```
void printHedgeTab();
```
#### Remarks
This function prints all half-edges, including the twins.
### printFaceTab
Print the face table of DCEL.
```
void printFaceTab();
```


# Class Vertex v1.1
## Dependencies in postechDNN
Class Point
## Members
|Protected Variables||
|:---|:---|
|[vertex_key](#vertex_key)|Stores ID of Vertex.|
|[incidentEdge](#incidentEdge)|Stores half edge whose origin is this vertex.|

|Constructors & Destructors||
|:---|:---|
|[Vertex(void)](#Vertex)|Construct a Vertex of (0,0).|
|[Vertex(Point*)](#Vertex-1)|Construct a Vertex whose coordinate is same with a point|
|[~Vertex](#Vertex-2)|Delete a DCEL object.|

|Getters & Setters||
|:---|:---|
|[getVertexKey](#getVertexKey)|Returns ```vertex_key```.|
|[setVertexKey](#setVertexKey)|Sets ```vertex_key```.|
|[getIncidentEdge](#getIncidentEdge)|Returns ```incidentEdge```.|
|[setIncidentEdge](#setIncidentEdge)|Sets ```incidentEdge```.|
|[getx](#getx)|Returns ```x``` coordinate.|
|[setx](#setx)|Sets ```x``` coordinate.|
|[gety](#gety)|Returns ```y``` coordinate.|
|[sety](#sety)|Returns ```y``` coordinate.|

|Functions||
|:---|:---|
|||

### x
Stores the x-coordinate of vertex.
```
double x;
```
### y
Stores the y-coordinate of vertex.
```
double y;
```
### vertex_key
Stores ID of vertex in an array of char
```
char *vertex_key;
```
### incidentEdge
Stores a half edge whose origin is this vertex.
```
HEdge *incidentEdge;
```

### Vertex
Constructs a vertex object.
```
Vertex();
Vertex(Point* _p);
```

#### Parameters
*_p* 
Pointer of the point object to copy its coordinate to a vertex object.

#### Remarks
A constructor with no input will make a Vertex object with (0.0, 0.0).

### ~Vertex
Deletes a Vertex object with its ID.
```
~Vertex();
```

### getx
Returns x-coordinate value.
```
double getx();
```

### setx
Sets x-coordinate value.
```
double setx(double _x);
```

#### Parameters
*_x*  
x-coordinate value to be changed.

### gety
Returns y-coordinate value.
```
double gety();
```

### sety
Sets y-coordinate value.
```
double sety();
```

#### Parameters
*_y*  
y-coordinate value to be changed.

### getVertexKey
Returns vertex ID.
```
char *getVertexKey();
```

### setVertexKey
Allocates memeory to store Vertex ID and sets ID.
```
void setVertexKey(char *_k)
```
#### Parameters
*_k*  
An array of char to be copied to vertex ID.

### getIncidentEdge
Returns the pointer of half edge whose origin is this vertex.
```
HEdge *getIncidentEdge()
```

### setIncidentEdge
Sets half edge to the incident edge of this vertex.
```
void setIncidentEdge(HEdge *_e)
```

#### Parameters
*_e*  
A pointer of half edge which has this vertex as origin.

# Class HEdge v1.1
## Dependencies in postechDNN
Class Edge
## Members
|Protected Variables||
|:---|:---|
|[hedge_key](#hedge_key)|Stores ID of Half Edge.|
|[origin](#origin)|Stores origin vertex of Half Edge.|
|[next](#next)|Stores next Half Edge.|
|[prev](#prev)|Stores previous Half Edge.|
|[twin](#twin)|Stores twin Half Edge.|
|[incidentFace](#incidentFace)|Stores face which is left to Half Edge.|

|Constructors & Destructors||
|:---|:---|
|[HEdge(void)](#Hedge)|Construct a Half Edge object.|
|[HEdge(Vertex*, Vertex*)](#Hedge-1)|Construct a Half Edge object using two existing vertices.|
|[HEdge(Point*, Point*)](#Hedge-2)|Construct a Half Edge object using two points.|
|[~HEdge](#Hedge-3)|Delete a Half Edge object.|

|Getters & Setters||
|:---|:---|
|[getHedgeKey](#getHedgeKey)|Returns ```hedge_key```.|
|[setHedgeKey](#setHedgeKey)|Sets ```hedge_key```.|
|[getOrigin](#getOrigin)|Returns ```origin```.|
|[setOrigin](#setOrigin)|Sets ```origin```.|
|[getNext](#getNext)|Returns ```next```.|
|[setNext](#setNext)|Sets ```next```.|
|[getPrev](#getPrev)|Returns ```prev```.|
|[setPrev](#setPrev)|Sets ```prev```.|
|[getTwin](#getTwin)|Returns ```twin```.|
|[setTwin](#setTwin)|Sets ```twin```.|
|[getIncidentFace](#getIncidentFace)|Returns ```incidentFace```.|
|[setIncidentFace](#setIncidentFace)|Sets ```incidentFace```.|

|Functions||
|:---|:---|
|||

### hedge_key
Stores ID of Half Edge.
```
char* hedge_key;
```

### origin
Stores origin vertex of half edge.
```
Vertex *origin;
```

### next
Stores next half edge.
```
Hedge *next;
```

### prev
Stores previous half edge.
```
Hedge *prev;
```

### twin
Stores twin half edge.
```
Hedge *twin;
```

### incidentFace
Stores face which is left to Half Edge.
```
Face *incidentFace;
```

### HEdge
Construct a Half Edge object.
```
Hedge();
Hedge(Vertex *_v1,Vertex *_v2);
Hedge(Point *_p1, Point *_p2);
```

#### Parameters
*_v1*
A vertex which is origin of Half Edge.
*_v2*  
A vertex which is destination of Half Edge.
*_p1*  
A point which is origin of Half Edge.
*_p2*  
A point which is destination of Half Edge.

#### Remarks
When point parameters are given, it create two vertex objects. 
It create corespoinding twin Half Edge automatically.

### ~HEdge
Delete two Half Edges object.
```
~Hedge()
```

#### Remarks
It delete coresponding twin Half Edge automatically.

### getHedgeKey
Returns ID of Half Edge.
```
char *getHedgeKey();
```

### setHedgeKey
Sets ID of Half Edge.
```
void setHedgeKey(char *_k);
```
#### Parameters
*_k*  
An array of char which has ID of Half Edge.

### getOrigin
Returns origin vertex of Half Edge.
```
Vertex *getOrigin();
```

### setOrigin
Sets origin vertex of Half Edge.
```
void setOrigin(Vertex* _v);
```

#### Parameters
*_v*  
A pointer of vertex to be stored as origin.

### getNext
Returns next Half Edge of it.
```
HEdge *getNext();
```

### setNext
Sets next Half Edge of it.
```
void setNext(HEdge *_e);
```

#### Parameters
*_e*  
A Half Edge to be stored as next half edge.

### getPrev
Returns previous Half Edge of it.
```
HEdge *getPrev();
```

### setPrev
Sets previous Half Edge of it.
```
void setPrev(HEdge *_e);
```
#### Parameters
*_e*  
A Half Edge to be stored as previous half edge.

### getTwin
Returns twin Half Edge of it.
```
HEdge *getTwin();
```

### setTwin
Sets twin Half Edge of it.
```
void setTwin(Hedge *_e);
```

#### Parameters
*_e*  
A Half Edge to be stored as twin Half Edge.

### getIncidentFace
Return a face which is left to the Half Edge.
```
Face *getIncidentFace();
```

### setIncidentFace
Sets a face to be incident face.
```
void setIncidentFace(Face *_f);
```

#### Parameters
*_f*  
A face which is left to the Half Edge.

# Class Face v1.1
## Dependencies in postechDNN
None
## Members
|Protected Variables||
|:---|:---|
|[face_key](#face_key)|Stores ID of Face.|
|[outer](#outer)|Stores one half-edge of the outer boundary.|
|[inners](#inners)|Stores one half-edge for every inner boundary.|

|Constructors & Destructors||
|:---|:---|
|[Face(void)](#Face)|Construct a face object.|
|[~Face](#Face-1)|Delete a face object.|

|Getters & Setters||
|:---|:---|
|[getFaceKey](#getFaceKey)|Returns ```face_key```.|
|[setFaceKey](#setFaceKey)|Sets ```face_key```.|
|[getOuter](#getOuter)|Returns ```outer```.|
|[setOuter](#setOuter)|Sets ```outer```.|
|[getInners](#getInners)|Returns ```inners```.|

|Functions||
|:---|:---|
|[isOutMost](#isOutMost)|Returns true if this face is the out-most face, false otherwise.|
|[addInner](#addInner)|Adds given half-edge to ```inners```.|


### face_key
Stores ID of Face.
```
char* face_key;
```
### outer
Stores one half-edge of the outer boundary.
```
HEdge* outer;
```
### inners
Stores one half-edge for every inner boundary.
```
std::vector<HEdge*>* inners;
```


### Face
Constructs a face object.
```
Face();
```
#### Remarks
A constructor with no input will make a Face object that is a nullptr.

### ~Face
Deletes a Face object.
```
~Face();
```
#### Remarks

### getFaceKey
Returns ```face_key```.
```
char* getFaceKey();
```
### Remarks

### setFaceKey
Sets ```face_key```.
```
void setFaceKey(char* _key);
```
#### Parameters
*_key*   
Face ID to be changed. 
### Remarks


### getOuter
Returns ```outer```.
```
HEdge* getOuter();
```
### Remarks

### setOuter
Sets ```outer```.
```
HEdge* setOuter(HEdge * _e);
```
#### Parameters
*_e*  
Face ID to be changed. 
### Remarks

### getInners
Returns ```inners```
```
std::vector<HEdge*>* getInners();
```
### Remarks
