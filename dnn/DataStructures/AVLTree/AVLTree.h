#pragma once
#include <algorithm>
#include <cstdlib>

//operater =, < and == must be defined for type T.
template <typename T>
struct AVLTreeNode {
	AVLTreeNode();
	~AVLTreeNode();
	int rank; // stores a rank of the node
	T value; // stores a data
	AVLTreeNode *l, *r, *p; // l stores left child, r stores right child, p stores parent. Initialized as nullptr
};

template <typename T>
class AVLTree {
private:
	AVLTreeNode<T>* root;
	void LRotate(AVLTreeNode<T>*); // perform L rotate from subtree
	void RRotate(AVLTreeNode<T>*); // perform R rotate from subtree
	void balance(AVLTreeNode<T>*); // balance the subtree
	void calRank(AVLTreeNode<T>*); // calculate rank for each subtrees
public:
	AVLTree();
	~AVLTree();
	bool isEmpty();
	void insert(T);
	void insert(AVLTreeNode<T>*);
	AVLTreeNode<T>* getRoot();
	AVLTreeNode<T>* getLeftNode(T);
	AVLTreeNode<T>* getRightNode(T);
	AVLTreeNode<T>* pop();
	AVLTreeNode<T>* pop(T);
};

template <typename T>
AVLTreeNode<T>::AVLTreeNode() {
	this->l = nullptr;
	this->r = nullptr;
	this->p = nullptr;
	this->rank = 0;
}

template <typename T>
AVLTreeNode<T>::~AVLTreeNode() {
	if (this->l)
		delete(this->l);
	if (this->r)
		delete(this->r);
}

template <typename T>
void AVLTree<T>::LRotate(AVLTreeNode<T>* n) {
	AVLTreeNode<T>* temp;
	if (n == this->root)
		this->root = n->l;
	else if (n == n->p->l)
		n->p->l = n->l;
	else
		n->p->r = n->l;
	temp = n->p;
	n->p = n->l;
	n->l->p = temp;
	temp = n->l->r;
	if (temp)
		temp->p = n;
	n->l->r = n;
	n->l = temp;
	calRank(n);
	temp = n;
	while (temp != this->root) {
		temp = temp->p;
		calRank(temp);
	}
}

template <typename T>
void AVLTree<T>::RRotate(AVLTreeNode<T>* n) {
	AVLTreeNode<T>* temp;
	if (n == this->root)
		this->root = n->r;
	else if (n == n->p->l)
		n->p->l = n->r;
	else
		n->p->r = n->r;
	temp = n->p;
	n->p = n->r;
	n->r->p = temp;
	temp = n->r->l;
	if (temp)
		temp->p = n;
	n->r->l = n;
	n->r = temp;
	calRank(n);
	temp = n;
	while (temp != this->root) {
		temp = temp->p;
		calRank(temp);
	}
}

template <typename T>
void AVLTree<T>::balance(AVLTreeNode<T>* n) {
	if ((!n->r&&n->l&&n->l->rank == 1) || (n->l&&n->r&&n->l->rank - n->r->rank == 2)) {
		if (!n->l->l || (n->l->r && n->l->r->rank > n->l->l->rank))
			RRotate(n->l);
		LRotate(n);
		return;
	}
	if ((!n->l&&n->r&&n->r->rank == 1) || (n->l&&n->r&&n->r->rank - n->l->rank == 2)) {
		if (!n->r->r || (n->r->l && n->r->l->rank > n->r->r->rank))
			LRotate(n->r);
		RRotate(n);
		return;
	}
}

template <typename T>
void AVLTree<T>::calRank(AVLTreeNode<T>* n) {
	if (n->l&&n->r)
		n->rank = std::max(n->l->rank, n->r->rank) + 1;
	else if (n->l)
		n->rank = n->l->rank + 1;
	else if (n->r)
		n->rank = n->r->rank + 1;
	else
		n->rank = 0;
}

template <typename T>
AVLTree<T>::AVLTree() {
	this->root = nullptr;
}

template <typename T>
AVLTree<T>::~AVLTree() {
	if (this->root)
		delete(this->root);
}

template <typename T>
bool AVLTree<T>::isEmpty() {
	return !this->root;
}

template <typename T>
void AVLTree<T>::insert(T v) {
	AVLTreeNode<T>* newnode = new AVLTreeNode<T>();
	newnode->value = v;
	this->insert(newnode);
}

