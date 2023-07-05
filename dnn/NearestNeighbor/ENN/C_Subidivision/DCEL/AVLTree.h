
#pragma once
#include <algorithm>
#include <cstdlib>

//operater =, < and == must be defined for type T.
template <typename T>
struct AVLTreeNode {
	AVLTreeNode();
	~AVLTreeNode();
	int rank; // stores a rank of the node
	int size; // stores size of the subtree
	T value; // stores a data
	AVLTreeNode *l, *r, *p; // l stores left child, r stores right child, p stores parent. Initialized as nullptr
};

template <typename T>
class AVLTree {
private:
	AVLTreeNode<T>* root;
	//need to update size
	void LRotate(AVLTreeNode<T>*); // perform L rotate from subtree
	void RRotate(AVLTreeNode<T>*); // perform R rotate from subtree
	void balance(AVLTreeNode<T>*); // balance the subtree
	void calRank(AVLTreeNode<T>*); // calculate rank for each subtrees
public:
	AVLTree();
	~AVLTree();
	int size();
	bool isEmpty();
	void insert(T);
	void insert(AVLTreeNode<T>*);
	AVLTreeNode<T>* getRoot();
	AVLTreeNode<T>* getLeftNode(T);
	AVLTreeNode<T>* getRightNode(T);
	AVLTreeNode<T>* pop();
	AVLTreeNode<T>* pop(T);
	AVLTreeNode<T>* getkthNode(int k);
	AVLTreeNode<T>* successorNode(AVLTreeNode<T>* v);
	AVLTreeNode<T>* predecessorNode(AVLTreeNode<T>* v);
	AVLTreeNode<T>* minSubtree(AVLTreeNode<T>* v);
	AVLTreeNode<T>* maxSubtree(AVLTreeNode<T>* v);
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
	int tempsize = n->size;
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
	int sz = temp ? temp->size : 0;
	if (temp)
		temp->p = n;
	n->l->r = n;
	n->size += sz - n->l->size;
	n->l->size = tempsize;
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
	int tempsize = n->size;
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
	int sz = temp ? temp->size : 0;
	if (temp)
		temp->p = n;
	n->r->l = n;
	n->size += sz - n->r->size;
	n->r->size = tempsize;
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
int AVLTree<T>::size() {
	return this->root->size;
}

template <typename T>
void AVLTree<T>::insert(T v) {
	AVLTreeNode<T>* newnode = new AVLTreeNode<T>();
	newnode->value = v;
	newnode->size = 1;
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
		temp->size++;
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
	while (temp->l) {
		temp->size--;
		temp = temp->l;
	}
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
	// else if (this->size() == 1 && this->root->value == v){
	// 	AVLTreeNode<T>* tmp = this->root;
	// 	this->root = nullptr;
	// 	return tmp;
	// }
	AVLTreeNode<T>* temp = this->root;
	while (true) {
		if (temp->value == v) {
			temp->size--;
			if (!temp->l) {
				if (temp == this->root) {
					this->root = temp->r;
					if (temp->r)
						temp->r->p = nullptr;
					return temp;
				}
				if (temp->r)
					temp->r->p = temp->p;
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
				if (temp == this->root) {
					this->root = temp->l;
					temp->l->p = nullptr;
					return temp;
				}
				temp->l->p = temp->p;
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
				stemp->size--;
				while (stemp->r) {
					stemp = stemp->r;
					stemp->size--;
				}
				if (stemp->p != temp)
					stemp->p->r = stemp->l;
				else
					stemp->p->l = stemp->l;
				if (stemp->l)
					stemp->l->p = stemp->p;
				AVLTreeNode<T>* newtemp = stemp->p;
				calRank(newtemp);
				balance(newtemp);
				while (newtemp != temp) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				stemp->p = temp->p;
				stemp->rank = temp->rank;
				stemp->l = temp->l;
				stemp->size = temp->size;
				if (temp->l)
					temp->l->p = stemp;
				stemp->r = temp->r;
				if (temp->r)
					temp->r->p = stemp;
				if (this->root == temp) {
					this->root = stemp;
					return temp;
				}
				if (temp->p->l == temp)
					temp->p->l = stemp;
				else
					temp->p->r = stemp;
				newtemp = stemp;
				/*
				while (newtemp != temp) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}*/
				while (newtemp != this->root) {
					newtemp = newtemp->p;
					calRank(newtemp);
					balance(newtemp);
				}
				return temp;
			}
		}
		else if (temp->value < v) {
			if (!temp->r) {
				while (temp != this->root) {
					temp = temp->p;
					temp->size++;
				}
				return nullptr;
			}
			temp->size--;
			temp = temp->r;
		}
		else {
			if (!temp->l) {
				while (temp != this->root) {
					temp = temp->p;
					temp->size++;
				}
				return nullptr;
			}
			temp->size--;
			temp = temp->l;
		}
	}
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::getkthNode(int k)
{
	AVLTreeNode<T>* temp = root;
	int psize = temp->size;
	while (true)
	{
		int tsize = temp->size;
		if (k >= tsize)
			return nullptr;
		if (temp->l && k == temp->l->size || !temp->l && k == 0)
			return temp;
		if (temp->l && k < temp->l->size)
			temp = temp->l;
		else if (temp->r)
		{
			k--;
			if (temp->l)
				k -= temp->l->size;
			temp = temp->r;
		}
		else
			return nullptr;
	}
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::minSubtree(AVLTreeNode<T>* v){
	if(v == nullptr) return nullptr;
	AVLTreeNode<T>* ret = v;
	while(ret->l)
		ret = ret->l;
	return ret;
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::maxSubtree(AVLTreeNode<T>* v){
	if(v == nullptr) return nullptr;
	AVLTreeNode<T>* ret = v;
	while(ret->r)
		ret = ret->r;
	return ret;
}

//return the successor of the node v
template<typename T>
AVLTreeNode<T>* AVLTree<T>::successorNode(AVLTreeNode<T>* v){
	if(v->r){
		return minSubtree(v->r);
	}
	else{
		while(v->p && v->p->r == v) //v is right child
			v = v->p;
		if(v == this->root)
			return nullptr;
		else return v->p; 
	}
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::predecessorNode(AVLTreeNode<T>* v){
	if(v->l){
		return maxSubtree(v->l);
	}
	else{
		while(v->p && v->p->l == v) //v is right child
			v = v->p;
		if(v == this->root)
			return nullptr;
		else return v->p; 
	}
}