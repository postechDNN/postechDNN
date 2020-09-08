# Class AVLTree v1.1
AVLTree class is a balanced binary search tree data structure which can store various types of datas. Operator '=', '<', and '=='  must be defined for type that stores the datas. This library can be widely used on implementing computational geometry techniques such as line-sweep algorithms.
## Dependencies in postechDNN
None
## AVLTreeNode
AVLTreeNode is a structure that used to implement AVLTree. AVLTreeNode stores a data value, rank, parent node and two children node. If there are no such node(s), AVLTreeNode stores nullptr.

## Members
|Protected Variables||
|:---|:---|
|[root](#root)|Stores a root node|

|Constructors & Destructors||
|:---|:---|
|[AVLTree](#AVLTree)|Construct an empty AVLTree object.|
|[~AVLTree](#AVLTree-1)|Delete an AVLTree object.|


|Functions||
|:---|:---|
|[isEmpty](#isEmpty)|Return if the AVLTree is empty.|
|[insert](#insert)|Insert a node to the AVLTree.|
|[getRoot](#getRoot)|Return root node.|
|[getLeftNode](#getLeftNode)|Return a node that stores the biggest data which is smaller than the given data.|
|[getRightNode](#getRightNode)|Return a node that stores the smallest data which is larger than the given data.|
|[pop](#pop)|Pop and return a node.|

### root
Stores a root node as AVLTreeNode pointer. If the tree is empty, it stores nullptr.
```
AVLTreeNode<T>* root;
```
### AVLTree
Constructs an empty AVLTree object.
```
AVLTree<T>();
```
#### Remarks
root will be initialized as nullptr.

### ~AVLTree
Deletes a AVLTree object.
```
~AVLTree<T>();
```
### isEmpty
Returns true when the AVLTree is empty.
```
bool isEmpty();
```
### insert
Insert an data to AVLTree via data or AVLTreeNode
```
void insert(T data);
void insert(AVLTreeNode<T>* datanode)
```
#### Parameters
*T data*  
Data to insert.  
*AVLTreeNode<T>* datanode*  
AVLTreeNode that stores data to insert.

#### Remarks
As a data is inserted to AVLTree, AVLTree automatically is sorted by the datas and is balanced.

### getRoot
Return *root*.
```
AVLTreeNode<T>* getRoot();
```

### getLeftNode
Return a node that stores the largest data smaller than the given data. If there is no such node, return nullptr.
```
AVLTreeNode<T>* getLeftNode(T data);
```

#### Parameter
*T data*  
Data to compare.


### getRightNode
Return a node that stores the smallest data larger than the given data. If there is no such node, return nullptr.
```
AVLTreeNode<T>* getLeftNode(T data);
```

#### Parameter
*T data*  
Data to compare.

### pop
Pop a node and return. If a parameter is given, return the node contains the data. Otherwise, return the node with the smallest data.If there is no such node, return nullptr.
```
AVLTreeNode<T>* pop();
AVLTreeNode<T>* pop(T data);
```

#### Parameter
*T data*  
Data to return.