template <typename T>
void AVLTree<T>::insert(AVLTreeNode<T>* newnode) {
	if (isEmpty()) {
		this->root = newnode;
		newnode->p = nullptr;
		return;
	}
	AVLTreeNode<T>* temp = this->root;
	while (true) {
		if (temp->value < newnode->value) {
			if (!temp->r) {
				temp->r = newnode;
				newnode->p = temp;
				temp->rank = 1;
				while (temp != this->root) {
					temp = temp->p;
					calRank(temp);
					balance(temp);
				}
				return;
			}
			temp = temp->r;
		}
		else {
			if (!temp->l) {
				temp->l = newnode;
				newnode->p = temp;
				temp->rank = 1;
				while (temp != this->root) {
					temp = temp->p;
					calRank(temp);
					balance(temp);
				}
				return;
			}
			temp = temp->l;
		}
	}
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::getRoot() {
	return this->root;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::getLeftNode(T v) {
	if (!this->root)
		return nullptr;
	AVLTreeNode<T>* temp = this->root;
	AVLTreeNode<T>* localMax = nullptr;
	while (true) {
		if (temp->value < v) {
			localMax = temp;
			if (!temp->r)
				return temp;
			temp = temp->r;
		}
		else {
			if (!temp->l)
				return localMax;
			temp = temp->l;
		}
	}
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::getRightNode(T v) {
	if (!this->root)
		return nullptr;
	AVLTreeNode<T>* temp = this->root;
	AVLTreeNode<T>* localMin = nullptr;
	while (true) {
		if (v < temp->value) {
			localMin = temp;
			if (!temp->l)
				return temp;
			temp = temp->l;
		}
		else {
			if (!temp->r)
				return localMin;
			temp = temp->r;
		}
	}
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::pop() {
	if (!this->root)
		return nullptr;
	AVLTreeNode<T>* temp = this->root;
	while (temp->l)
		temp = temp->l;
	if (temp == this->root) {
		this->root = temp->r;
		if (temp->r)
			temp->r->p = nullptr;
		return temp;
	}
	temp->p->l = temp->r;
	if (temp->r)
		temp->r->p = temp->p;
	AVLTreeNode<T>* newtemp = temp->p;
	calRank(newtemp);
	balance(newtemp);
	while (newtemp != this->root) {
		newtemp = newtemp->p;
		calRank(newtemp);
		balance(newtemp);
	}
	return temp;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::pop(T v) {
	if (!this->root)
		return nullptr;
	AVLTreeNode<T>* temp = this->root;
	while (true) {
		if (temp->value == v) {
			if (temp == this->root) {
				this->root = nullptr;
				return temp;
			}
			if (!temp->l) {
				if (temp->p->l == temp)
					temp->p->l = temp->r;
				else
					temp->p->r = temp->r;
				AVLTreeNode<T>* newtemp = temp->p;
				calRank(newtemp);
				balance(newtemp);
				while (newtemp != this->root) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				return temp;
			}
			else if (!temp->r) {
				if (temp->p->l == temp)
					temp->p->l = temp->l;
				else
					temp->p->r = temp->l;
				AVLTreeNode<T>* newtemp = temp->p;
				calRank(newtemp);
				balance(newtemp);
				while (newtemp != this->root) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				return temp;
			}
			else {
				AVLTreeNode<T>* stemp = temp->l;
				while (stemp->r)
					stemp = stemp->r;
				stemp->p->r = stemp->l;
				AVLTreeNode<T>* newtemp = stemp->p;
				calRank(newtemp);
				balance(newtemp);
				while (newtemp != temp) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				stemp->p = temp->p;
				if (temp->p->l == temp)
					temp->p->l = stemp;
				else
					temp->p->r = stemp;
				stemp->rank = temp->rank;
				stemp->l = temp->l;
				if (temp->l)
					temp->l->p = stemp;
				stemp->r = temp->r;
				if (temp->r)
					temp->r->p = stemp;
				newtemp = stemp;
				while (newtemp != temp) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				return temp;
			}
		}
		else if (temp->value < v) {
			if (!temp->r)
				return nullptr;
			temp = temp->r;
		}
		else {
			if (!temp->l)
				return nullptr;
			temp = temp->l;
		}
	}
}