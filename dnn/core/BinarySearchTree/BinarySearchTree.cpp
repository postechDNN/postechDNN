#include "BinarySearchTree.h"
#include <algorithm>
#include <cstdlib>

template <typename T>
BinarySearchTree<T>::BinarySearchTree() {
	this->root = nullptr;
}

template <typename T>
bool BinarySearchTree<T>::isEmpty(){
	return this->root;
}

template <typename T>
void BinarySearchTree<T>::insert(T data) {
	node<T>* n = new node<T>;
	n->value = data;
	n->rank = 0;
	n->l = nullptr;
	n->r = nullptr;
	n->p = nullptr;
	if (!this->root)
		this->root = n;
	else {
		node<T>* i = this->root;
		while (true) {
			if (i->value > data) {
				if (i->l)
					i = i->l;
				else {
					i->l = n;
					i->rank = 1;
					n->p = i;
					break;
				}
			}
			else {
				if (i->r)
					i = i->r;
				else {
					i->r = n;
					i->rank = 1;
					n->p = i;
					break;
				}
			}
		}
		while (i->p) {
			
		}
	}
}