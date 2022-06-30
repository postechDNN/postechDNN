# Class PDNode v1.1
PDNode class is a class to represent a node of PDGraph, which supports slow but accurate nearest neighbor.
## Dependencies in postechDNN
DataStructures/DCEL
## Members
|Protected Variables||
|:---|:---|
|[p](#p)|Stores corresponding point.|
|[site](#site)|Stores whether it is a site or an obstacle vertex.|
|[adjlist](#adjlist)|Stores adjacent vertices in PDGraph.|
|Constructors & Destructors||
|:---|:---|
|[PDNode](#PDNode)|Construct a PDNode.|
|[~PDNode](#PDNode-1)|Delete a PDNode object.|

|Getters & Setters||
|:---|:---|
|[getPoint](#getPoint)|Returns ```p```.|
|[setPoint](#setPoint)|Sets ```p```.|
|[issite](#issite)|Returns ```site```.|
|[setSite](#setSite)|Sets ```site``` vector.|
|[getAdj](#getAdj)|Returns ```adjlist``` vector.|
|[setAdj](#setAdj)|Sets ```adjlist``` vector.|

|Functions||
|:---|:---|
|[operator==](#operator)|Checks whether two nodes is the same point.|
|[addNode](#addNode)|Add a node the ```adjlist```.|

### p
Stores the corresponding point as a Point vector.
```
Point* p;
```
### site
Stores a boolean value whether it is a site or an obstacle vertex.
```
bool site;
```
### adjlist
Stores the adjacent PDNodes in PDGraph by PDNode pointer vector.
```
std::vector<PDNode*>* adjlist;
```
### PDNode
Constructs a PDNode object.
```
PDNode();
PDNode(Point* _p);
PDNode(Point* _p, std::vector<PDNode*>* _adjlist);
```
#### Parameters
*_p*  
Corresponding point.
*_adjlist*
Corresponding adjacent list.
#### Remarks
One can modify both of the variables by getters and setters.
### ~PDNode
Deletes a PDNode object.
```
~PDNode();
```
#### Remarks
Deletion of PDNode will also delete its adjlist.
### getPoint
Returns pointer of corresponding point.
```
Point* getPoint();
```
### setPoint
Sets p.
```
void setPoint(Point* _p);
```
#### Parameters
*_p*  
Pointer of Point to change to.
### issite
Returns site value.
```
bool issite();
```
### setSite
Sets site.
```
void setSite(bool _site);
```
#### Parameters
*_site*  
boolean value to change to.
### getAdj
Returns pointer of adjlist.
```
std::vector<PDNode*>* getAdj();
```
### setAdj
Sets adjlist.
```
void setAdj(std::vector<PDNode*>* _adjlist);
```
#### Parameters
*_adjlist*  
Pointer of PDNode pointer that contains adjacent PDNodes..
### operator==
Checks whether two nodes has same location.
```
bool operator==(PDNode _node);
```
#### Parameters
*_node*  
Node to compare.
#### Remarks
This function works exactly same as operator== in Point class.
### addNode
Add a node to adjlist
```
void addNode(PDNode* _node);
```
#### Parameters
*_node*  
Node to add.
