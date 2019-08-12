#pragma once

template <typename T>
struct node {
	int rank;
	T value;
	node *l, *r, *p;
};

template <typename T>
class BinarySearchTree {
private:
	node<T>* root;
public:
	BinarySearchTree();
	bool isEmpty();
	void insert(T);
	void erase(T);
	node<T>* getLeftNode(T);
	node<T>* getRightNode(T);
	T popLeftmost();
};