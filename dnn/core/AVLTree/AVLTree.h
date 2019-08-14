#pragma once
//operater < and == must be defined for type T.
template <typename T>
struct AVLTreeNode {
	AVLTreeNode();
	~AVLTreeNode();
	int rank;
	T value;
	AVLTreeNode *l, *r, *p;
};

template <typename T>
class AVLTree {
private:
	AVLTreeNode<T>* root;
	void LRotate(AVLTreeNode<T>*);
	void RRotate(AVLTreeNode<T>*);
	void balance(AVLTreeNode<T>*);
public:
	AVLTree();
	~AVLTree();
	bool isEmpty();
	void insert(T);
	AVLTreeNode<T>* getLeftNode(T);
	AVLTreeNode<T>* getRightNode(T);
	AVLTreeNode<T>* pop();
	AVLTreeNode<T>* pop(T);
};