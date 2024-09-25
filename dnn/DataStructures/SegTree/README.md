# SegmentTree

This project is an implementation of Segment Tree which can support dynamic range maximum queries. 
There is no corresponding paper. 

## What is 'SegmentTree'
SegmentTree is implemented as a balanced binary tree. 
SegmentTree has 'Node' struct as its node.  
Node has these elements:
- int a: store the value of the node.
- int b: store the maximum value in the subtree of the node.
- int c: store the index of the node.
- int l: store the smallest index in the subtree of the node.
- int r: store the largest index in the subtree of the node.
- struct Node* lchild: store a root of left subtree of this node. If there is no such node, store nullptr.
- struct Node* rchild: store a root of right subtree of this node. If there is no such node, store nullptr.


SegmentTree supports these kinds of queries below.
- Node* makeSegmentTree(int left, int right): make a segment tree with range [left,right].
- pair<int, int> max(Node* node, int left, int right): query the maximum value of the segment tree with root node in range [left,right].
- void add(Node* node, int l, int r, int x): add x to the nodes in range [left,right].
- void set(Node* node, int idx, int x): set the value to x to the nodes in range [left,right].
- void update(Node* node): update the segment tree with root node.
- void clearTree(Node* node): clear the segment tree with root node.
- void postorderout(Node* node): print all nodes of the segment tree with root node in postorder.

## Before use it 
### Setup the project
None

### Dependent project
None