# RBTree

This project is an implementation of RBTree which can support various kinds of line-sweep algorithms such as line sweep intersection, constructing point location datastructure, etc.  
There is no corresponding paper.

## What is 'RBTree'

RBTree is a data structure which stores data with template type T for each node.  
Operator '=', '<' and '==' must be defined for type T to support RBTree.  
The datas are stored in increasing order.  
RBTree has 'Node<T>' struct as its node.  
Node has these elements:
- T key: store a value with type T.
- Color color: store the color of the node(red, black or double_black)
- Node* left_child: store a root of left subtree of this node. If there is no such node, store nullptr.
- Node* right_child: store a root of right subtree of this node. If there is no such node, store nullptr.
- Node* parent: store a parent node of this node. If there is no such node, store nullptr.
- Node * pre: the previous node of the node in the tree.
- Node * next: the next node of the node in the tree.

RBTree supports these kinds of operations below.
- void insert_bst(T key): insert a node storing value key into the tree.
- void delete_bst(T key): delete the node storing value key into the tree.
- Node<T>* search(T key): return the node storing value key.
- Node<T>* find_same_or_biggest_small(T key): return the node storing the value same as key or the biggest value smaller than key.
- Node<T>* find_same_or_smallest_big(T key): return the node storing the value same as key or the smallest value bigger than key. 

## Before use it 
### Setup the project
none
### Dependent project
none
