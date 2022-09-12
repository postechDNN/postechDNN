// Reference: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
#pragma once


#include <iostream>

using namespace std;

enum Color {
	black,
	red
};

// data structure that represents a node in the tree
template <typename T>
struct Node {
	T data; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	bool ins_interval;
	bool del_interval;
	Node* top;
	Node* bottom;
	int size; //erase ->size after debugging!!
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
		node->color = black;
		node->ins_interval = false;
		node->del_interval = false;
		node->top = nullptr;
		node->bottom = nullptr;
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

	Node<T>* searchTreeHelper(Node<T>* node, T key) {
		if (node == TNULL || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(Node<T>* x, Node<T>* top_t, Node<T>* escape_t) { //modified by Taekang
		Node<T>* s;
		Node<T>* bot = x; //added by Taekang
		Node<T>* top = TNULL;     //added by Taekang
		while (x != root && ((x->color == 0) == (x != escape_t))) { //modified by Taekang
			if (x == x->parent->left) {
				s = x->parent->right;
				if ((s->color == 1) == (x != escape_t)) {  //modified by Taekang
					// case 3.1
					split_intv1(x, s, top_t, escape_t);
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0 && !s->del_interval && 
					(x != escape_t || x == top_t->bottom || s == top_t->bottom)) { //modified by Taekang
					// case 3.2
					if (x == top_t->bottom || s == top_t->bottom) //added by taekang //x is the end of ins_interval
						split_insert_interval(top_t, escape_t);
					if (x->parent->color == red) //Added by Taekang
						s->color = 1; 
					if (x != bot)
						top = x;
					x = x->parent;
					if (x == escape_t && top_t->del_interval) //parent is the bottom of del_interval
						split_del_interval(top_t, escape_t);
				}
				else {
					if ((s->right->color == 0 && x != escape_t) || (s->left->color == 0 && s->right->color == 0)) { //modified by Taekang
						// case 3.3
						if (x != escape_t && !s->del_interval && !s->ins_interval) { //modified by Taekang
							s->left->color = 0;
							s->color = 1;
						}
						else if (x == top_t->bottom) { //do nothing

						}
						else if (s == top_t->bottom) {
							top_t->bottom = x;
						}
						else if (s->del_interval) {
							if (s->bottom != s->right) {
								s->right->del_interval = true;
								s->right->bottom = s->bottom;
							}
							s->color = red;
							s->del_interval = false;
							s->bottom = TNULL;
						}
						else {
							Node<T>* sl = s->left;
							Node<T>* new_top = (sl->data < top_t->bottom->data) ? sl->right : sl->left;
							Node<T>* new_top_s = (sl->data < top_t->bottom->data) ? sl->left : sl->right;
							Node<T>* old_bottom = s->ins_interval ? s->bottom:top_t->bottom;
							if (!s->ins_interval)
								x->parent->color = red;
							if (x->parent->parent != top_t && !s->ins_interval) {
								top_t->bottom = x->parent->parent;
							}
							else {
								top_t->bottom = TNULL;
								top_t->ins_interval = false;
								top_t = TNULL;
								escape_t = TNULL;
							}
							new_top_s->color = black;
							if (new_top == old_bottom) {
								new_top->color = 0;
							}
							else {
								new_top->ins_interval = true;
								new_top->bottom = old_bottom;
							}
						}
						rightRotate(s);
						s = x->parent->right;
					}
					// case 3.4
					if (x != escape_t && !s->del_interval && !s->ins_interval) { //modified by Taekang
						if (x->parent == top_t->bottom)
							top_t->bottom = s;
						s->color = x->parent->color;
						x->parent->color = 0;
						s->right->color = 0;
					}
					else if (x == top_t->bottom || s == top_t->bottom) {
						x->parent->color = red;
						s->color = black;
						top_t->bottom = x->parent;
					}
					else if (s->del_interval) {
						if (x->parent == top_t->bottom)
							top_t->bottom = s;
						if (s->bottom != s->left) {
							s->left->del_interval = true;
							s->left->bottom = s->bottom;
						}
						s->color = x->parent->color;
						x->parent->color = black;
						s->del_interval = false;
						s->bottom = TNULL;
					}
					else {
						Node<T>* sr = s->right;
						Node<T>* new_top = (sr->data < top_t->bottom->data) ? sr->right : sr->left;
						Node<T>* new_top_s = (sr->data < top_t->bottom->data) ? sr->left : sr->right;
						Node<T>* old_bottom = s->ins_interval ? s->bottom : top_t->bottom;
						s->color = s->ins_interval? x->parent->color : red;
						x->parent->color = black;
						sr->color = black;
						if (x->parent->parent != top_t && !s->ins_interval) {
							top_t->bottom = x->parent->parent;
						}
						else {
							top_t->bottom = TNULL;
							top_t->ins_interval = false;
							top_t = TNULL;
							escape_t = TNULL;
						}
						new_top_s->color = black;
						if (new_top == old_bottom) {
							new_top->color = 0;
						}
						else {
							new_top->ins_interval = true;
							new_top->bottom = old_bottom;
						}
					}
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					split_intv1(x, s, top_t, escape_t);
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->left->color == 0 && s->right->color == 0 && !s->del_interval &&
					(x != escape_t || x == top_t->bottom || s == top_t->bottom)) { //modified by Taekang
					// case 3.2
					if (x == top_t->bottom || s == top_t->bottom) //added by taekang //x is the end of ins_interval
						split_insert_interval(top_t, escape_t);
					if (x->parent->color == red) //Added by Taekang
						s->color = 1;
					if (x != bot)
						top = x;
					x = x->parent;
					if (x == escape_t && top_t->del_interval)
						split_del_interval(top_t, escape_t);
				}
				else {
					if ((s->left->color == 0 && x != escape_t) || (s->left->color == 0 && s->right->color == 0)) { //modified by Taekang
						// case 3.3
						if (x != escape_t && !s->del_interval && !s->ins_interval) { //modified by Taekang
							s->right->color = 0;
							s->color = 1;
						}
						else if (x == top_t->bottom) { //do nothing

						}
						else if (s == top_t->bottom) {
							top_t->bottom = x;
						}
						else if (s->del_interval) {
							if (s->bottom != s->left) {
								s->left->del_interval = true;
								s->left->bottom = s->bottom;
							}
							s->color = red;
							s->del_interval = false;
							s->bottom = TNULL;
						}
						else {
							Node<T>* sr = s->right;
							Node<T>* new_top = (sr->data < top_t->bottom->data) ? sr->right : sr->left;
							Node<T>* new_top_s = (sr->data < top_t->bottom->data) ? sr->left : sr->right;
							Node<T>* old_bottom = s->ins_interval ? s->bottom : top_t->bottom;
							if (!s->ins_interval)
								x->parent->color = red;
							if (x->parent->parent != top_t && !s->ins_interval) {
								top_t->bottom = x->parent->parent;
							}
							else {
								top_t->bottom = TNULL;
								top_t->ins_interval = false;
								top_t = TNULL;
								escape_t = TNULL;
							}
							new_top_s->color = black;
							if (new_top == old_bottom) {
								new_top->color = 0;
							}
							else {
								new_top->ins_interval = true;
								new_top->bottom = old_bottom;
							}
						}
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					if (x != escape_t && !s->del_interval && !s->ins_interval) { //modified by Taekang
						if (x->parent == top_t->bottom)
							top_t->bottom = s;
						s->color = x->parent->color;
						x->parent->color = 0;
						s->left->color = 0;
					}
					else if (x == top_t->bottom || s == top_t->bottom) {
						x->parent->color = red;
						s->color = black;
						top_t->bottom = x->parent;
					}
					else if (s->del_interval) {
						if (x->parent == top_t->bottom)
							top_t->bottom = s;
						if (s->bottom != s->right) {
							s->right->del_interval = true;
							s->right->bottom = s->bottom;
						}
						s->color = x->parent->color;
						x->parent->color = black;
						s->del_interval = false;
						s->bottom = TNULL;
					}
					else {
						Node<T>* sl = s->left;
						Node<T>* new_top = (sl->data < top_t->bottom->data) ? sl->right : sl->left;
						Node<T>* new_top_s = (sl->data < top_t->bottom->data) ? sl->left : sl->right;
						Node<T>* old_bottom = s->ins_interval ? s->bottom : top_t->bottom;
						s->color = s->ins_interval ? x->parent->color : red;
						x->parent->color = black;
						sl->color = black;
						if (x->parent->parent != top_t && !s->ins_interval) {
							top_t->bottom = x->parent->parent;
						}
						else {
							top_t->bottom = TNULL;
							top_t->ins_interval = false;
							top_t = TNULL;
							escape_t = TNULL;
						}
						new_top_s->color = black;
						if (new_top == old_bottom) {
							new_top->color = 0;
						}
						else {
							new_top->ins_interval = true;
							new_top->bottom = old_bottom;
						}
					}
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
		if (top != TNULL) {
			top->del_interval = true;
			top->bottom = bot;
		}
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
		Node<T>* top = TNULL;  //added by Taekang
		Node<T>* escape = TNULL; //added by Taekang
		while (node != TNULL) {
			if (node->ins_interval || node->del_interval)
				top = node;
			if (top->bottom == node)
				escape = node;
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
			if (top != TNULL && node != TNULL && escape == TNULL && ((key < node->parent->data) != (top->bottom->data < node->parent->data)))
				escape = node;
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		if (escape == z) {
			if (top->ins_interval)
				split_insert_interval(top, escape);
			else
				split_del_interval(top, escape);
		}
			
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
			y = minimum(z->right, top, escape);
			if (escape == y) {
				if (top->ins_interval)
					split_insert_interval(top, escape);
				else
					split_del_interval(top, escape);
			}
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
		if ((y_original_color == 0 && (escape != x || !top->del_interval) )
			|| (escape == x && top->ins_interval)) { //modified by taekang
			fixDelete(x, top , escape);
		}
	}

	void split_intv1(Node<T>* x, Node<T>* s, Node<T>* &top_t, Node<T>* &escape_t) {
		if (x != escape_t) {
			if (x->parent == top_t->bottom)  //added by Taekang
				top_t->bottom = s;
			else
				s->color = 0;
			x->parent->color = 1;
		}
		else {  //added by Taekang
			x->color = 0;
			if (x == top_t->bottom) {  //x is the bottom of del_interval
				x->parent->color = 1;
				if (x->parent == top_t) {  //Erase del_interval
					s->color = 0;
					top_t->del_interval = false;
					top_t->bottom = TNULL;
					top_t = NULL;
					escape_t = NULL;
				}
				else
					top_t->bottom = s;
			}
			else {     //x is a fringe of ins_interval
				Node<T>* new_top = (s->data < top_t->bottom->data) ? s->right : s->left;
				Node<T>* new_top_s = (s->data < top_t->bottom->data) ? s->left : s->right;
				Node<T>* old_bottom = top_t->bottom;
				if (top_t == x->parent) {  //Erase ins_interval
					top_t->bottom = TNULL;
					top_t->ins_interval = false;
					top_t->del_interval = false;
					top_t = TNULL;
					escape_t = TNULL;
				}
				else {
					top_t->bottom = s;
					s->color = 1;
					escape_t = s;
				}
				new_top_s->color = 0;
				if (new_top == old_bottom) {
					new_top->color = 0;
				}
				else {
					new_top->ins_interval = true;
					new_top->bottom = old_bottom;
				}
			}
		}
	}

	// fix the red-black tree
	void fixInsert(Node<T>* k, Node<T>* s_top, Node<T>* s_escape) { //modified by Taekang
		Node<T>* u;
		Node<T>* bot = k->parent; //added by Taekang
		Node<T>* top = TNULL;     //added by Taekang
		Node<T>* end = TNULL;     //added by Taekang
		Node<T>* end_u = TNULL;     //added by Taekang
		while ((k->parent->color == 1) == (k->parent != s_escape)) { //modified by Taekang
			if (k->parent == s_escape) {
				if (s_top->ins_interval)
					split_insert_interval(s_top, s_escape);
				else
					split_del_interval(s_top, s_escape);
			}
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					/*(Ignored by Taekang)
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					*/
					if (k->parent->parent == s_escape) {
						if (s_top->ins_interval)
							split_insert_interval(s_top, s_escape);
						else
							split_del_interval(s_top, s_escape);
					}
						
					if (bot != k->parent)
						top = k->parent;
					k = k->parent->parent;
					end = k;
					end_u = u;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					if (k->parent->parent == s_escape) {
						if (s_top->ins_interval)
							split_insert_interval(s_top, s_escape);
						else
							split_del_interval(s_top, s_escape);
					}
					//Debug
					if (k->parent->ins_interval) {
						int asdf = 1;
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					/*(Ignored by Taekang)
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					*/
					if (k->parent->parent == s_escape) {
						if (s_top->ins_interval)
							split_insert_interval(s_top, s_escape);
						else
							split_del_interval(s_top, s_escape);
					}
					if (bot != k->parent)
						top = k->parent;
					k = k->parent->parent;
					end = k;
					end_u = u;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					if (k->parent->parent == s_escape) {
						if (s_top->ins_interval)
							split_insert_interval(s_top, s_escape);
						else
							split_del_interval(s_top, s_escape);
					}
					//Debug
					if (k->parent->ins_interval) {
						int asdf = 1;
					}
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
		if (top != TNULL) {
			//Debug
			if (top->color == black) {
				int asdf = 1;
			}
			top->ins_interval = true;
			top->bottom = bot;
			if (end != root)
				end->color = 1;
			//Debug
			if (end_u->ins_interval) {
				int asdf = 1;
			}
			end_u->color = 0;
		}
			
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


	void split_insert_interval(Node<T>*& top, Node<T>*& escape) {
		//Debug
		if (top->color == black) {
			int asdf = 1;
		}
		if (escape == top->bottom) {
			escape->parent->color = red;
			//Debug
			if (escape->parent->left->ins_interval) {
				int asdf = 1;
			}
			//Debug
			if (escape->parent->right->ins_interval) {
				int asdf = 1;
			}
			escape->parent->left->color = black;
			escape->parent->right->color = black;
			if (escape->parent->parent == top) {
				top->color = black;
				top->bottom = TNULL;
				top->ins_interval = false;
				escape = TNULL;
				top = TNULL;
			}
			else {
				top->bottom = escape->parent->parent;
				escape = escape->parent->parent;
			}
		}
		else if (escape->color == red) {
			//Debug
			if (escape->parent->color == red) {
				int asdf = 1;
			}
			escape->parent->color = red;
			//Debug
			if (escape->ins_interval) {
				int asdf = 1;
			}
			escape->color = black;
			Node<T>* old_bottom = top->bottom;
			Node<T>* new_top = escape->parent->data < old_bottom->data ? escape->parent->right : escape->parent->left;
			//Debug
			if (new_top->color == black) {
				int asdf = 1;
			}
			if (escape->parent->parent == top) {
				top->color = black;
				top->bottom = TNULL;
				top->ins_interval = false;
				escape = TNULL;
				top = TNULL;
			}
			else {
				top->bottom = escape->parent->parent;
				escape = escape->parent->parent;
			}
			if (new_top != old_bottom) {
				new_top->ins_interval = true;
				new_top->bottom = old_bottom;
			}
			else {
				//Debug
				if (new_top->ins_interval) {
					int asdf = 1;
				}
				new_top->color = black;
			}
		}
		else {
			Node<T>* old_bottom = top->bottom;
			Node<T>* s = escape->parent->data < old_bottom->data ? escape->parent->right : escape->parent->left;
			Node<T>* new_top = s->data < old_bottom->data ? s->right : s->left;
			//Debug
			if (new_top->color == black) {
				int asdf = 1;
			}
			Node<T>* new_top_s = s->data < old_bottom->data ? s->left : s->right;
			s->color = red;
			//Debug
			if (new_top_s->ins_interval) {
				int asdf = 1;
			}
			new_top_s->color = black;
			if (escape->parent == top) {
				top->color = black;
				top->bottom = TNULL;
				top->ins_interval = false;
				escape = TNULL;
				top = TNULL;
			}
			else {
				top->bottom = escape->parent;
				escape = escape->parent;
			}
			if (new_top != old_bottom) {
				
				new_top->ins_interval = true;
				new_top->bottom = old_bottom;
			}
			else {
				//Debug
				if (new_top->ins_interval) {
					int asdf = 1;
				}
				new_top->color = black;
			}
		}
	}

	void split_del_interval(Node<T>*& top, Node<T>*& escape) {
		Node<T>* s = (escape == escape->parent->left) ? escape->parent->right : escape->parent->left;
		if (escape == top->bottom) {
			
			s->color = red;
			if (escape->parent == top) {
				top->bottom = TNULL;
				top->del_interval = false;
				escape = TNULL;
				top = TNULL;
			}
			else {
				top->bottom = escape->parent;
				escape = escape->parent;
			}
		}
		else {
			escape->color = red;
			Node<T>* old_bottom = top->bottom;
			Node<T>* new_top = s;
			if (escape->parent == top) {
					top->bottom = TNULL;
					top->del_interval = false;
					escape = TNULL;
					top = TNULL;
			}
			else {
				top->bottom = escape->parent;
				escape = escape->parent;
			}
			if (new_top != old_bottom) {
				new_top->del_interval = true;
				new_top->bottom = old_bottom;
			}
		}
	}

	void myprint(Node<T>*& node) {
		if ((node != TNULL) && (node != nullptr)) {
			//cout << "key= " << node->key << endl;

			cout << "key= " << node->data << " color= " << node->color << " top = " << node->top->data << " bottom = " << node->bottom->data << endl;
			cout << "ins_interval= " << node->ins_interval << " del_interval= " << node->del_interval << endl;
			cout << "parent=" << node->parent->data << " left_child= " << node->left->data << " right_child= " << node->right->data << "\n\n" << endl;
		}
	}

	Node<T>* grandparent(Node<T>*& node) {
		if ((node != TNULL) && (node->parent != TNULL))
			return node->parent->parent;
		else
			return TNULL;
	}

	Node<T>* sibling(Node<T>*& node) {
		if ((node == TNULL) || (node == nullptr) || (node->parent == TNULL) || (node->parent == nullptr))
			return TNULL;
		else if (node == node->parent->left)
			return node->parent->right;
		else
			return node->parent->left;
	}

public:
	RBTree() {
		TNULL = new Node<T>;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->ins_interval = false;
		TNULL->del_interval = false;
		TNULL->top = nullptr;
		TNULL->bottom = nullptr;
		root = TNULL;
		TNULL->size = 0;
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
	Node<T>* searchTree(T k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	Node<T>* minimum(Node<T>* node, Node<T>* &top, Node<T>* &escape) { //modified by Taekang
		while (node->left != TNULL) {
			if (node->ins_interval || node->del_interval)
				top = node;
			if (top->bottom == node)
				escape = node;
			node->size--;
			node = node->left;
			if (top != TNULL && node->parent->data < top->bottom->data)
				escape = node;
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
			return minimum(x->right);
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
		node->ins_interval = false;
		node->del_interval = false;
		node->top = TNULL;
		node->bottom = TNULL;
		node->size = 1;

		Node<T>* y = nullptr;
		Node<T>* x = this->root;
		Node<T>* top = TNULL;  //added by Taekang
		Node<T>* escape = TNULL; //added by Taekang

		while (x != TNULL) {
			y = x;
			if (x->ins_interval || x->del_interval) {
				//Debug
				if (x->color == black) {
					int asdf = 1;
				}
				top = x;
				escape = TNULL;
			}
			if (top->bottom == x)
				escape = x;
			x->size++;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
			if (top != TNULL && x != TNULL && escape == TNULL && ((key < x->parent->data) != (top->bottom->data < x->parent->data)))
				escape = x;
		}

		// y is parent of x
		node->parent = y;
		if ((top != TNULL) && (escape == TNULL)) { //if node is inserted in the middle of insert interval
			Node<T>* old_bottom = top->bottom;
			Node<T>* s = y->left != TNULL ? y->left : y->right;
			Node<T>* new_top = s->data < old_bottom->data ? s->right : s->left;
			Node<T>* new_top_s = s->data < old_bottom->data ? s->left : s->right;
			s->color = red;
			//Debug
			if (new_top_s->ins_interval) {
				int asdf = 1;
			}
			new_top_s->color = black;
			if (y == top) {
				top->color = black;
				top->bottom = TNULL;
				top->ins_interval = false;
				escape = TNULL;
				top = TNULL;
			}
			else {
				top->bottom = y;
				escape = y;
			}
			if (new_top != old_bottom) {
				//Debug
				if (new_top->color == black) {
					int asdf = 1;
				}
				new_top->ins_interval = true;
				new_top->bottom = old_bottom;
			}
			else {
				//Debug
				if (new_top->ins_interval) {
					int asdf = 1;
				}
				new_top->color = black;
			}
		}
			
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
			if (node->ins_interval) {
				int asdf = 1;
			}
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}



		// Fix the tree
		fixInsert(node, top, escape);
	}

	Node<T>* getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

	//Debug
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