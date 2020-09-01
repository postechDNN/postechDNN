# Epsilon Graph

Given a set of points $P$ and a set of polygonal obstacles $O$ in the plane
This project is an implementation of AVLTree which can support various kinds of line-sweep algorithms such as line sweep intersection, constructing point location datastructure, etc.  
There is no corresponding paper.

## What is 'AVLTree'

AVLTree is a data structure which stores data with template type T for each node.  
Operator '=', '<' and '==' must be defined for type T to support AVLTree.  
The datas are stored in increasing order.  
AVLTree has 'AVLTreeNode<T>' struct as its node.  
AVLTreeNode has these elements:
- int rank: store the rank of subtree which has this node as a root. (0 for the one-noded tree)
- T value: store a value with type T.
- AVLTreeNode* l: store a root of left subtree of this node. If there is no such node, store nullptr.
- AVLTreeNode* r: store a root of right subtree of this node. If there is no such node, store nullptr.
- AVLTreeNode* p: store a parent node of this node. If there is no such node, store nullptr.

AVLTree supports these kinds of queries below.
- bool isEmpty(void): return true when 
- void insert(T t): insert a node storing value t into the tree
- AVLTreeNode<T>* getRoot(): return root node of the tree. If the tree is empty, return nullptr.
- AVLTreeNode<T>* pop(): pop a node with the smallest value and return. If the tree is empty, return nullptr.
- AVLTreeNode<T>* pop(T t): pop a node with a certain value t and return. If there is no such node, return nullptr.
