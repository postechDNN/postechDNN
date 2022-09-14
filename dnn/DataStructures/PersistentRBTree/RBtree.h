#pragma once

#include <iostream>
#include <queue>
#include <unordered_set>

using namespace std;

// data structure that represents a node in the tree
template <typename T>
struct Node {
	T data; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	int size;
};


// class RBTree implements the operations in Red Black Tree
template <typename T>
class RBTree {
private:
	Node<T>* root;
	Node<T>* TNULL;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(Node<T>* node, Node<T>* parent) {
		//node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
		node->size = 0;
	}

	void preOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			cout << node->data << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout << node->data << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->data << " ";
		}
	}

	Node<T>* searchTreeHelper(Node<T>* node, T key, int op) {
		if (node == TNULL || key == node->data) {
			return node;
		}

		if (key < node->data) {
			Node<T>* x = searchTreeHelper(node->left, key, op);
			if (op == 1 && x == TNULL) //searchGEQ
				return node;
			else
				return x;
		}
		else {
			Node<T>* x = searchTreeHelper(node->right, key, op);
			if (op == 2 && x == TNULL) //searchLEQ
				return node;
			else
				return x;
		}
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(Node<T>* x) {
		Node<T>* s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(Node<T>* u, Node<T>* v) {
		if (u->parent == nullptr) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(Node<T>* node, T key) {
		// find the node containing key
		Node<T>* z = TNULL;
		Node<T>* x;
		Node<T>* y;
		while (node != TNULL) {
			node->size--;
			if (node->data == key) {
				z = node;
				break;
			}

			if (node->data < key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = minimum(z->right, true);
			y_original_color = y->color;
			x = y->right;
			y->size = z->size;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}
	}

	// fix the red-black tree
	void fixInsert(Node<T>* k) {
		Node<T>* u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(Node<T>* root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != TNULL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->data << "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		TNULL = new Node<T>;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
		TNULL->size = 0;
	}

	//copy the tree using BFS
	RBTree(RBTree<T>* tree) {
		TNULL = new Node<T>;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
		TNULL->size = 0;
		if (tree->getRoot() == tree->TNULL)
			return;
		queue<pair<Node<T>*,Node<T>*>> q;
		
		Node<T>* n_root = new Node<T>;
		n_root->parent = nullptr;
		n_root->data = tree->root->data;
		n_root->left = TNULL;
		n_root->right = TNULL;
		n_root->color = tree->root->color;
		n_root->size = tree->root->size;
		root = n_root;
		q.push({ tree->root, n_root });
		while (!q.empty()) {
			Node<T>* x = q.front().first;
			Node<T>* y = q.front().second;
			q.pop();
			if (x->left != tree->TNULL) {
				Node<T>* node = new Node<T>;
				y->left = node;
				node->parent = y;
				node->data = x->left->data;
				node->left = TNULL;
				node->right = TNULL;
				node->color = x->left->color; 
				node->size = x->left->size;
				q.push({ x->left,node });
			}
			if (x->right != tree->TNULL) {
				Node<T>* node = new Node<T>;
				y->right = node;
				node->parent = y;
				node->data = x->right->data;
				node->left = TNULL;
				node->right = TNULL;
				node->color = x->right->color;
				node->size = x->right->size;
				q.push({ x->right,node });
			}
		}
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	Node<T>* search(T k) {
		return searchTreeHelper(this->root, k, 0);
	}

	// search the tree for the least key great or equal than k
	// and return the corresponding node
	Node<T>* searchGEQ(T k) {
		return searchTreeHelper(this->root, k, 1);
	}

	// search the tree for the greatest key less or equal than k
	// and return the corresponding node
	Node<T>* searchLEQ(T k) {
		return searchTreeHelper(this->root, k, 2);
	}

	// find the node with the minimum key
	Node<T>* minimum(Node<T>* node, bool erase) {
		while (node->left != TNULL) {
			if (erase)
				node->size--;
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	Node<T>* maximum(Node<T>* node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	Node<T>* successor(Node<T>* x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right, false);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		Node<T>* y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	Node<T>* predecessor(Node<T>* x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		Node<T>* y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(Node<T>* x) {
		Node<T>* y = x->right;

		int tmp = y->size;
		y->size = x->size;
		x->size -= (tmp - y->left->size);

		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(Node<T>* x) {
		Node<T>* y = x->left;

		int tmp = y->size;
		y->size = x->size;
		x->size -= (tmp - y->right->size);

		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(T key) {
		// Ordinary Binary Search Insertion
		Node<T>* node = new Node<T>;
		node->parent = nullptr;
		node->data = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red
		node->size = 1;

		Node<T>* y = nullptr;
		Node<T>* x = this->root;

		while (x != TNULL) {
			y = x;
			x->size++;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->data < y->data) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	Node<T>* getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(T data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

	Node<T>* getkthNode(int k) {
		Node<T>* node = root;
		if (k < 0 || k >= root->size)
			return TNULL;
		while (node != TNULL) {
			if (k == node->left->size) {
				return node;
			}
			if (k < node->left->size) {
				node = node->left;
			}
			else {
				k -= (node->left->size + 1);
				node = node->right;
			}
		}
		return TNULL;
	}

};