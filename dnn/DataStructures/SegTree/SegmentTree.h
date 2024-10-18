#pragma once

#include <iostream>
#include <vector>
#include <type_traits>


using namespace std;

//operater =, + and == must be defined for type T if a comparison function is not given. 
template <typename T, typename Comp = std::less<T>()>
struct Node {
	T a;
	T b;
	int c;

	int l;
	int r;
	
	struct Node* lchild;
	struct Node* rchild;
};

template <typename T, typename Comp = std::less<T>()>
class SegmentTree {
public:
	Node<T,Comp>* root;

	Node<T, Comp>* makeSegmentTree(int left, int right, std::vector<T> data) {
		Node<T, Comp>* newNode = new Node<T, Comp>;

		if (left == right) {
			newNode->a = data[left];
			newNode->b = data[left];
			newNode->lchild = NULL;
			newNode->rchild = NULL;
		}
		else {
			newNode->a = 0;
			newNode->lchild = makeSegmentTree(left, (left + right) / 2);
			newNode->rchild = makeSegmentTree((left + right) / 2 + 1, right);
			newNode->b = Comp(newNode->lchild->b, newNode->rchild->b) ? newNode->rchild->b : newNode->lchild->b;
		}
		newNode->c = left;
		newNode->l = left;
		newNode->r = right;

		return newNode;
	}

	pair<int, int> max(Node<T, Comp>* node, int l, int r) {
		pair<int, T> p;
		pair<int, T> p2;
		if (node->l >= l && node->r <= r) {
			p.first = node->c; p.second = node->b;
			return p;
		}
		else if (node->l >= l && node->r > r) {
			if (node->lchild->r >= r) {
				p = max(node->lchild, l, r);
				p.second += node->a;
				return p;
			}
			else {
				p = max(node->lchild, l, r);
				p2 = max(node->rchild, l, r);
				if (Comp(p.second, p2.second)) p = p2;
				p.second += node->a;
				return p;
			}
		}
		else if (node->l < l && node->r <= r) {
			if (node->rchild->l <= l) {
				p = max(node->rchild, l, r);
				p.second += node->a;
				return p;
			}
			else {
				p = max(node->lchild, l, r);
				p2 = max(node->rchild, l, r);
				if (Comp(p.second, p2.second)) p = p2;
				p.second += node->a;
				return p;
			}
		}
		else {
			if (node->lchild->r < l) {
				p = max(node->rchild, l, r);
				p.second += node->a;
				return p;
			}
			if (node->rchild->l > r) {
				p = max(node->lchild, l, r);
				p.second += node->a;
				return p;
			}
			p = max(node->lchild, l, r);
			p2 = max(node->rchild, l, r);
			if (Comp(p.second, p2.second)) p = p2;
			p.second += node->a;
			return p;
		}
	}
	void add(Node<T, Comp>* node, int l, int r, T x) {
		if (l > r) return;
		if (node->l >= l && node->r <= r) {
			node->a += x;
			update(node);
		}
		else if (node->l >= l && node->r > r) {
			if (node->lchild->r >= r) {
				add(node->lchild, l, r, x);
				update(node);
			}
			else {
				add(node->lchild, l, r, x);
				add(node->rchild, l, r, x);
				update(node);
			}
		}
		else if (node->l < l && node->r <= r) {
			if (node->rchild->l <= l) {
				add(node->rchild, l, r, x);
				update(node);
			}
			else {
				add(node->lchild, l, r, x);
				add(node->rchild, l, r, x);
				update(node);
			}
		}
		else {
			if (node->lchild->r < l) {
				add(node->rchild, l, r, x);
				update(node);
			}
			else if (node->rchild->l > r) {
				add(node->lchild, l, r, x);
				update(node);
			}
			else {
				add(node->lchild, l, r, x);
				add(node->rchild, l, r, x);
				update(node);
			}
		}
	}

	void set(Node<T, Comp>* node, int idx, T x) {
		if (node->l == idx && node->r == idx) {
			node->a = x;
			update(node);
		}
		else if (node->lchild->r < idx) {
			set(node->rchild, idx, x - node->a);
			update(node);
		}
		else {
			set(node->lchild, idx, x - node->a);
			update(node);
		}
	}

	void update(Node<T, Comp>* node) {
		if (node->lchild == NULL) {
			node->b = node->a;
		}
		else if (node->rchild == NULL || node->rchild->b <= node->lchild->b) {
			node->b = node->a + node->lchild->b;
			node->c = node->lchild->c;
		}
		else {
			node->b = node->a + node->rchild->b;
			node->c = node->rchild->c;
		}
	}

	/*void clearTree(Node<T, Comp>* node) {
		if (node == NULL) return;

		node->a = (node->lchild == NULL) ? MIN : 0;
		node->b = MIN;
		node->c = node->l;
		clearTree(node->lchild);
		clearTree(node->rchild);
	}
	void postorderout(Node<T, Comp>* node) {
		if (node == NULL) return;
		postorderout(node->lchild);
		postorderout(node->rchild);
		cout << node->l << " " << node->r << " " << node->a << " " << node->b << " " << node->c << endl;
	}*/
};