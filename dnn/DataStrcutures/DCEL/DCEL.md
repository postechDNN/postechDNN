# Class DCEL v1.1
DCEL class is a class for a DCEL(doubly-connected edge list). DCEL stores pointers of its face, half-edge, vertex records in pointer of vector type.
## Dependencies in postechDNN
Class Point, Edge
## Members
|Protected Variables||
|:---|:---|
|[num_faces](#num_faces)|Stores the number of faces.|
|[num_hedges](#num_hedges)|Stores the number of half-edges.|
|[num_vertices](#num_verticecs)|Stores the number of vertices.|
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
|[inPolygon](#inPolygon)|Return 1 if given point is in the polygon defined by given hedges, 
0 if the point is on the boundary of the polygon, -1 otherwise.|
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
```
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
```
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
```
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
```
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
```
### mergeDCEL
```
DCEL* mergeDCEL(DCEL*);
```
#### Parameters

#### Remarks
```
### searchHedge
Return the hedge object with the given key.
```
HEdge* searchHedge(char* key);
```
#### Parameters
*key*  

#### Remarks
```
### searchVertex
Return the vertex object with the given key.
```
Vertex* searchVertex(char* key);
```
#### Parameters
*key*  

#### Remarks
```
### searchFace
Return the face object with the given key.
```
Face* searchFace(char* key);
```
#### Parameters
*key*  

#### Remarks
```

### getOutgoingHEdges
Return a vector containing all hedge objects that has given vertex as origin.
```
std::vector<HEdge*> getOutgoingHEdges(Vertex* _v);
```
#### Parameters
*_v*
Pointer of the vertex object to compute all half-edges that has it as origin.

#### Remarks

```

### getIncomingHEdges
Return a vector containing all hedge objects whose twin has given vertex as origin.
```
std::vector<HEdge*> getIncomingHEdges(Vertex* _v);
```
#### Parameters
*_v*
Pointer of the vertex object to compute all half-edges whose twin has it as origin.

#### Remarks


|[inPolygon](#inPolygon)|Return 1 if given point is in the polygon defined by given hedges, 
0 if the point is on the boundary of the polygon, -1 otherwise.|
|[DCELtotext](#DCELtotext)|Create a text file of DCEL.|
|[printVertexTab](#printVertexTab)|Print the vertex table of DCEL.|
|[printHedgeTab](#printHedgeTab)|Print the hedge table of DCEL.|
|[printFaceTab](#printFaceTab)|Print the face table of DCEL.|
