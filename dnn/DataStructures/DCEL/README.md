# Doubly-Connected Edge List(DCEL)
DCEL class is a class for a DCEL(doubly-connected edge list). DCEL stores pointers of its face, half-edge, vertex records in pointer of vector type.
## What is 'DCEL'?
DCEL is a basic data structure to store geometric objects in 2D space. DCEL consists of several classes.  
Import **DCEL.h** to use this data structure. 

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



## Before use it 
### Setup the project
none
### Dependent project
DataStructures/AVLTree