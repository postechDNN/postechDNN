# Balanced Binary Search Tree(BBST)

This project is an implementation of BBST which can support various kinds of line-sweep algorithms such as line sweep intersection, constructing point location datastructure, etc.  
There is no corresponding paper.

## What is 'BBST'

BBST is a data structure which stores data with template type T for each node.  
Import **SimpleBBST.h** to use this data structure. 
Operator '=', '<' and '==' must be defined for type T to support BBST.  
The datas are stored in increasing order.  
BBST has 'SimpleBBSTNode<T>' struct as its node.  
BBST has these elements:
- T value: store a value with type T.
- std::multiset<T>::iterator it: store the iterator of this node.

BBST supports these kinds of queries below.
- bool isEmpty(): return true when the tree is empty
- bool isLeftMost(SimpleBBSTNode<T> nt): return true when the node has the smallest value.
- bool isRightMost(SimpleBBSTNode<T> nt): return true when the node has the largest value.
- void insert(T t): insert a node storing value t into the tree.
- void erase(T t): erase the node storing value t into the tree.
- SimpleBBSTNode<T> findLeft(T t): return the node left to the node	storing value t into the tree.
- SimpleBBSTNode<T> findRight(T t): return the node right to the node storing value t into the tree.
- SimpleBBSTNode<T> getLeftNode(SimpleBBSTNode<T> nt): return the node left to the node nt.
- SimpleBBSTNode<T> getRightNode(SimpleBBSTNode<T> nt): return the node right to the node nt.


## Before use it 
### Setup the project
none
### Dependent project
none
